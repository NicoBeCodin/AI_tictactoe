#ifndef AGENT_H
#define AGENT_H

#include "NeuralNetwork.h"
#include "MatrixMath.h"
#include "Game.h"

class Agent{
public:
    Agent(NeuralNetwork* policyNet, double espilon = 0.1);
    int chooseAction(const Matrix& state, const vector<int> &availableActions);
    int chooseMove(TicTacToe &game);
    void trainPolicy(const vector<pair<Matrix, int>>& gameHistory, double outcome);
    void trainPolicy_deprecated(const vector<pair<Matrix, int>>& gameHistory, double outcome);
    void updateQValues(const Matrix& state, int action, double reward, const Matrix &nextState, double gamma, double learningRate);
    
    int minimax(TicTacToe& game, int depth, bool isMaximizing);
    int findBestMove(TicTacToe& game, int player);

    pair<vector<pair<Matrix,int>>,int> playGame(TicTacToe& game);
    pair<vector<pair<Matrix, int>>, int> playGameTwo(TicTacToe& game, Agent& opponent);
    
private: 
    NeuralNetwork* policyNet;
    double epsilon;
};

#endif