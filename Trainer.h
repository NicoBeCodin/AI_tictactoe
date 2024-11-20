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
private:
    double gamma;
    double learningRate;
};

#endif