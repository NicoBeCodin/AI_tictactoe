// Trainer.h
#ifndef TRAINER_H
#define TRAINER_H

#include "Agent.h"

class Trainer {
public:
    Trainer(double gamma, double learningRate);
    void trainPolicyGradient(Agent& agent, TicTacToe& game, int episodes);
    void trainPolicyGradientTwo(Agent& agent1, Agent& agent2, TicTacToe& game, int episodes);
    void playAgainstAI(NeuralNetwork& nn);
    void playAgainstMinimax();

    void trainQAgent(QAgent& qAgent, int episodes, double alpha, double gamma, double epsilon_decay, double epsilon_min);
    void playAgainstQAgent(QAgent& qAgent);  
private:
    double gamma;
    double learningRate;
};

#endif