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
    NeuralNetwork nn(9, 64, 9);
    nn.printNetwork();  
    Agent agent(&nn, 0.1);
    TicTacToe game;
    Trainer trainer(0.9, 0.01);

    // Train the agent
    int episodes = 500000;
    trainer.trainPolicyGradient(agent, game, episodes);

    cout<<"Printing network: "<<endl;
    nn.printNetwork();
    // Save the trained model weights
    nn.saveWeights("trained_weights.txt");
    
    
    trainer.playAgainstAI(nn);

    return 0;
}