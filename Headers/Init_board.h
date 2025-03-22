#ifndef INIT_BOARD_H 
#define INIT_BOARD_H
#include "Declare.h"

//Khai bao bang
void InitBoard() {
    int pos = 0;
    std::vector<int> tiles;
    for (int i = 0; i < (ROWS * COLS) / 2; ++i) {
        tiles.push_back(i);
        tiles.push_back(i);
        save_color[i] = pos;
        if(i % 2 == 1) pos++;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(tiles.begin(), tiles.end(), g);

    board.resize(ROWS, std::vector<int>(COLS));
    save_board.resize(ROWS, std::vector<int>(COLS));
    for (int i = 0, k = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j, ++k) {
            board[i][j] = tiles[k];
            save_board[i][j] = tiles[k];
        }
    }
}

#endif