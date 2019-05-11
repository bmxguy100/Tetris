#ifdef QUEUE_H
#error QUEUE_H redefined?!?!
#endif

#ifndef QUEUE_H
#define QUEUE_H

#include "game.h"

extern tetrimino_tile_t queue[3];
extern tetrimino_tile_t asideTetrimino;

void initializeQueue();

#endif