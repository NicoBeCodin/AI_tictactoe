#include <vector>
#include <iostream>
#include "Game.h"
using namespace std;
typedef vector<vector<double>> Matrix;

TicTacToe::TicTacToe()
{
    board = vector<int>(9, 0); // Initialize empty board (0 = empty, 1 = X, -1 = O)
}

// Print the current board state (optional, for debugging)
void TicTacToe::printBoard() const
{
    for (int i = 0; i < 9; ++i)
    {
        if (i % 3 == 0)
            cout << "\n";
        if (board[i] == 1)
            cout << "X ";
        else if (board[i] == -1)
            cout << "O ";
        else
            cout << "- ";
    }
    cout << "\n";
}

// Get available actions (empty spots on the board)
vector<int> TicTacToe::getAvailableActions() const
{
    vector<int> actions;
    for (int i = 0; i < 9; ++i)
        if (board[i] == 0)
            actions.push_back(i);
    return actions;
}

vector<int> TicTacToe::getAvailableActionsState(vector<int>& board_state) const
{
    vector<int> actions;
    for (int i = 0; i < 9; ++i)
        if (board_state[i] == 0)
            actions.push_back(i);
    return actions;
}


// Make a move on the board
bool TicTacToe::makeMove(int pos, int player)
{
    if (board[pos] == 0)
    {
        board[pos] = player;
        return true;
    }
    return false;
}
bool TicTacToe::revertMove(int pos){
    if (board[pos] !=0) {
        board[pos]=0;
        return true;
    }
    std::cout<< "Can't revert move!!\n";
    return false;
}

// Check if the game has ended and return the winner (1 = X, -1 = O, 0 = draw, -2 = not ended)
int TicTacToe::checkWinner() const
{
    // Check rows, columns, and diagonals
    for (int i = 0; i < 3; i++)
    {
        if (board[i * 3] != 0 && board[i * 3] == board[i * 3 + 1] && board[i * 3] == board[i * 3 + 2])
            return board[i * 3];
        if (board[i] != 0 && board[i] == board[i + 3] && board[i] == board[i + 6])
            return board[i];
    }
    if (board[0] != 0 && board[0] == board[4] && board[0] == board[8])
        return board[0];
    if (board[2] != 0 && board[2] == board[4] && board[2] == board[6])
        return board[2];

    // Check if nobody has won
    for (int i = 0; i < 9; i++)
        if (board[i] == 0)
            return -2; // Game is not finished

    return 0; // Draw
}

int TicTacToe::checkTwoInARow(int player, vector<int> board) {
    // Define the winning lines on a 3x3 board
    std::vector<std::vector<int>> winningLines = {
        {0, 1, 2}, // Row 1
        {3, 4, 5}, // Row 2
        {6, 7, 8}, // Row 3
        {0, 3, 6}, // Column 1
        {1, 4, 7}, // Column 2
        {2, 5, 8}, // Column 3
        {0, 4, 8}, // Diagonal 1
        {2, 4, 6}  // Diagonal 2
    };
    
    // Check each line to see if there is a "2 in a row" situation
    for (const auto& line : winningLines) {
        int countPlayer = 0;
        int emptyIndex = -1;
        
        for (int index : line) {
            if (board[index] == player) {
                countPlayer++;
            } else if (board[index] == 0) {
                emptyIndex = index;
            }
        }
        
        // If there are exactly two of the player's marks and one empty cell, return the empty index
        if (countPlayer == 2 && emptyIndex != -1) {
            return emptyIndex;
        }
    }
    
    // Return -1 if no "2 in a row" situation is found
    return -1;
}

// Reset the board
void TicTacToe::reset()
{
    board = vector<int>(9, 0);
}

vector<int> TicTacToe::getState() const
{
    return board; // Return the current board state
}
