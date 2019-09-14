// http://tetris.wikia.com/wiki/Tetris_Guideline

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#define _ 0
#define I 1
#define O 2
#define T 3
#define S 4
#define Z 5
#define J 6
#define L 7
#define outline 8
typedef char tetrimino_tile_t;

#include "srs.h"

extern uint24_t frameCount;
extern tetrimino_tile_t board[400];

extern uint24_t score;
extern uint24_t linesCleared;
extern uint24_t level;

tetrimino_tile_t tetriminoLUT(tetrimino_tile_t tetrimino, int8_t tetriminoX, int8_t tetriminoY, int8_t x, int8_t y, int8_t rotation);
tetrimino_tile_t getTileAt(int8_t x, int8_t y);
void calculateOutline();
bool inBoundingBox(tetrimino_tile_t tetrimino, int8_t tetriminoX, int8_t tetriminoY, int8_t x, int8_t y);
bool testPiecePosition(tetrimino_tile_t tetrimino, int8_t x, int8_t y, int8_t rotation);
bool tryMove(int8_t x, int8_t y, int8_t rotationDir);
bool tryRotate(int8_t dir);
void resetTetriminoPostion();
void lockTetriminoPosiontion();

#endif