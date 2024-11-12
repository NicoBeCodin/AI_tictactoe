#include <vector>
#include <iostream>
#include "Game.h"
using namespace std;

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

    // Check if it's a draw
    for (int i = 0; i < 9; i++)
        if (board[i] == 0)
            return -2; // Game is not finished

    return 0; // Draw
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
