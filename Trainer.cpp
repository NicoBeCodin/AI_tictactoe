#include <iostream>
#include "Trainer.h"

Trainer::Trainer(double gamma, double learningRate) : gamma(gamma), learningRate(learningRate) {};


void Trainer::trainPolicyGradient(Agent &agent, TicTacToe &game, int episodes)
{

    for (int episode = 0; episode < episodes; ++episode)
    {
        game.reset();
        auto [gameHistory, outcome] = agent.playGame(game);

        double reward = (outcome == 1) ? 1.0 : (outcome == -1) ? -1.0
                                                               : 0.0;

        agent.trainPolicy(gameHistory, reward);

        if (episode % 100 == 0)
        {
            cout << "Episode: " << episode << endl;
        }
    }

    cout << "Training finished..." << endl;
}

void Trainer::trainPolicyGradientTwo(Agent &agent1, Agent &agent2, TicTacToe &game, int episodes)
{
    for (int episode = 0; episode < episodes; ++episode)
    {
        game.reset();

        // Alternate which agent starts the game
        Agent *currentPlayer = (episode % 2 == 0) ? &agent1 : &agent2;
        Agent *opponent = (currentPlayer == &agent1) ? &agent2 : &agent1;

        // Play the game with both agents
        auto [gameHistory1, outcome1] = currentPlayer->playGameTwo(game, *opponent);
        auto [gameHistory2, outcome2] = opponent->playGameTwo(game, *currentPlayer);

        // Calculate rewards (zero-sum: 1 for win, -1 for loss)
        double reward1 = (outcome1 == 1) ? 1.0 : (outcome1 == -1) ? -1.0
                                                                  : 0.0;
        double reward2 = -reward1;

        // Train both agents
        currentPlayer->trainPolicy(gameHistory1, reward1);
        opponent->trainPolicy(gameHistory2, reward2);

        // Log progress
        if (episode % 100 == 0)
        {
            std::cout << "Episode: " << episode << std::endl;
        }
    }

    std::cout << "Training finished..." << std::endl;
}

void Trainer::playAgainstAI(NeuralNetwork &nn)
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

void Trainer::playAgainstMinimax()
{
    TicTacToe game;
    NeuralNetwork nn(9, 64, 9); //Dummy network
    Agent agent(&nn);
    std::cout << "Let's play! I am the minimax algorithm\n";
    int player = 1; // X
    game.printBoard();
    while (true)
    {
        if (player == 1)
        {
            // minimax turn
            int robotMove = agent.findBestMove(game, player);
            if (!game.makeMove(robotMove, player))
            {
                cout << "Invalid move!!!\n";
            }

            cout << "robot chooses position: " << robotMove << endl;
        }
        else
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

        game.printBoard();

        // Check for winner
        int result = game.checkWinner();
        if (result == -1)
        {
            cout << "You (X) won! Congrats!\n";
            break;
        }
        else if (result == 1)
        {
            cout << "minimax (O) won! Better luck next time.\n";
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
void Trainer::trainQAgent(QAgent& qAgent, int episodes, double alpha, double gamma, double epsilon_decay, double epsilon_min) {
    TicTacToe game;

    for (int episode = 0; episode < episodes; ++episode) {
        game.reset();
        
        int currentPlayer = 1; // Agent is Player 1
        bool gameOver = false;

        while (!gameOver) {
            // Get available actions
            std::vector<int> availableActions = game.getAvailableActions();


            // Select an action using epsilon-greedy
            int action = qAgent.selectAction(game);

            bool isIn = false;

            for (int act: availableActions){
                if (act==action){
                isIn = true;
                }
            }
            if (!isIn){
                cout<<"Trainer: illegal action: "<< action << endl;
                cout<<"Trainer: Printingboard \n";
                game.printBoard();
                cout<<"Trainer: availableActions: "<<endl;
                for (int a: availableActions){
                    cout<<a<<" ";
                }
                
            }
            
            // Save the current state and action
            std::vector<int> currentState = game.getState();
            

            // Apply the action
            if (!game.makeMove(action, currentPlayer)){
                cout<<"Trainer: Illegal move!!! \n";
               
            }
            
            vector<int> nextAvailableActions = game.getAvailableActions();
            vector<int> nextState = game.getState();
            
            // Check for game result
            int result = game.checkWinner(); // Replace with your existing function
            double reward = 0.0;
            
            if (result == 1) {
                reward = 1.0; // Win for the agent
                //std::cout<<"Win, reward = 1.0\n";
                qAgent.updateQValue(currentState, action, nextState, nextAvailableActions, reward, alpha, gamma, false);
                break;
            } else if (result == -1) {
                reward = -1.0; // Loss for the agent
                qAgent.updateQValue(currentState, action, nextState, nextAvailableActions, reward, alpha, gamma, false);
                //std::cout<<"loss, reward = -1.0\n";
                break;
            } else if (result == 0) {
                reward = 0.5; // Draw
                qAgent.updateQValue(currentState, action, nextState, nextAvailableActions, reward, alpha, gamma, false);
                //std::cout<<"draw, reward = 0.5\n";
                break;
            }
            else {
                reward =0.0;
                qAgent.updateQValue(currentState, action, nextState, nextAvailableActions, reward, alpha, gamma, false);

            }
            // If the game is not over, compute the next state


            // Update Q-values
            // Switch players
            currentPlayer *= -1;
        }

        // Decay epsilon to reduce exploration over time
        qAgent.decreaseEpsilon(epsilon_decay, epsilon_min);
        if (episode % 100 == 0){
            cout<<"Episode " << episode << endl;

        }
    }
}




void Trainer::playAgainstQAgent(QAgent &qAgent)
{
    TicTacToe game;
    

    std::cout << "Let's play! I am the qAgent\n";
    int player = 1; // X
    game.printBoard();
    while (true)
    {
        if (player == 1)
        {
            // minimax turn
            int agentMove = qAgent.selectAction(game);
            if (!game.makeMove(agentMove, player))
            {
                cout << "Invalid move!!!\n";
            }

            cout << "robot chooses position: " << agentMove << endl;
        }
        else
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

        game.printBoard();

        // Check for winner
        int result = game.checkWinner();
        if (result == 1)
        {
            cout << "QAgent (X) won! Better luck next time.\n";
            break;
        }
        else if (result == -1)
        {
            cout << "You (O) won! Congrats!\n";
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