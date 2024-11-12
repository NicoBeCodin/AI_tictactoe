#ifndef GAME_H
#define GAME_H

#include <vector>
using namespace std;

class TicTacToe {
public:
    TicTacToe();
    void reset();
    vector<int> getAvailableActions() const;
    bool makeMove(int pos, int player);
    int checkWinner() const;
    vector<int> getState() const;
    void printBoard() const;

private:
    vector<int> board; // 3x3 Tic-Tac-Toe board represented as a 1D array of size 9
};
#endif