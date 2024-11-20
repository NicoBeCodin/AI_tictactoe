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
    NeuralNetwork nnOne(9, 64, 9);
    NeuralNetwork nnTwo(9, 64, 9);
    
    Agent agentOne(&nnOne, 0.05);
    Agent agentTwo(&nnTwo, 0.05);
    TicTacToe game;
    Trainer trainer(0.9, 0.01);

    // Train the agent
    int episodes = 100000;
    // trainer.trainPolicyGradientTwo(agentOne,agentTwo, game, episodes);

    // Save the trained model weights

    // nnOne.saveWeights("trained_weights.txt");
    nnOne.loadWeights("trained_weights.txt");
    
    // trainer.playAgainstAI(nnOne);
    // trainer.playAgainstAI(nnTwo);
    trainer.playAgainstMinimax();

    return 0;
}