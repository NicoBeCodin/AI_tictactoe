#include "Agent.h"

Agent::Agent(NeuralNetwork* policyNet, double epsilon):policyNet(policyNet), epsilon(epsilon){};


int Agent::chooseAction(const Matrix &state, const vector<int> &availableActions)
{
    if ((double)rand() / RAND_MAX < epsilon)
    {
        // Explore: choose a random action
        return availableActions[rand() % availableActions.size()];
    }
    else
    {
        // Exploit: choose the best action (highest Q-value)
        Matrix qValues = (*policyNet).forward(state);
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

// Update the Q-values (reward-based learning with temporal difference update)
void Agent::updateQValues(const Matrix &state, int action, double reward, const Matrix &nextState, double gamma, double learningRate)
{
    Matrix qValues = (*policyNet).forward(state);         // Q-values for current state
    Matrix nextQValues = (*policyNet).forward(nextState); // Q-values for next state

    // Calculate maximum Q-value from the next state (for the TD target)
    double maxNextQ = maxElement(nextQValues[0]);

    // Temporal difference target calculation
    double currentQ = qValues[0][action];
    double targetQ = reward + gamma * maxNextQ; // Q-learning target

    // Update Q-value for the action taken with TD update
    qValues[0][action] = currentQ + learningRate * (targetQ - currentQ);

    // Perform backward propagation to update weights
    (*policyNet).backward(state, qValues);
}


//Reinforce update training function
void Agent::trainPolicy(const vector<pair<Matrix, int>>& gameHistory, int outcome){
    double reward;
    
    if (outcome == 1) reward =1.0;
    else if (outcome == -1) reward = -1.0;
    else reward =0.0;

    for (size_t t = 0; t< gameHistory.size(); ++t){
        const auto& [state, action] =gameHistory[t];


        int player = (t % 2 == 0) ? 1: -1;
        double adjustedReward = player*reward;

        Matrix output = (*policyNet).forward(state);
        vector<double> probs = softmax(output[0]);

        probs[action] -= 1;
        for (size_t i =0; i<probs.size(); ++i){
            output[0][i] = adjustedReward * probs[i];

        }
        (*policyNet).backward(state, output);
    }
}

pair<vector<pair<Matrix,int>>,int> Agent::playGame(TicTacToe& game){
    vector<pair<Matrix, int>> gameHistory;

    int player =1;

    while(true){
        Matrix state = stateToMatrix(game.getState());
        vector<int> availableActions = game.getAvailableActions();


        vector<double> actionProbs = softmax((*policyNet).forward(state)[0]);

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
