
#ifndef BOARD_H
#define BOARD_H

#define EMPTY 0
#define ATTACKER 1
#define DEFENDER 2
#define KING 3

#include <stdio.h>

void initBoard(char board[11][11]) {
    // Init all offsets in board to 0
    for (int i = 0; i <= 11; i++) {
        for (int j = 0; j <= 11; j++) {
            board[i][j] = EMPTY;
        }
    }
  
    board[3][0] = ATTACKER;
    board[4][0] = ATTACKER;
    board[5][0] = ATTACKER; 
    board[6][0] = ATTACKER; 
    board[7][0] = ATTACKER; 
    board[3][10] = ATTACKER; 
    board[4][10] = ATTACKER;
    board[5][10] = ATTACKER;
    board[6][10] = ATTACKER;
    board[7][10] = ATTACKER;
    board[0][3] = ATTACKER; 
    board[0][4] = ATTACKER; 
    board[0][5] = ATTACKER; 
    board[0][6] = ATTACKER; 
    board[0][7] = ATTACKER; 
    board[10][3] = ATTACKER; 
    board[10][4] = ATTACKER; 
    board[10][5] = ATTACKER; 
    board[10][6] = ATTACKER;
    board[10][7] = ATTACKER;

    board[5][3] = DEFENDER;
    board[4][4] = DEFENDER; 
    board[6][4] = DEFENDER;
    board[3][5] = DEFENDER;
    board[7][5] = DEFENDER;
    board[4][6] = DEFENDER;
    board[6][6] = DEFENDER;
    board[5][7] = DEFENDER;
    board[5][0] = DEFENDER;
    board[5][10] = DEFENDER;
    board[0][5] = DEFENDER;
    board[10][5] = DEFENDER;

    board[5][5] = KING;
}

#endif // BOARD_H
