using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace TestGaussian
{
    class Program
    {
        static void Main(string[] args)
        {
            var newMat = GenerateMatrix(10);
            var result = GaussSeidel(newMat);

            Console.WriteLine("Gaussian Elimination1");
            //PrintMatrix(newMat);
            Console.Write("\r\n");

            //List<List<double>> geMatrix = new List<List<double>>() { new List<double>() { 5, -2, 3, -1 }, new List<double>() { -3, 9, 1, 2 }, new List<double>() { 2, -1, -7, 3 } };

            ////Gaussian Elimination
            //GaussianElimination(geMatrix);

            //Console.WriteLine("Gaussian Elimination2");
            //PrintMatrix(geMatrix);
            //Console.Write("\r\n");

            ////Gauss-Seidel

            //List<List<double>> gsMatrix = new List<List<double>>() { new List<double>() { 5, -2, 3, -1 }, new List<double>() { -3, 9, 1, 2 }, new List<double>() { 2, -1, -7, 3 } };
            //List<double> result = GaussSeidel(gsMatrix);
            
            //Console.WriteLine("Gauss-Seidel");
            for (int i = 0; i < result.Count; i++)
            {
                Console.WriteLine(String.Format("X[{0}] = {1}", (i + 1), result[i]));
            }
            Console.Read();
        }
        
        private static void GaussianElimination(List<List<double>> geMatrix)
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            for (int i = 0; i < geMatrix[0].Count - 2; i++)
            {
                for (int j = i + 1; j < geMatrix.Count; j++)
                {
                    double number = geMatrix[j][i] / geMatrix[i][i] * -1;
                    for (int k = i; k < geMatrix[0].Count; k++)
                    {
                        geMatrix[j][k] = geMatrix[j][k] + number * geMatrix[i][k];
                    }
                }
            }
            sw.Stop();
            Console.WriteLine(String.Format("Elapsed Milliseconds: {0}", sw.ElapsedMilliseconds));
        }

        private static List<List<double>> GenerateMatrix(int p)
        {
            Random r = new Random(DateTime.Now.Millisecond);
            List<List<double>> newMatrix = new List<List<double>>();
            for (int i = 0 ; i < p - 1; i++)
            {
                List<double> newList = new List<double>(p);
                for (int j = 0; j < p; j++)
                {
                    newList.Add(r.Next(-10, 10));
                }
                newMatrix.Add(newList);
            }
            return newMatrix;
        }

        private static List<double> GaussSeidel(List<List<double>> gsMatrix, int iterations = 7)
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            List<double> x = new List<double>(gsMatrix.Count);
            for (int i = 0; i < gsMatrix.Count; i++)
            {
                x.Add(0);
            }

            for (int k = 0; k < iterations; k++)
            {
                for (int i = 0; i < gsMatrix.Count; i++)
                {
                    x[i] = 0;
                    for (int j = 0; j < gsMatrix[0].Count - 1; j++)
                    {
                        if (i != j)
                            x[i] -= gsMatrix[i][j] / gsMatrix[i][i] * x[j];
                    }

                    x[i] += gsMatrix[i][gsMatrix[0].Count - 1] / gsMatrix[i][i];
                }
            }
            sw.Stop();
            Console.WriteLine(String.Format("Elapsed Milliseconds: {0}", sw.ElapsedMilliseconds));
            return x;
        }

        static void PrintMatrix(List<List<double>> matrix)
        {
            foreach (var x in matrix)
            {
                foreach (var y in x)
                {
                    Console.Write(y + "\t");
                }
                Console.Write("\r\n");
            }
        }
    }
}
