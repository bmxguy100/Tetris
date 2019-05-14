// http://tetris.wikia.com/wiki/Tetris_Guideline

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

enum Tetrimino_Tile {
    _,
    I,
    O,
    T,
    S,
    Z,
    J,
    L,
    outline
};
typedef enum Tetrimino_Tile tetrimino_tile_t;

#include "srs.h"

extern uint24_t frameCount;
extern tetrimino_tile_t board[400];

tetrimino_tile_t tetriminoLUT(tetrimino_tile_t tetrimino, int8_t tetriminoX, int8_t tetriminoY, int8_t x, int8_t y, int8_t rotation);
tetrimino_tile_t getTileAt(int8_t x, int8_t y);
bool inBoundingBox(tetrimino_tile_t tetrimino, int8_t tetriminoX, int8_t tetriminoY, int8_t x, int8_t y);
bool tryMove(int8_t x, int8_t y, int8_t rotationDir);
bool tryRotate(int8_t dir);
void resetTetriminoPostion();
void lockTetriminoPosiontion();

#endif