
#include "NeuralNetwork.h"
#include "MatrixMath.h"


using namespace std;
// Fully connected neural network for Q-learning

NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize):inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize){
   
    weightsInputHidden = randomMatrix(inputSize, hiddenSize);
    weightsHiddenOutput = randomMatrix(hiddenSize, outputSize);
    biasHidden = randomMatrix(1, hiddenSize);
    biasOutput = randomMatrix(1, outputSize);

    
}


// Forward pass: input -> hidden -> output
Matrix NeuralNetwork::forward(const Matrix &input)
{
    // Input to hidden layer
    hiddenLayer = matAdd(matMul(input, weightsInputHidden), biasHidden);
    hiddenLayerActivated = applyActivation(hiddenLayer, relu);

    // Hidden to output layer (Q-values for each action)
    outputLayer = matAdd(matMul(hiddenLayerActivated, weightsHiddenOutput), biasOutput);
    outputLayerActivated = applyActivation(outputLayer, sigmoid); // You can use softmax here as well

    return outputLayerActivated; // Return the Q-values
}

// Backward pass (update weights)
void NeuralNetwork::backward(const Matrix &input, const Matrix &target)
{
    // cout<< "Backward adjustment "<<endl;

    // Calculate error and deltas (gradient of the loss)

    // printMatrix(input, "input");
    Matrix outputError = matAdd(target, applyActivation(outputLayerActivated, [](double x)
                                                        { return -x; }));
    Matrix outputDelta = applyActivationDerivative(outputLayerActivated, sigmoidDerivative);
    for (size_t i = 0; i < outputDelta.size(); ++i)
        for (size_t j = 0; j < outputDelta[0].size(); ++j)
            outputDelta[i][j] *= outputError[i][j];

    Matrix hiddenError = matMul(outputDelta, transpose(weightsHiddenOutput));
    Matrix hiddenDelta = applyActivationDerivative(hiddenLayerActivated, reluDerivative);
    for (size_t i = 0; i < hiddenDelta.size(); ++i)
        for (size_t j = 0; j < hiddenDelta[0].size(); ++j)
            hiddenDelta[i][j] *= hiddenError[i][j];

    Matrix inputT = transpose(input);

    // printMatrix(inputT, "inputT");

    weightsInputHidden = matAdd(weightsInputHidden, matMul(inputT, hiddenDelta));
    weightsHiddenOutput = matAdd(weightsHiddenOutput, matMul(transpose(hiddenLayerActivated), outputDelta));

    biasHidden = matAdd(biasHidden, hiddenDelta);
    biasOutput = matAdd(biasOutput, outputDelta);
}



void NeuralNetwork::printNetwork() 
{

    cout << "weightsInputHidden: \n"
         << endl;
    
    for (int i = 0; i < inputSize; ++i)
    {
        for (int j = 0; j < hiddenSize; ++j)
        {
            cout << weightsInputHidden[i][j] << " ";
        }
        cout << "biasHidden: " << biasHidden[0][i] << " \n";
        cout << endl;
    }

    cout << "WeightHiddenOutput: \n"
         << endl;
    for (int i = 0; i < hiddenSize; ++i)
    {
        for (int j = 0; j < outputSize; ++j)
        {
            cout << weightsHiddenOutput[i][j] << " ";
        }

        cout << "biasOutput: " << biasOutput[0][i] << " \n";
        cout << endl;
    }
};

Matrix NeuralNetwork::getInputNetworkMatrix() const
{
    return weightsInputHidden;
};

Matrix NeuralNetwork::getOutputNetworkMatrix() const
{
    return weightsHiddenOutput;
};
Matrix NeuralNetwork::getInputNetworkBias() const
{
    return biasHidden;
};
Matrix NeuralNetwork::getOutputNetworkBias() const
{
    return biasOutput;
};

// Save weights and biases to a file
void NeuralNetwork::saveWeights(const std::string &filename) 
{
    cout << "Current working directory: " << filesystem::current_path() << endl;

    std::ofstream file(filename);
    if (file.is_open())
    {
        // Save weightsInputHidden
        for (const auto &row : weightsInputHidden)
        {
            for (const auto &value : row)
                file << value << " ";
            file << "\n";
        }

        // Separate each matrix with a blank line for easy loading
        file << "\n";

        // Save weightsHiddenOutput
        for (const auto &row : weightsHiddenOutput)
        {
            for (const auto &value : row)
                file << value << " ";
            file << "\n";
        }

        file << "\n";

        // Save biasHidden
        for (const auto &value : biasHidden[0])
            file << value << " ";
        file << "\n\n";

        // Save biasOutput
        for (const auto &value : biasOutput[0])
            file << value << " ";

        cout<<"Weights succesfully written to "<< filename<<endl;
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file for saving weights.\n";
    }
}

// Load weights and biases from a file
void NeuralNetwork::loadWeights(const std::string &filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;

        // Load weightsInputHidden
        for (auto &row : weightsInputHidden)
        {
            std::getline(file, line);
            std::istringstream iss(line);
            for (auto &value : row)
                iss >> value;
        }

        // Skip blank line
        std::getline(file, line);

        // Load weightsHiddenOutput
        for (auto &row : weightsHiddenOutput)
        {
            std::getline(file, line);
            std::istringstream iss(line);
            for (auto &value : row)
                iss >> value;
        }

        // Skip blank line
        std::getline(file, line);

        // Load biasHidden
        std::getline(file, line);
        std::istringstream issHidden(line);
        for (auto &value : biasHidden[0])
            issHidden >> value;

        // Skip blank line
        std::getline(file, line);

        // Load biasOutput
        std::getline(file, line);
        std::istringstream issOutput(line);
        for (auto &value : biasOutput[0])
            issOutput >> value;

        file.close();
    }
    else
    {
        std::cerr << "Unable to open file for loading weights.\n";
    }
}
