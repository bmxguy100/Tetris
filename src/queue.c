#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include "queue.h"

tetrimino_tile_t queue[4];
tetrimino_tile_t asideTetrimino;

tetrimino_tile_t bag[7];
uint8_t bagIndex = 0;

void resetBag(){
  uint8_t i, j;

  bag[0] = I;
  bag[1] = O;
  bag[2] = T;
  bag[3] = S;
  bag[4] = Z;
  bag[5] = J;
  bag[6] = L;

  // Shuffle
  for(i = 0; i < 6; i++){
    tetrimino_tile_t tmp = bag[i];
    j = randInt(0, i);
    bag[i] = bag[j];
    bag[j] = tmp;
  }

  bagIndex = 0;
}

tetrimino_tile_t bagPop(){
  tetrimino_tile_t t;
  if(bagIndex == 7){
    resetBag();
  }
  t = bag[bagIndex];
  bagIndex++;
  return t;
}

void initializeQueue(){
  resetBag();
  queue[0] = bagPop();
  queue[1] = bagPop();
  queue[2] = bagPop();
  queue[3] = bagPop();

  asideTetrimino = _;
}

void nextQueueTetrimino(){
  queue[0] = queue[1];
  queue[1] = queue[2];
  queue[2] = queue[3];
  queue[3] = bagPop();
}