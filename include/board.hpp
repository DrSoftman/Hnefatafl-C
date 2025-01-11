
#ifndef BOARD_HPP
#define BOARD_HPP

#define EMPTY 0
#define KING 4
#define ATTACKER 1
#define DEFENDER 2

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
      // TODO: Set coordinates of pieces

    }

};

#endif
