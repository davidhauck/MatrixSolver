//CudaTest.cpp : Tests CUDA solving of a sparse matrix
//David Hauck

//Some setup code taken from CUSPARSE demo program "conjugateGradient"
//	http://docs.nvidia.com/cuda/cuda-samples/index.html#conjugategradient

#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctime>

#include <cuda_runtime.h>
#include <cusparse_v2.h>
#include <cublas_v2.h>

#include <cuda.h>
#include <helper_functions.h>  // helper for shared functions common to CUDA SDK samples
#include <helper_cuda.h>       // helper function CUDA error checking and intialization


//Generates a sparse matrix in CSR format
//Taken from the CUSPARSE sample project
void genTridiag(int *I, int *J, cuDoubleComplex *val, int N, int nz)
{
	I[0] = 0, J[0] = 0, J[1] = 1;
	val[0] = make_cuDoubleComplex(rand() / RAND_MAX + 10.0f, rand() / RAND_MAX + 10.0f);
	val[1] = make_cuDoubleComplex(rand() / RAND_MAX, rand() / RAND_MAX);
	int start;

	for (int i = 1; i < N; i++)
	{
		if (i > 1)
		{
			I[i] = I[i - 1] + 3;
		}
		else
		{
			I[1] = 2;
		}

		start = (i - 1) * 3 + 2;
		J[start] = i - 1;
		J[start + 1] = i;

		if (i < N - 1)
		{
			J[start + 2] = i + 1;
		}

		val[start] = val[start - 1];
		val[start + 1] = make_cuDoubleComplex(rand() / RAND_MAX + 10.0f, rand() / RAND_MAX + 10.0f);

		if (i < N - 1)
		{
			val[start + 2] = make_cuDoubleComplex(rand() / RAND_MAX, rand() / RAND_MAX);
		}
	}

	I[N] = nz;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int M = 0, N = 0, nz = 0, *I = NULL, *J = NULL;
	cuDoubleComplex *val = NULL;
	cuDoubleComplex *x, *y;
	cuDoubleComplex *d_x, *d_y;
	double duration, duration_setup;


	std::clock_t setup_clock;
	setup_clock = std::clock();
	// This will pick the best possible CUDA capable device
	cudaDeviceProp deviceProp;
	int devID = findCudaDevice(argc, (const char **)argv);

	if (devID < 0)
	{
		printf("no devices found...\n");
		exit(EXIT_SUCCESS);
	}

	checkCudaErrors(cudaGetDeviceProperties(&deviceProp, devID));

	// Statistics about the GPU device
	printf("> GPU device has %d Multi-Processors, SM %d.%d compute capabilities\n\n",
		deviceProp.multiProcessorCount, deviceProp.major, deviceProp.minor);

	int version = (deviceProp.major * 0x10 + deviceProp.minor);

	if (version < 0x11)
	{
		printf("Requires a minimum CUDA compute 1.1 capability\n");
		cudaDeviceReset();
		exit(EXIT_SUCCESS);
	}

	M = N = 8388608; //2 ^ 23
	//M = N = 4194304; //2 ^ 22
	//M = N = 2097152; //2 ^ 21
	//M = N = 1048576; //2 ^ 20
	//M = N = 524288; //2 ^ 19

	nz = N * 8;
	I = (int *)malloc(sizeof(int)*(N + 1));
	J = (int *)malloc(sizeof(int)*nz);
	val = (cuDoubleComplex *)malloc(sizeof(cuDoubleComplex)*nz);
	genTridiag(I, J, val, N, nz);

	x = (cuDoubleComplex*)malloc(sizeof(cuDoubleComplex)* N);
	y = (cuDoubleComplex*)malloc(sizeof(cuDoubleComplex)* N);

	//create an array for the answer array (Y) and set all of the answers to 0 for the test (could do random)
	for (int i = 0; i < N; i++)
	{
		y[i] = make_cuDoubleComplex(0.0, 0.0);
	}

	//Get handle to the CUBLAS context
	cublasHandle_t cublasHandle = 0;
	cublasStatus_t cublasStatus;
	cublasStatus = cublasCreate(&cublasHandle);

	checkCudaErrors(cublasStatus);

	//Get handle to the CUSPARSE context
	cusparseHandle_t cusparseHandle = 0;
	cusparseStatus_t cusparseStatus;
	cusparseStatus = cusparseCreate(&cusparseHandle);

	checkCudaErrors(cusparseStatus);

	//Get handle to a CUSPARSE matrix descriptor
	cusparseMatDescr_t descr = 0;
	cusparseStatus = cusparseCreateMatDescr(&descr);

	checkCudaErrors(cusparseStatus);

	//Get handle to a matrix_solve_info object
	cusparseSolveAnalysisInfo_t info = 0;
	cusparseStatus = cusparseCreateSolveAnalysisInfo(&info);

	checkCudaErrors(cusparseStatus);

	cusparseSetMatType(descr, CUSPARSE_MATRIX_TYPE_GENERAL);
	cusparseSetMatIndexBase(descr, CUSPARSE_INDEX_BASE_ZERO);

	duration_setup = (std::clock() - setup_clock) / (double)CLOCKS_PER_SEC;
	printf("setup_time: %f\r\n", duration_setup);

	std::clock_t start;
	start = std::clock();
	checkCudaErrors(cudaMalloc((void **)&d_x, N*sizeof(float)));
	checkCudaErrors(cudaMalloc((void **)&d_y, N*sizeof(float)));

	cudaMemcpy(d_x, x, N*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_y, y, N*sizeof(float), cudaMemcpyHostToDevice);

	//Analyze the matrix. The info variable is needed to perform additional operations on the matrix
	cusparseStatus = cusparseZcsrsv_analysis(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, N, nz, descr, val, J, I, info);
	//Uses infor gathered from the matrix to solve the matrix.
	cusparseStatus = cusparseZcsrsv_solve(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, N, 0, descr, val, J, I, info, d_x, d_y);

	//Get the result back from the device
	cudaMemcpy(x, d_x, N*sizeof(float), cudaMemcpyDeviceToHost);
	cudaMemcpy(y, d_y, N*sizeof(float), cudaMemcpyDeviceToHost);

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	printf("time ellapsed: %f", duration);

	//free up memory
	cusparseDestroy(cusparseHandle);
	cublasDestroy(cublasHandle);
	free(I);
	free(J);
	free(val);
	free(x);
	cudaFree(d_x);
	cudaDeviceReset();

	//Wait for user input so they can see the results
	char* s = (char*)malloc(sizeof(char) * 8);
	scanf(s);

	exit(0);
}
