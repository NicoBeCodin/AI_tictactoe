#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#include <vector>
#include <string>
#include <random>

typedef std::vector<std::vector<double>> Matrix;

// Matrix operations
Matrix randomMatrix(int rows, int cols);
Matrix matMul(const Matrix &A, const Matrix &B);
Matrix matAdd(const Matrix &A, const Matrix &B);
Matrix matScalarMultiply(const Matrix &A, double scalar);
Matrix transpose(const Matrix &A);

// Activation functions and their derivatives
double sigmoid(double x);
double sigmoidDerivative(double x);
double relu(double x);
double reluDerivative(double x);
Matrix applyActivation(const Matrix &M, double (*activation)(double));
Matrix applyActivationDerivative(const Matrix &M, double (*activationDeriv)(double));

// Softmax and sampling
std::vector<double> softmax(std::vector<double> &values);
int sampleAction(std::vector<double> actions);

// Utility functions
double maxElement(std::vector<double> vec);
void printMatrix(const Matrix matrix, std::string text);
Matrix stateToMatrix(const std::vector<int> &board);
std::vector<int> matrixToState(const Matrix &board);
int randomAction(std::vector<int> actions);

#endif // MATRIX_MATH_H
