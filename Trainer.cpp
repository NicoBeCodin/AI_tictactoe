#include <iostream>
#include "Trainer.h"


Trainer::Trainer(double gamma, double learningRate):gamma(gamma), learningRate(learningRate){};

//Deprecated method of qValues,


// NeuralNetwork trainQValues(Agent& agent, Game& game)
// {
//     srand(time(0)); 

//     // Hyperparameters
//     int numEpisodes = 20000;
//     double learningRate = 0.01;
//     double gamma = 0.9; // Discount factor
//     double epsilon = 1.0;
//     double epsilonDecay = 0.9995;
//     double minEpsilon = 0.1;

//     // Initialize Tic-Tac-Toe game and neural network
    
//     // 9 input nodes (board state), 64 hidden nodes, 9 output nodes (Q-values)

//     for (int episode = 0; episode < numEpisodes; ++episode)
//     {
//         game.reset();
//         vector<int> board = game.getState();
//         int player = 1; // X starts

//         // Play an episode
//         while (true)
//         {
//             Matrix state = stateToMatrix(game.getState());
//             // printMatrix(state, "state ");
//             vector<int> availableActions = game.getAvailableActions();
//             int action = chooseAction(nn, state, epsilon, availableActions);

//             game.makeMove(action, player);
//             int result = game.checkWinner();
//             vector<int> nextBoard = game.getState();
//             Matrix nextState = stateToMatrix(nextBoard);

//             // Calculate reward
//             double reward = -0.05;
//             if (result == 1)
//             {
//                 reward = (double)player; // X wins
//             }
//             else if (result == -1)
//             {
//                 reward = -(double)player; // O wins
//             }
//             else if (result == 0)
//             {
//                 reward = 0.5; // Draw
//             }
//             // Small negative reward to incentivize winning early

//             // Update Q-values
//             agent.updateQValues(state, action, reward, nextState, gamma, learningRate);

//             // Switch player
//             player *= -1;

//             // End game if finished
//             if (result != -2)
//             {
//                 break;
//             }
//         }

//         // Decay epsilon
//         epsilon = max(minEpsilon, epsilon * epsilonDecay);

//         if (episode % 100 == 0)
//         {
//             cout << "Episode: " << episode << " - Epsilon: " << epsilon << endl;
//         }
//     }

//     return nn;
// }

void Trainer::trainPolicyGradient(Agent& agent, TicTacToe& game, int episodes){
    

    for (int episode = 0; episode<episodes; ++episode ){
        game.reset();
        auto [gameHistory, outcome] = agent.playGame(game);

        double reward = (outcome ==1) ? 1.0 : (outcome==-1) ? -1.0: 0.0;

        agent.trainPolicy(gameHistory, reward);

        if (episode % 100 == 0){
            cout << "Episode: "<< episode<< endl;
        }
    }
    
    cout<<"Training finished..."<<endl;
    
}

void Trainer::trainPolicyGradientTwo(Agent& agent1, Agent& agent2, TicTacToe& game, int episodes) {
    for (int episode = 0; episode < episodes; ++episode) {
        game.reset();

        // Alternate which agent starts the game
        Agent* currentPlayer = (episode % 2 == 0) ? &agent1 : &agent2;
        Agent* opponent = (currentPlayer == &agent1) ? &agent2 : &agent1;

        // Play the game with both agents
        auto [gameHistory1, outcome1] = currentPlayer->playGameTwo(game, *opponent);
        auto [gameHistory2, outcome2] = opponent->playGameTwo(game, *currentPlayer);

        // Calculate rewards (zero-sum: 1 for win, -1 for loss)
        double reward1 = (outcome1 == 1) ? 1.0 : (outcome1 == -1) ? -1.0 : 0.0;
        double reward2 = -reward1;

        // Train both agents
        currentPlayer->trainPolicy(gameHistory1, reward1);
        opponent->trainPolicy(gameHistory2, reward2);

        // Log progress
        if (episode % 100 == 0) {
            std::cout << "Episode: " << episode << std::endl;
        }
    }

    std::cout << "Training finished..." << std::endl;
}


void Trainer::playAgainstAI(NeuralNetwork& nn)
{
    TicTacToe game;
    int player = 1; // You are 'X'
    Agent agent(&nn);
    
    
    game.reset();
    cout << "Let's play Tic-Tac-Toe! You are 'X'.\n";
    game.printBoard();

    while (true)
    {
        if (player == 1)
        {
            int move;
            cout << "Your turn! Enter a move (0-8): ";
            cin >> move;

            // Ensure the move is valid
            while (move < 0 || move > 8 || !game.makeMove(move, player))
            {
                cout << "Invalid move! Try again (0-8): ";
                cin >> move;
            }
        }
        else
        {
            // AI's turn
            vector<int> boardState = game.getState();
            Matrix state = stateToMatrix(boardState);
            vector<int> availableActions = game.getAvailableActions();

            // Use the neural network to choose the action
            int aiMove = agent.chooseAction(state, availableActions); // epsilon = 0 to use greedy action
            game.makeMove(aiMove, player);
            cout << "AI chooses position: " << aiMove << endl;
        }

        game.printBoard();

        // Check for winner
        int result = game.checkWinner();
        if (result == 1)
        {
            cout << "You (X) won! Congrats!\n";
            break;
        }
        else if (result == -1)
        {
            cout << "AI (O) won! Better luck next time.\n";
            break;
        }
        else if (result == 0)
        {
            cout << "It's a draw!\n";
            break;
        }

        // Switch player
        player *= -1;
    }
}

void Trainer::playAgainstMinimax(){
    TicTacToe game;
    NeuralNetwork nn(9,64,9);
    Agent agent(&nn);
    std::cout<<"Let's play! I am the minimax algorithm\n";
    int who_started = 1; //X
    int player =1; //X
    game.printBoard();
    while (true)
    {
        if (player == -1)
        {
            int move;
            cout << "Your turn! Enter a move (0-8): ";
            cin >> move;

            // Ensure the move is valid
            while (move < 0 || move > 8 || !game.makeMove(move, player))
            {
                cout << "Invalid move! Try again (0-8): ";
                cin >> move;
            }
        }
        else
        {
            // minimax turn
            int robotMove = agent.findBestMove(game, player);
            if (!game.makeMove(robotMove, player)){
                cout<<"invalid move!!!\n";
            }

            cout << "robot chooses position: " << robotMove << endl;
        }

        game.printBoard();

        // Check for winner
        int result = game.checkWinner();
        if (result == 1)
        {
            cout << "You (X) won! Congrats!\n";
            break;
        }
        else if (result == -1)
        {
            cout << "AI (O) won! Better luck next time.\n";
            break;
        }
        else if (result == 0)
        {
            cout << "It's a draw!\n";
            break;
        }

        // Switch player
        player *= -1;
    }

}
