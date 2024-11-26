#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Trainer.h"
#include "Agent.h"
#include "NeuralNetwork.h"


using namespace std;
typedef vector<vector<double>> Matrix;

// int main()
// {



//     NeuralNetwork nn = neuralNetworkTraining();
//     nn.saveWeights("weights_2.txt");

//     // NeuralNetwork nn(9,128,9);
//     // nn.loadWeights("weights_1.txt");

//     cout << "Trying game... " << endl;

//     playAgainstAI(nn);

//     return 0;
// }


int main(){
    TicTacToe game;
    NeuralNetwork nnOne(9, 64, 9);
    NeuralNetwork nnTwo(9, 64, 9);
    
    Agent agentOne(&nnOne, 0.05);
    Agent agentTwo(&nnTwo, 0.05);

    Trainer trainer(0.9, 0.01);

    int episodes = 500000;
    // Train the agent
    // trainer.trainPolicyGradientTwo(agentOne,agentTwo, game, episodes);

    // Save the trained model weights

    // nnOne.saveWeights("trained_weights.txt");
    nnOne.loadWeights("trained_weights.txt");
    
    // trainer.playAgainstAI(nnOne);
    // trainer.playAgainstAI(nnTwo);
    // trainer.playAgainstMinimax();
    map<pair<vector<int>, int>, double> qTable;
    double epsilon = 0.95;
    QAgent qAgent(epsilon, qTable);
    trainer.trainQAgent(qAgent, episodes, 0.3, 0.95);
    qAgent.saveQTable("Qtable_test.txt");
    trainer.playAgainstQAgent(qAgent);
    return 0;
}