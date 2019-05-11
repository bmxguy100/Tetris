// http://tetris.wikia.com/wiki/Tetris_Guideline

#ifndef GAME_H
#define GAME_H

enum Tetrimino_Tile {
    none,
    I,
    O,
    T,
    S,
    Z,
    J,
    L
};
typedef enum Tetrimino_Tile tetrimino_tile_t;

extern tetrimino_tile_t board[400];

#endif