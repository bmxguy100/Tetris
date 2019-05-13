#include "game.h"
#include "queue.h"
#include "srs.h"

uint24_t frameCount;
tetrimino_tile_t board[400];

int8_t currentPieceX = 0;
int8_t currentPieceY = 0;
int8_t rotation = 0;

tetrimino_tile_t tetriminoLUTUnclipped(tetrimino_tile_t tetrimino, int8_t x, int8_t y, int8_t rotation){
  switch (tetrimino){
      case I:
        return I_ROTATION_LUT[rotation][y][x];
      case T:
        return T_ROTATION_LUT[rotation][y][x];
      case S:
        return S_ROTATION_LUT[rotation][y][x];
      case Z:
        return Z_ROTATION_LUT[rotation][y][x];
      case J:
        return J_ROTATION_LUT[rotation][y][x];
      case L:
        return L_ROTATION_LUT[rotation][y][x];
      case O:
        return O;
      break;
    }
}

tetrimino_tile_t tetriminoLUT(tetrimino_tile_t tetrimino, int8_t tetriminoX, int8_t tetriminoY, int8_t x, int8_t y, int8_t rotation){
  if(inBoundingBox(tetrimino, tetriminoX, tetriminoY, x, y)){
    int8_t localX = x - tetriminoX;
    int8_t localY = y - tetriminoY;
    return tetriminoLUTUnclipped(tetrimino, localX, localY, rotation);
  }
  return _;
}

tetrimino_tile_t boardLookUp(int8_t x, int8_t y){
  if(x < 0 || x >= 10 || y < 0 || y >= 40) return O;
  return board[x + y * 10];
}

tetrimino_tile_t getTileAt(int8_t x, int8_t y){
  tetrimino_tile_t a = boardLookUp(x, y);
  if(a != _) return a;

  a = tetriminoLUT(queue[0], currentPieceX, currentPieceY, x, y, rotation);
  if(a != _) return a;

  // TODO: Get Outline y-postion
  // a = tetriminoLUT(queue[0], currentPieceX, 38, x, y, rotation);
  // if(a != _) return outline;

  return _;
}

bool inBoundingBox(tetrimino_tile_t tetrimino, int8_t tetriminoX, int8_t tetriminoY, int8_t x, int8_t y){
  if(tetrimino == O){
    return (
      (x >= tetriminoX) &&
      (x <= tetriminoX + 1) &&
      (y >= tetriminoY) &&
      (y <= tetriminoY + 1));
  }
  if(tetrimino == I){
    return (
      (x >= tetriminoX) &&
      (x <= tetriminoX + 3) &&
      (y >= tetriminoY) &&
      (y <= tetriminoY + 3));
  }
  return (
    (x >= tetriminoX) &&
    (x <= tetriminoX + 2) &&
    (y >= tetriminoY) &&
    (y <= tetriminoY + 2));
}

bool tryMove(int8_t x, int8_t y, int8_t rotationDir){
  int8_t newX = currentPieceX + x;
  int8_t newY = currentPieceY + y;
  int8_t newRotation = (rotation + rotationDir + 4) % 4;

  if(queue[0] == O){
    bool canMove =
     (boardLookUp(newX,   newY  ) == _) &&
     (boardLookUp(newX+1, newY  ) == _) &&
     (boardLookUp(newX,   newY+1) == _) &&
     (boardLookUp(newX+1, newY+1) == _);
    
    if(canMove){
      currentPieceX = newX;
      currentPieceY = newY;
      return true;
    }
    return false;
  }else if(queue[0] == I){
    uint8_t xi;
    uint8_t yi;
    for (xi = 0; xi < 4; xi++){
      for (yi = 0; yi < 4; yi++){
        if(tetriminoLUTUnclipped(queue[0], xi, yi, newRotation) != _){
          if(boardLookUp(newX + xi, newY + yi) != _){
            return false;
          }
        }
      }
    }
    currentPieceX = newX;
    currentPieceY = newY;
    rotation = newRotation;
    return true;
  }else{
    uint8_t xi;
    uint8_t yi;
    for (xi = 0; xi < 3; xi++){
      for (yi = 0; yi < 3; yi++){
        if(tetriminoLUTUnclipped(queue[0], xi, yi, newRotation) != _){
          if(boardLookUp(newX + xi, newY + yi) != _){
            return false;
          }
        }
      }
    }
    currentPieceX = newX;
    currentPieceY = newY;
    rotation = newRotation;
    return true;
  }
}

bool tryRotate(int8_t dir){
  int8_t attempt;
  int8_t newRotation = (rotation + dir + 4) % 4;

  if(queue[0] == O) return true;

  for (attempt = 0; attempt < 5; attempt++){
    if(queue[0] == I){
      if(tryMove(
        I_WALL_KICK_LUT[rotation][newRotation][attempt][0],
        I_WALL_KICK_LUT[rotation][newRotation][attempt][1],
        dir)
      ){
        return true;
      }
    } else {
      if(tryMove(
        JLSTZ_WALL_KICK_LUT[rotation][newRotation][attempt][0], 
        JLSTZ_WALL_KICK_LUT[rotation][newRotation][attempt][1], 
        dir)
      ){
        return true;
      }
    }
  }
}

void resetTetriminoPostion(){
  if(queue[0] == I){
    currentPieceX = 3;
    currentPieceY = 19;
  }else if(queue[0] == O){
    currentPieceX = 4;
    currentPieceY = 19;
  }else{
    currentPieceX = 3;
    currentPieceY = 20;
  }
  // TODO: Check for game over
}

void lockTetriminoPosiontion(){
  if(queue[0] == O){
    board[currentPieceX + currentPieceY * 10] = O;
    board[currentPieceX + currentPieceY * 10 + 1] = O;
    board[currentPieceX + currentPieceY * 10 + 10] = O;
    board[currentPieceX + currentPieceY * 10 + 11] = O;
  }else if(queue[0] == I){
    uint8_t xi;
    uint8_t yi;
    for (xi = 0; xi < 4; xi++){
      for (yi = 0; yi < 4; yi++){
        if(tetriminoLUTUnclipped(queue[0], xi, yi, rotation) != _){
          board[(currentPieceX + xi) + (currentPieceY + yi) * 10] = queue[0];
        }
      }
    }
  }else{
    uint8_t xi;
    uint8_t yi;
    for (xi = 0; xi < 3; xi++){
      for (yi = 0; yi < 3; yi++){
        if(tetriminoLUTUnclipped(queue[0], xi, yi, rotation) != _){
          board[(currentPieceX + xi) + (currentPieceY + yi) * 10] = queue[0];
        }
      }
    }
  }
  nextQueueTetrimino();
  resetTetriminoPostion();
}