/* 
   Generating Sparse Matrices
   Matt Greeley, 2014
*/

#include <iostream>
#include <vector>

using namespace std;


vector<vector<double> > generateSparseMatrix(int iMax, int kMax, int jMax) {
    vector<vector<double> > matrix;
    vector<double> row;
    double aValue;
    int home, neighbor, new_k, new_j, new_i;
    for (int k = 0; k < kMax; ++k) {
        for (int j = 0; j < jMax; ++j) {
            for (int i = 0; i < iMax; ++i) {
                home = (k - 1) * jMax * iMax + (j - 1) * iMax + i;
                for (int nk = 0; nk < 2; ++nk) {
                    new_k = k + nk - 1;
                    if !(new_k < 1 || new_k > kMax) 
                        // What do we do here..?


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
