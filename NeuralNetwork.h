#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include <string>

typedef std::vector<std::vector<double>> Matrix;

// Neural Network class for Q-learning
class NeuralNetwork
{
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize);

    // Forward and backward propagation
    Matrix forward(const Matrix &input);
    void backward(const Matrix &input, const Matrix &target);

    // Print network details (for debugging)
    void printNetwork();

    // Getters for weights and biases (optional)
    Matrix getInputNetworkMatrix() const;
    Matrix getOutputNetworkMatrix() const;
    Matrix getInputNetworkBias() const;
    Matrix getOutputNetworkBias() const;

    // Saving and loading model weights
    void saveWeights(const std::string &filename);
    void loadWeights(const std::string &filename);

private:
    Matrix weightsInputHidden, weightsHiddenOutput;
    Matrix biasHidden, biasOutput;
    Matrix hiddenLayer, hiddenLayerActivated, outputLayer, outputLayerActivated;
    int inputSize, hiddenSize, outputSize;
};

#endif // NEURAL_NETWORK_H
