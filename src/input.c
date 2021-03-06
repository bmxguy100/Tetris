#include "input.h"
#include "queue.h"

#define IDLE_DAS_CYCLES 6

uint24_t leftDASCounter;
uint24_t rightDASCounter;
uint24_t softDropDASCounter;

int8_t lockDelay = 0;

bool wasHardDropKey;
bool wasRotateRightKey;
bool wasRotateLeftKey;
bool wasStoreKey;

void processInput(){
  bool leftKey = kb_Data[kb_group_3] & kb_4;
  bool rightKey = kb_Data[kb_group_5] & kb_6;
  bool softDropKey = kb_Data[kb_group_4] & kb_2;

  bool hardDropKey = kb_Data[kb_group_4] & kb_8;
  bool hardDrop = hardDropKey && !wasHardDropKey;

  bool rotateRightKey = kb_Data[kb_group_5] & kb_9;
  bool rotateLeftKey = kb_Data[kb_group_3] & kb_7;

  bool rotateRight = rotateRightKey && !wasRotateRightKey && !rotateLeftKey;
  bool rotateLeft = rotateLeftKey && !wasRotateLeftKey && !rotateRightKey;

  bool storeKey = kb_Data[kb_group_4] & kb_5;
  bool store = storeKey && !wasStoreKey;

  bool left = false;
  bool right = false;
  bool softDrop = false;

  uint8_t speed;

  if(leftKey && !rightKey) {
    leftDASCounter++;
  } else {
    leftDASCounter = 0;
  }
  if(rightKey && !leftKey) {
    rightDASCounter++;
  } else {
    rightDASCounter = 0;
  }
  if(softDropKey) {
    softDropDASCounter++;
  } else {
    softDropDASCounter = 0;
  }

  left = (leftDASCounter == 1) || (leftDASCounter > IDLE_DAS_CYCLES);
  right = (rightDASCounter == 1) || (rightDASCounter > IDLE_DAS_CYCLES);
  softDrop = (softDropDASCounter == 1) || (softDropDASCounter > IDLE_DAS_CYCLES);

  wasHardDropKey = hardDropKey;
  wasRotateRightKey = rotateRightKey;
  wasRotateLeftKey = rotateLeftKey;
  wasStoreKey = storeKey;

  if(store){
    storeTetrimino();
  }

  if(left || right){
    tryMove(left ? -1 : (right ? 1 : 0), 0, 0);
  }

  if(softDrop){
    if(tryMove(0, 1, 0)){
      score += 1;
    }
  }

  if(rotateLeft){
    tryRotate(-1);
  }

  if(rotateRight){
    tryRotate(1);
  }
  
  if(level <= 4){
    speed = 15 - (level * 3);
  } else {
    speed = 1;
  }

  if(frameCount % speed == 0){
    if(tryMove(0, 1, 0)){
      lockDelay = 4;
    }else{
      if(--lockDelay == 0){
        lockTetriminoPosiontion();
      }
    }
  }

  if(hardDrop){
    while(tryMove(0, 1, 0)){
      score += 2;
    }
    lockTetriminoPosiontion();
  }
}