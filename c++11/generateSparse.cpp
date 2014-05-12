/* 
   Generating Sparse Matrices
   Matt Greeley, 2014
*/

#include <iostream>
#include <vector>
#include <complex>

using namespace std;


void generateSparseMatrix(int iMax, int kMax, int jMax) {
    int nMax = iMax * jMax * kMax;
    complex<double> matrix[nMax][nMax];
    int home, neighbor, new_k, new_j, new_i;
    for (int k = 0; k < kMax; ++k) {
        for (int j = 0; j < jMax; ++j) {
            for (int i = 0; i < iMax; ++i) {
                home = (k - 1) * jMax * iMax + (j - 1) * iMax + i;
                for (int nk = 0; nk < 2; ++nk) {
                    new_k = k + nk - 1;
                    if (!(new_k < 1 || new_k > kMax)) {
                        for (int nj = 0; nj < 2; ++nj) {
                            new_j = j + nj - 1;
                            if (!(new_j < 1 || new_j > jMax)) {
                                for (int ni = 0; ni < 2; ++ni) {
                                    new_i = i + ni - 1;
                                    if !(new_i < 1 || new_i > iMax) {
                                        neighbor = (new_k - 1) * jMax * iMax + (new_j - 1) * iMax + new_i
                                        if (neighbor > home) {
                                            complex<double> aValue(jMax - j + 1, iMax - i + 1);
                                            matrix[home][home] = matrix[home][home] + aValue;
                                            matrix[neighbor][neighbor] = matrix[neighbor][neighbor] + aValue;
                                            matrix[home][neighbor] = -1 * aValue;
                                            matrix[neighbor][home] = -1 * aValue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (k == kMax)
                    matrix[home][home] = matrix[home][home] + aValue;
            }
        }
    }

    for (int i = 0; i < nMax; ++i) {
        for (int j = 0; j < nMax; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }



}

void printMatrix(vector<vector<double> > matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            cout << matrix[i][j];
            cout << " ";
        }
        cout << endl;
    }
}

int main() {
    // Get user input for iMax, kMax, and jMax
    int iMax, kMax, jMax;
    cout << "Generating Sparse Matrices\n\niMax: " << flush; cin >> iMax; 
    cout << "kMax: " << flush; cin >> kMax;
    cout << "jMax: " << flush; cin >> jMax;

    generateSparseMatrix(iMax, kMax, jMax);


}
