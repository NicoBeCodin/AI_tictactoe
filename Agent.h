#ifndef AGENT_H
#define AGENT_H

#include "NeuralNetwork.h"
#include "MatrixMath.h"
#include "Game.h"

class Agent{
public:
    Agent(NeuralNetwork* policyNet, double espilon = 0.1);
    int chooseAction(const Matrix& state, const vector<int> &availableActions);
    void trainPolicy(const vector<pair<Matrix, int>>& gameHistory, int outcome);
    void updateQValues(const Matrix& state, int action, double reward, const Matrix &nextState, double gamma, double learningRate);
    pair<vector<pair<Matrix,int>>,int> playGame(TicTacToe& game);
    
private: 
    NeuralNetwork* policyNet;
    double epsilon;
};

#endif