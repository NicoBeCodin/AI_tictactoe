#include "Agent.h"
#include <iostream>
#include <limits>

vector<double> QAgent::initializeQValues() {
    return vector<double>(9, 0.0); // Initialize Q-values for all 9 actions to 0
}

QAgent::QAgent(double epsilon, map<pair<vector<int>, int>, double> qTable): epsilon(epsilon), qTable(qTable) {} ;


int QAgent::selectAction(TicTacToe &game) {
    vector<int> availableActions = game.getAvailableActions();
    if (availableActions.empty()){
        cerr<<"No available actions for this state:"<<endl;
        game.printBoard();
        return -1;
    }

    if (((double) rand() / RAND_MAX) < epsilon) {
        // Exploration: choose a random action
        return availableActions[rand() % availableActions.size()];
    }

    // Exploitation: choose the action with the highest Q-value
    int bestAction = availableActions[0];
    double maxQ = -std::numeric_limits<double>::infinity();
    vector<int> state = game.getState();
    for (int action : availableActions) {
        auto key = std::make_pair(state, action);
        double qValue = getQValue(key);
        if (qValue > maxQ) {
            maxQ = qValue;
            bestAction = action;
        }
    }
    cout<<endl;
    return bestAction;
}

void QAgent::decreaseEpsilon(double coeff, double lowest){
    if (epsilon>= lowest){
        
    epsilon*=coeff;
    }
}

//Only when debugging
double QAgent::getQValue(std::pair<vector<int>, int> key){
    auto it = qTable.find(key);
    if (it != qTable.end()) {
        // Key exists, return the associated value
        return it->second;
    } else {
        // Key does not exist, return 0
        return 0.0;
    }
}

// void QAgent::updateQValue(vector<int>& state, int action, vector<int>& nextState, vector<int>& nextAvailableActions ,double reward, double alpha, double gamma){
//     double nextMaxQ = -std::numeric_limits<double>::infinity();
//     for (int a: nextAvailableActions){
        
//         nextMaxQ = std::max(getQValue(std::make_pair(nextState, a)), nextMaxQ);
//     }
//     if (nextMaxQ == -std::numeric_limits<double>::infinity()){
//         cout<<"***nextMaxQ is -inf"<<endl;
//         cout<<"getQValue result is: "<<endl;
//         for (int s: nextAvailableActions){
//             cout<<"action :" << s << "getQValue: " <<getQValue(std::make_pair(nextState, s))<<endl;
//         }
//     }
//     std::pair<vector<int>, int> qKey = std::make_pair(state, action);
//     double currentQValue = getQValue(qKey);
//     double newQValue = currentQValue + alpha  * (reward + gamma * nextMaxQ - currentQValue);
//     // std::cout<<"New Q Value: " << newQValue << std::endl;
//     if (newQValue == -std::numeric_limits<double>::infinity()){
//         cout<<"-inf assigned to: " <<endl;
//         for(int s: state){
//             cout<<s<<endl;
//         }
//         cout<<"action is : "<< action<<endl;
//     }
//     qTable[qKey] = newQValue;

// }

void QAgent::updateQValue(std::vector<int>& state, int action, std::vector<int>& nextState,
                          std::vector<int>& nextAvailableActions, double reward, double alpha, double gamma) {
    if (nextAvailableActions.empty()) {
        std::cerr << "Error: No available actions for next state!" << std::endl;
        return;
    }

    double nextMaxQ = -std::numeric_limits<double>::infinity();
    for (int a : nextAvailableActions) {
        double qValue = getQValue(std::make_pair(nextState, a));
        nextMaxQ = std::max(nextMaxQ, qValue);
    }

    if (nextMaxQ == -std::numeric_limits<double>::infinity()) {
        std::cerr << "Error: nextMaxQ is -inf! Check available actions and state validity." << std::endl;
        return;
    }

    std::pair<std::vector<int>, int> qKey = std::make_pair(state, action);
    double currentQValue = getQValue(qKey);
    double newQValue = currentQValue + alpha * (reward + gamma * nextMaxQ - currentQValue);

    // Log the Q-value update
    std::cout << "Updating Q-value for State: [";
    for (int val : state) std::cout << val << " ";
    std::cout << "], Action: " << action << ", Reward: " << reward
              << ", Old Q-Value: " << currentQValue << ", New Q-Value: " << newQValue << std::endl;

    qTable[qKey] = newQValue;
}



void QAgent::printFirstFiveQTable() {
    int count = 0;
    for (const auto& entry : qTable) {
        // Print the key (state-action pair) and value (Q-value)
        const auto& state = entry.first.first; // The vector<int> (state)
        int action = entry.first.second;       // The int (action)
        double qValue = entry.second;          // The double (Q-value)

        // Print the state
        std::cout << "State: [";
        for (size_t i = 0; i < state.size(); ++i) {
            std::cout << state[i];
            if (i < state.size() - 1) std::cout << ", ";
        }
        std::cout << "], Action: " << action << ", Q-Value: " << qValue << std::endl;

        // Stop after printing 5 entries
        if (++count >= 5) break;
    }

    if (count == 0) {
        std::cout << "The Q-table is empty!" << std::endl;
    }
}


void QAgent::saveQTable(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::out);
    if (!outFile) {
        std::cerr << "Error: Could not open file for saving Q-Table." << std::endl;
        return;
    }

    for (const auto& entry : qTable) {
        const auto& state = entry.first.first;
        int action = entry.first.second;
        double qValue = entry.second;

        // Write the state, action, and Q-value
        for (int val : state) {
            outFile << val << " ";
        }
        outFile << action << " " << qValue << std::endl;
    }

    outFile.close();
    std::cout << "Q-Table saved to " << filename << std::endl;
}

void QAgent::loadQTable(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::in);
    if (!inFile) {
        std::cerr << "Error: Could not open file for loading Q-Table." << std::endl;
        return;
    }

    qTable.clear(); // Clear the current Q-table before loading

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);

        // Read the state
        std::vector<int> state(9);
        for (int i = 0; i < 9; ++i) {
            iss >> state[i];
        }

        // Read the action and Q-value
        int action;
        double qValue;
        iss >> action >> qValue;

        // Insert into the Q-table
        qTable[{state, action}] = qValue;
    }

    inFile.close();
    std::cout << "Q-Table loaded from " << filename << std::endl;
}





//Q Learning <ith neural network (maybe too complex for task)


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
    if ((double)rand() / (double)RAND_MAX < epsilon)
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