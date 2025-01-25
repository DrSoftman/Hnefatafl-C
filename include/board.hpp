
#ifndef BOARD_HPP
#define BOARD_HPP

#define EMPTY 0
#define ATTACKER 1
#define DEFENDER 2
#define KING 3

#include <iostream>

class Board {

  private:
    int grid[11][11];

  public:

    void init_grid() {
      for (int i = 0; i <= 11; i++) {
        for (int j = 0; j <= 11; j++) {
          grid[i][j] = EMPTY;
        }
      }
      // King
      grid[5][5] = KING;

      // White defenders
      grid[5][3] = DEFENDER;
      grid[4][4] = DEFENDER;
      grid[6][4] = DEFENDER;
      grid[3][5] = DEFENDER; 
      grid[7][5] = DEFENDER; 
      grid[4][6] = DEFENDER;
      grid[6][6] = DEFENDER; 
      grid[5][7] = DEFENDER;
      grid[5][0] = DEFENDER;
      grid[5][10] = DEFENDER;
      grid[0][5] = DEFENDER;
      grid[10][5] = DEFENDER;

      // Black attackers
      grid[3][0] = ATTACKER; 
      grid[4][0] = ATTACKER;
      grid[5][0] = ATTACKER;
      grid[6][0] = ATTACKER;
      grid[7][0] = ATTACKER;
      grid[3][10] = ATTACKER; 
      grid[4][10] = ATTACKER;
      grid[5][10] = ATTACKER;
      grid[6][10] = ATTACKER;
      grid[7][10] = ATTACKER;
      grid[0][3] = ATTACKER;
      grid[0][4] = ATTACKER;
      grid[0][5] = ATTACKER;
      grid[0][6] = ATTACKER;
      grid[0][7] = ATTACKER;
      grid[10][3] = ATTACKER;   
      grid[10][4] = ATTACKER;
      grid[10][5] = ATTACKER;
      grid[10][6] = ATTACKER;
      grid[10][7] = ATTACKER;
    }

};

#endif
