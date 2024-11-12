// Trainer.h
#ifndef TRAINER_H
#define TRAINER_H

#include "Agent.h"

class Trainer {
public:
    Trainer(double gamma, double learningRate);
    void trainPolicyGradient(Agent& agent, TicTacToe& game, int episodes);
    void playAgainstAI(NeuralNetwork& nn);
private:
    double gamma;
    double learningRate;
};

#endif