#ifndef QUEUE_H
#define QUEUE_H

#include "game.h"

extern tetrimino_tile_t queue[4];
extern tetrimino_tile_t asideTetrimino;

void initializeQueue();
void nextQueueTetrimino();
void storeTetrimino();

#endif