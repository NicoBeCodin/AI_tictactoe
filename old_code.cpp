#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <random>


//This is the old code before being refactored into classes


using namespace std;
typedef vector<vector<double>> Matrix;

//MATH

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
    for (int i = 1; i < vec.size(); ++i)
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

vector<double> softmax(vector<double>& values){
    double max = *max_element(values.begin(),values.end());
    double sum = 0.0;
    vector<double> probs(values.size());
    for(size_t i = 0; i<values.size(); ++i){
        probs[i] = exp(values[i]- max);
        sum += probs[i];
    }
    for(auto p: probs) p/=sum;
    return probs;
    
}

//Print matrix values for debu
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

class TicTacToe
{
public:
    TicTacToe()
    {
        board = vector<int>(9, 0); // Initialize empty board (0 = empty, 1 = X, -1 = O)
    }

    // Print the current board state (optional, for debugging)
    void printBoard()
    {
        for (int i = 0; i < 9; ++i)
        {
            if (i % 3 == 0)
                cout << "\n";
            if (board[i] == 1)
                cout << "X ";
            else if (board[i] == -1)
                cout << "O ";
            else
                cout << "- ";
        }
        cout << "\n";
    }

    // Get available actions (empty spots on the board)
    vector<int> getAvailableActions()
    {
        vector<int> actions;
        for (int i = 0; i < 9; ++i)
            if (board[i] == 0)
                actions.push_back(i);
        return actions;
    }

    // Make a move on the board
    bool makeMove(int pos, int player)
    {
        if (board[pos] == 0)
        {
            board[pos] = player;
            return true;
        }
        return false;
    }

    // Check if the game has ended and return the winner (1 = X, -1 = O, 0 = draw, -2 = not ended)
    int checkWinner()
    {
        // Check rows, columns, and diagonals
        for (int i = 0; i < 3; i++)
        {
            if (board[i * 3] != 0 && board[i * 3] == board[i * 3 + 1] && board[i * 3] == board[i * 3 + 2])
                return board[i * 3];
            if (board[i] != 0 && board[i] == board[i + 3] && board[i] == board[i + 6])
                return board[i];
        }
        if (board[0] != 0 && board[0] == board[4] && board[0] == board[8])
            return board[0];
        if (board[2] != 0 && board[2] == board[4] && board[2] == board[6])
            return board[2];

        // Check if it's a draw
        for (int i = 0; i < 9; i++)
            if (board[i] == 0)
                return -2; // Game is not finished

        return 0; // Draw
    }

    // Reset the board
    void reset()
    {
        board = vector<int>(9, 0);
    }

    vector<int> getState()
    {
        return board; // Return the current board state
    }

private:
    vector<int> board; // 3x3 Tic-Tac-Toe board represented as a 1D array of size 9
};


// Fully connected neural network for Q-learning
class NeuralNetwork
{
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize)
    {
        // Initialize weights and biases
        weightsInputHidden = randomMatrix(inputSize, hiddenSize);
        weightsHiddenOutput = randomMatrix(hiddenSize, outputSize);
        biasHidden = randomMatrix(1, hiddenSize);
        biasOutput = randomMatrix(1, outputSize);
        inputSize = inputSize;
        hiddenSize = hiddenSize;
        outputSize = outputSize;
    }

    // Forward pass: input -> hidden -> output
    Matrix forward(const Matrix &input)
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
    void backward(const Matrix &input, const Matrix &target)
    {
        // cout<< "Backward adjustment "<<endl;

        // Calculate error and deltas (gradient of the loss)

        // printMatrix(input, "input");
        Matrix outputError = matAdd(target, applyActivation(outputLayerActivated, [](double x)
                                                            { return -x; }));
        Matrix outputDelta = applyActivationDerivative(outputLayerActivated, sigmoidDerivative);
        for (int i = 0; i < outputDelta.size(); ++i)
            for (int j = 0; j < outputDelta[0].size(); ++j)
                outputDelta[i][j] *= outputError[i][j];


        Matrix hiddenError = matMul(outputDelta, transpose(weightsHiddenOutput));
        Matrix hiddenDelta = applyActivationDerivative(hiddenLayerActivated, reluDerivative);
        for (int i = 0; i < hiddenDelta.size(); ++i)
            for (int j = 0; j < hiddenDelta[0].size(); ++j)
                hiddenDelta[i][j] *= hiddenError[i][j];

        Matrix inputT = transpose(input);

        // printMatrix(inputT, "inputT");

        weightsInputHidden = matAdd(weightsInputHidden, matMul(inputT, hiddenDelta));
        weightsHiddenOutput = matAdd(weightsHiddenOutput, matMul(transpose(hiddenLayerActivated), outputDelta));

        biasHidden = matAdd(biasHidden, hiddenDelta);
        biasOutput = matAdd(biasOutput, outputDelta);
    }

    void printNetwork()
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

    Matrix getInputNetworkMatrix()
    {
        return weightsInputHidden;
    };

    Matrix getOutputNetworkMatrix()
    {
        return weightsHiddenOutput;
    };
    Matrix getInputNetworkBias()
    {
        return biasHidden;
    };
    Matrix getOutputNetworkBias()
    {
        return biasOutput;
    };

    // Save weights and biases to a file
    void saveWeights(const std::string &filename)
    {
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

            file.close();
        }
        else
        {
            std::cerr << "Unable to open file for saving weights.\n";
        }
    }

    // Load weights and biases from a file
    void loadWeights(const std::string &filename)
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

