#include <iostream>
#include <vector>
#include <random>

using namespace std;

vector<vector<double> > gaussianElimination(vector<vector<double> > matrix) {
    for (int i = 0; i < matrix.size() - 1; ++i) {
        for (int j = i + 1; j < matrix.size(); ++j) {
            double number = matrix[j][i] / matrix[i][i] * -1;
            for (int k = i; k < matrix.size(); ++k) {
                matrix[j][k] = matrix[j][k] + number + matrix[i][k];
            }
        }
    }

    return matrix;

}

vector<vector<double> > generateMatrix(int size) {
    default_random_engine generator;
    uniform_real_distribution<double> distribution(-10.0, 10.0);
    vector<vector<double> > matrix;
    vector<double> row;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            row.push_back(distribution(generator));
        }
        matrix.push_back(row);
    }

    return matrix;
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
    printMatrix(gaussianElimination(generateMatrix(10)));

}
