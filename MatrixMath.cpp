#include <cmath>
#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "MatrixMath.h"
using namespace std;
typedef vector<vector<double>> Matrix;

// MATH

Matrix randomMatrix(int rows, int cols)
{
    Matrix mat(rows, vector<double>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            mat[i][j] = ((double)rand() / RAND_MAX) * 2 - 1; // Random between -1 and 1
    return mat;
}

// Matrix multiplication
Matrix matMul(const Matrix &A, const Matrix &B)
{
    int rowsA = A.size(), colsA = A[0].size(), colsB = B[0].size();
    Matrix result(rowsA, vector<double>(colsB, 0));
    for (int i = 0; i < rowsA; ++i)
        for (int j = 0; j < colsB; ++j)
            for (int k = 0; k < colsA; ++k)
                result[i][j] += A[i][k] * B[k][j];
    return result;
}

// Matrix addition
Matrix matAdd(const Matrix &A, const Matrix &B)
{
    int rows = A.size(), cols = A[0].size();
    Matrix result(rows, vector<double>(cols, 0));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result[i][j] = A[i][j] + B[i][j];
    return result;
}

// Transpose of a matrix
Matrix transpose(const Matrix &A)
{
    int rows = A.size(), cols = A[0].size();
    Matrix result(cols, vector<double>(rows, 0));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result[j][i] = A[i][j];
    return result;
}

// Apply activation function to matrix elements
Matrix applyActivation(const Matrix &M, double (*activation)(double))
{
    int rows = M.size(), cols = M[0].size();
    Matrix result(rows, vector<double>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result[i][j] = activation(M[i][j]);
    return result;
}

// Apply derivative of activation function to matrix elements
Matrix applyActivationDerivative(const Matrix &M, double (*activationDeriv)(double))
{
    int rows = M.size(), cols = M[0].size();
    Matrix result(rows, vector<double>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result[i][j] = activationDeriv(M[i][j]);
    return result;
}

double maxElement(vector<double> vec)
{
    double max_elem = vec[0];
    for (size_t i = 1; i < vec.size(); ++i)
    {
        if (vec[i] > max_elem)
            max_elem = vec[i];
    }
    return max_elem;
}

// Sigmoid activation function
double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

// Derivative of sigmoid (for backpropagation)
double sigmoidDerivative(double x)
{
    return x * (1.0 - x); // Assumes x is already passed through sigmoid
}

double relu(double x)
{
    return x > 0 ? x : 0;
}
double reluDerivative(double x)
{
    return x > 0 ? 1 : 0;
}

vector<double> softmax(vector<double> &values)
{
    double max = *max_element(values.begin(), values.end());
    double sum = 0.0;
    vector<double> probs(values.size());
    for (size_t i = 0; i < values.size(); ++i)
    {
        probs[i] = exp(values[i] - max);
        sum += probs[i];
    }
    for (auto p : probs)
        p /= sum;
    return probs;
}

// Print matrix values for debu
void printMatrix(const Matrix matrix, string text = " ")
{
    cout << text << endl;
    int i = 0;
    for (const auto &row : matrix)
    {
        cout << "row: " << i++ << endl;
        for (const auto &value : row)
        {
            cout << value << " ";
        }
        cout << endl;
    }
}

// Convert board state to input matrix
Matrix stateToMatrix(const vector<int> &board)
{
    Matrix input(1, vector<double>(9));
    for (int i = 0; i < 9; ++i)
    {
        input[0][i] = board[i];
    }
    return input;
}

int sampleAction(vector<double> actions){
    random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(actions.begin(), actions.end());
    return dist(gen);
}
