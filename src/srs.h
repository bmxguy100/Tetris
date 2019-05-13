#ifndef SRS_H
#define SRS_H
#include <stdint.h>

extern int8_t JLSTZ_WALL_KICK_LUT[4][4][5][2];
extern int8_t I_WALL_KICK_LUT[4][4][5][2];

#include "game.h"

extern tetrimino_tile_t I_ROTATION_LUT[4][4][4];
extern tetrimino_tile_t T_ROTATION_LUT[4][3][3];
extern tetrimino_tile_t S_ROTATION_LUT[4][3][3];
extern tetrimino_tile_t Z_ROTATION_LUT[4][3][3];
extern tetrimino_tile_t J_ROTATION_LUT[4][3][3];
extern tetrimino_tile_t L_ROTATION_LUT[4][3][3];
#endif