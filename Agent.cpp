#include "Agent.h"

Agent::Agent(NeuralNetwork *policyNet, double epsilon) : policyNet(policyNet), epsilon(epsilon) {};

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

// Reinforce update training function
void Agent::trainPolicy_deprecated(const vector<pair<Matrix, int>> &gameHistory, double outcome)
{
    double reward;

    if (outcome == 1)
        reward = 1.0;
    else if (outcome == -1)
        reward = -1.0;
    else
        reward = 0.0;

    // Go through the whole game
    for (size_t t = 0; t < gameHistory.size(); ++t)
    {
        const auto &[state, action] = gameHistory[t];

        int player = (t % 2 == 0) ? 1 : -1;
        double adjustedReward = player * reward;

        Matrix output = (*policyNet).forward(state);
        vector<double> probs = softmax(output[0]);

        // Create the target output
        // Game to get the methods
        TicTacToe game;
        Matrix targetOutput(1, vector<double>(9));
        // Check if I could win at this turn
        vector<int> vector_state = matrixToState(state);
        int winningMove = game.checkTwoInARow(player, vector_state);
        int blockingMove = game.checkTwoInARow(-player, vector_state);
        if (winningMove != -1)
        {

            targetOutput[0][winningMove] = 1.0;
        }
        // Check if I can block a 2 in a row
        else if (blockingMove != -1)
        {

            targetOutput[0][blockingMove] = 1.0;
        }
        else
        {
            vector<int> availableMoves = game.getAvailableActionsState(vector_state);
            for (auto &i : availableMoves)
            {
                targetOutput[0][i] = output[0][i];
            }
            double sum = accumulate(targetOutput[0].begin(), targetOutput[0].end(), 0.0);
            for (auto j : targetOutput[0])
            {
                j /= sum;
            }
        }
        // set zero to all unavailable plays
        // Adjust probs

        // Why ?
        probs[action] -= 1;
        for (size_t i = 0; i < probs.size(); ++i)
        {
            output[0][i] = adjustedReward * probs[i];
        }

        (*policyNet).backward(state, targetOutput);
    }
}

void Agent::trainPolicy(const vector<pair<Matrix, int>> &gameHistory, double outcome)
{
    double cumulativeReward = 0.0;
    double discountFactor = 0.99; // Discount factor for future rewards

    // Step 1: Compute cumulative rewards for the game
    vector<double> rewards(gameHistory.size());
    for (int t = gameHistory.size() - 1; t >= 0; --t)
    {
        cumulativeReward = outcome + discountFactor * cumulativeReward;
        rewards[t] = cumulativeReward;
    }

    // Step 2: Loop through game history and update the policy network
    for (size_t t = 0; t < gameHistory.size(); ++t)
    {
        const auto &[state, action] = gameHistory[t];

        // Forward pass: Get the action probabilities
        Matrix output = (*policyNet).forward(state);
        vector<double> probs = softmax(output[0]);

        // Adjust probabilities to compute the gradient-like "target"
        vector<double> target(9, 0.0); // Assuming 9 possible actions in Tic Tac Toe
        for (size_t i = 0; i < target.size(); ++i)
        {
            target[i] = probs[i]; // Start with the predicted probabilities
        }
        target[action] -= 1.0; // Gradient adjustment for the chosen action

        // Scale the adjustment by the cumulative reward (policy gradient rule)
        for (size_t i = 0; i < target.size(); ++i)
        {
            target[i] *= rewards[t];
        }

        // Convert target to a Matrix type for backward function
        Matrix targetMatrix(1, target);

        // Backward pass with the state (input) and adjusted t arget
        (*policyNet).backward(state, targetMatrix);
    }
}

pair<vector<pair<Matrix, int>>, int> Agent::playGame(TicTacToe &game)
{
    vector<pair<Matrix, int>> gameHistory;

    int player = 1;

    while (true)
    {
        Matrix state = stateToMatrix(game.getState());
        int action = chooseMove(game);

        game.makeMove(action, player);
        gameHistory.push_back({state, action});

        int result = game.checkWinner();
        if (result == 1)
            return {gameHistory, player};
        else if (result == -1)
            return {gameHistory, -player};
        else if (result == 0)
            return {gameHistory, 0};

        player *= -1;
    }
}


int Agent::chooseMove(TicTacToe &game)
{
    Matrix state = stateToMatrix(game.getState());
    vector<int> availableActions = game.getAvailableActions();

    vector<double> actionProbs = softmax((*policyNet).forward(state)[0]);

    vector<double> availableActionProbs(9, 0.0);
    for (int action : availableActions)
    {
        availableActionProbs[action] = actionProbs[action];
    }

    double sumProbs = accumulate(availableActionProbs.begin(), availableActionProbs.end(), 0.0);
    for (double &prob : actionProbs)
    {
        prob /= sumProbs;
    }
    int action;
    if (rand() / RAND_MAX < epsilon)
    {
        action = randomAction(availableActions);
    }
    else
    {
        action = sampleAction(availableActionProbs);
    }
    return action;
}

pair<vector<pair<Matrix, int>>, int> Agent::playGameTwo(TicTacToe &game, Agent &opponent)
{
    vector<pair<Matrix, int>> gameHistory;

    int player = 1; // Current player (1 or -1)
    while (true)
    {
        Matrix state = stateToMatrix(game.getState());
        vector<int> availableActions = game.getAvailableActions();

        int action;
        if (player == 1)
        {
            // Current agent's turn
            vector<double> actionProbs = softmax((*policyNet).forward(state)[0]);
            vector<double> availableActionProbs(9, 0.0);
            for (int act : availableActions)
            {
                availableActionProbs[act] = actionProbs[act];
            }
            action = sampleAction(availableActionProbs);
        }
        else
        {
            // Opponent's turn
            action = opponent.chooseMove(game);
        }

        game.makeMove(action, player);
        gameHistory.push_back({state, action});

        int result = game.checkWinner();
        if (result == 1)
            return {gameHistory, player};
        else if (result == -1)
            return {gameHistory, -player};
        else if (result == 0)
            return {gameHistory, 0};

        player *= -1;
    }
}

int Agent::minimax(TicTacToe &game, int depth, bool isMaximizing){
    int winner = game.checkWinner();

    //Could adjust to value quicker winning moves
    if (winner == 1 || winner==-1){
        return winner *10;
    }
    else if (winner==0){
        //DRAW
        return 0;
    }

    if (isMaximizing) {
        int best =std::numeric_limits<int>::min();
        for (int i=0; i<9;++i){
            //Could modify to only go through available states
            if (game.getState()[i]== 0){
                game.makeMove(i, 1);
                best = std::max(best, minimax(game, depth+1,false));
                game.revertMove(i);
                
            }

        }
        return best;

    }else {
        int best = std::numeric_limits<int>::max();
        for (int i =0; i<9;++i){
            if (game.getState()[i]==0){
                game.makeMove(i, -1);
                best = std::min(best, minimax(game, depth+1, true));
                game.revertMove(i);
            }
        }
        return best;
    }
}

int Agent::findBestMove(TicTacToe& game, int player){
    int bestValue= (player== 1) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    int bestMove =-1;
    
    for (int i=0;i<9;++i){
        if (game.getState()[i]==0){
            game.makeMove(i, player);
            int moveValue = minimax(game, 0, false);
            game.revertMove(i);

            if (moveValue > bestValue ) {
                bestValue =moveValue;
                bestMove =i;
            }
        }
    }
    return bestMove;
}