private:
    Matrix weightsInputHidden, weightsHiddenOutput;
    Matrix biasHidden, biasOutput;
    Matrix hiddenLayer, hiddenLayerActivated, outputLayer, outputLayerActivated;
    int inputSize, hiddenSize, outputSize;
};



// Choose an action using epsilon-greedy
int chooseAction(NeuralNetwork &nn, const Matrix &state, double epsilon, const vector<int> &availableActions)
{
    if ((double)rand() / RAND_MAX < epsilon)
    {
        // Explore: choose a random action
        return availableActions[rand() % availableActions.size()];
    }
    else
    {
        // Exploit: choose the best action (highest Q-value)
        Matrix qValues = nn.forward(state);
        int bestAction = availableActions[0];
        double bestValue = qValues[0][availableActions[0]];

        for (int action : availableActions)
        {
            if (qValues[0][action] > bestValue)
            {
                bestValue = qValues[0][action];
                bestAction = action;
            }
        }
        return bestAction;
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

// Update the Q-values (reward-based learning with temporal difference update)
void updateQValues(NeuralNetwork &nn, const Matrix &state, int action, double reward, const Matrix &nextState, double gamma, double learningRate)
{
    Matrix qValues = nn.forward(state);         // Q-values for current state
    Matrix nextQValues = nn.forward(nextState); // Q-values for next state

    // Calculate maximum Q-value from the next state (for the TD target)
    double maxNextQ = maxElement(nextQValues[0]);

    // Temporal difference target calculation
    double currentQ = qValues[0][action];
    double targetQ = reward + gamma * maxNextQ; // Q-learning target

    // Update Q-value for the action taken with TD update
    qValues[0][action] = currentQ + learningRate * (targetQ - currentQ);

    // Perform backward propagation to update weights
    nn.backward(state, qValues);
}

// In main(), after training is completed:
void playAgainstAI(NeuralNetwork &nn)
{
    TicTacToe game;
    int player = 1; // You are 'X'

    game.reset();
    cout << "Let's play Tic-Tac-Toe! You are 'X'.\n";
    game.printBoard();

    while (true)
    {
        if (player == 1)
        {
            int move;
            cout << "Your turn! Enter a move (0-8): ";
            cin >> move;

            // Ensure the move is valid
            while (move < 0 || move > 8 || !game.makeMove(move, player))
            {
                cout << "Invalid move! Try again (0-8): ";
                cin >> move;
            }
        }
        else
        {
            // AI's turn
            vector<int> boardState = game.getState();
            Matrix state = stateToMatrix(boardState);
            vector<int> availableActions = game.getAvailableActions();

            // Use the neural network to choose the action
            int aiMove = chooseAction(nn, state, 0.0, availableActions); // epsilon = 0 to use greedy action
            game.makeMove(aiMove, player);
            cout << "AI chooses position: " << aiMove << endl;
        }

        game.printBoard();

        // Check for winner
        int result = game.checkWinner();
        if (result == 1)
        {
            cout << "You (X) won! Congrats!\n";
            break;
        }
        else if (result == -1)
        {
            cout << "AI (O) won! Better luck next time.\n";
            break;
        }
        else if (result == 0)
        {
            cout << "It's a draw!\n";
            break;
        }

        // Switch player
        player *= -1;
    }
}

NeuralNetwork trainNeuralNetwork()
{
    srand(time(0)); 

    // Hyperparameters
    int numEpisodes = 20000;
    double learningRate = 0.01;
    double gamma = 0.9; // Discount factor
    double epsilon = 1.0;
    double epsilonDecay = 0.9995;
    double minEpsilon = 0.1;

    // Initialize Tic-Tac-Toe game and neural network
    TicTacToe game;
    NeuralNetwork nn(9, 64, 9); // 9 input nodes (board state), 64 hidden nodes, 9 output nodes (Q-values)

    for (int episode = 0; episode < numEpisodes; ++episode)
    {
        game.reset();
        vector<int> board = game.getState();
        int player = 1; // X starts

        // Play an episode
        while (true)
        {
            Matrix state = stateToMatrix(game.getState());
            // printMatrix(state, "state ");
            vector<int> availableActions = game.getAvailableActions();
            int action = chooseAction(nn, state, epsilon, availableActions);

            game.makeMove(action, player);
            int result = game.checkWinner();
            vector<int> nextBoard = game.getState();
            Matrix nextState = stateToMatrix(nextBoard);

            // Calculate reward
            double reward = -0.05;
            if (result == 1)
            {
                reward = (double)player; // X wins
            }
            else if (result == -1)
            {
                reward = -(double)player; // O wins
            }
            else if (result == 0)
            {
                reward = 0.5; // Draw
            }
            // Small negative reward to incentivize winning early

            // Update Q-values
            updateQValues(nn, state, action, reward, nextState, gamma, learningRate);

            // Switch player
            player *= -1;

            // End game if finished
            if (result != -2)
            {
                break;
            }
        }

        // Decay epsilon
        epsilon = max(minEpsilon, epsilon * epsilonDecay);

        if (episode % 100 == 0)
        {
            cout << "Episode: " << episode << " - Epsilon: " << epsilon << endl;
        }
    }

    return nn;
}


//policy gradient method



//Reinforce update training function
void trainPolicy(NeuralNetwork& nn, const vector<pair<Matrix, int>>& gameHistory, int outcome){
    double reward;
    
    if (outcome == 1) reward =1.0;
    else if (outcome == -1) reward = -1.0;
    else reward =0.0;

    for (size_t t = 0; t< gameHistory.size(); ++t){
        const auto& [state, action] =gameHistory[t];


        int player = (t % 2 == 0) ? 1: -1;
        double adjustedReward = player*reward;

        Matrix output = nn.forward(state);
        vector<double> probs = softmax(output[0]);

        probs[action] -= 1;
        for (size_t i =0; i<probs.size(); ++i){
            output[0][i] = adjustedReward * probs[i];

        }
        nn.backward(state, output);
    }
}

int sampleAction(vector<double> actions){
    random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(actions.begin(), actions.end());
    return dist(gen);
}



pair<vector<pair<Matrix,int>>,int> playGame(NeuralNetwork& nn, TicTacToe& game){
    vector<pair<Matrix, int>> gameHistory;

    int player =1;

    while(true){
        Matrix state = stateToMatrix(game.getState());
        vector<int> availableActions = game.getAvailableActions();


        vector<double> actionProbs = softmax(nn.forward(state)[0]);

        vector<double>availableActionProbs(9,0.0);
        for (int action: availableActions){
            availableActionProbs[action] = actionProbs[action];

        }

        double sumProbs = accumulate(availableActionProbs.begin(), availableActionProbs.end(), 0.0);
        for (double& prob: actionProbs) {
            prob /=sumProbs;
        }

        int action = sampleAction(availableActionProbs);
        game.makeMove(action, player);
        gameHistory.push_back({state, action});

        int result = game.checkWinner();
        if (result ==1) return {gameHistory, player};
        else if (result==-1) return {gameHistory, -player};
        else if (result== 0) return {gameHistory, 0};

        player *= -1;

    }

}


NeuralNetwork neuralNetworkTraining(){
    int max_episodes = 100000;
    NeuralNetwork nn(9,64,9);
    TicTacToe game;

    for (int episode = 0; episode<max_episodes; ++episode ){
        auto [gameHistory, outcome] = playGame(nn, game);

        double reward = (outcome ==1) ? 1.0 : (outcome==-1) ? -1.0: 0.0;

        trainPolicy(nn, gameHistory, reward);

        if (episode % 1000 == 0){
            cout << "Episode: "<< episode<< endl;
        }
    }
    return nn;
}

int main()
{

    NeuralNetwork nn = neuralNetworkTraining();
    nn.saveWeights("weights_2.txt");

    // NeuralNetwork nn(9,128,9);
    // nn.loadWeights("weights_1.txt");

    cout << "Trying game... " << endl;

    playAgainstAI(nn);

    return 0;
}