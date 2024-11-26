#ifndef AGENT_H
#define AGENT_H

#include <unordered_map>
#include <map>
#include "NeuralNetwork.h"
#include "MatrixMath.h"
#include "Game.h"

class Agent{
public:
    Agent(NeuralNetwork* policyNet, double espilon = 0.1);

    int chooseAction(const Matrix& state, const vector<int> &availableActions);
    //policy gradient
    int chooseMove(TicTacToe &game);
    void trainPolicy(const vector<pair<Matrix, int>>& gameHistory, double outcome);
    void trainPolicy_deprecated(const vector<pair<Matrix, int>>& gameHistory, double outcome);
    
    void updateQValues(const Matrix& state, int action, double reward, const Matrix &nextState, double gamma, double learningRate);
    
    pair<vector<pair<Matrix,int>>,int> playGame(TicTacToe& game);
    pair<vector<pair<Matrix, int>>, int> playGameTwo(TicTacToe& game, Agent& opponent);
    

    int minimax(TicTacToe& game, int depth, bool isMaximizing);
    int findBestMove(TicTacToe& game, int player);


private: 
    NeuralNetwork* policyNet;
    double epsilon;
    
    
};

class QAgent{
public:
    QAgent(double epsilon, map<pair<vector<int>, int>, double> qTable);

    vector<double> initializeQValues();
    int selectAction(TicTacToe &game);
    void updateQValue(vector<int>& state, int action, vector<int>& nextState, vector<int>& nextAvailableActions, double reward, double alpha, double gamma, bool debug);
    void decreaseEpsilon(double coeff, double lowest);
    void printQTable();
    void printFirstFiveQTable();
    double getQValue(pair<vector<int>, int> key);
    void saveQTable(const string& filename);
    void loadQTable(const string& filename);
private: 
    
    double epsilon;
    map<pair<vector<int>, int>, double> qTable;

};

#endif