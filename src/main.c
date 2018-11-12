/*
 *--------------------------------------
 * Program Name:
 * Author:
 * License:
 * Description:
 *--------------------------------------
*/

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Standard headers (recommended) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>
#include <keypadc.h>
/**
 * Keypad Data registers
 *
 * | Offset | Bit 0      | Bit 1      | Bit 2      | Bit 3      |  Bit 4     |  Bit 5     |  Bit 6     | Bit 7      |
 * | -------| ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- |
 * | 1      | kb_Graph   | kb_Trace   | kb_Zoom    | kb_Window  | kb_Yequ    | kb_2nd     | kb_Mode    | kb_Del     |
 * | 2      |            | kb_Sto     | kb_Ln      | kb_Log     | kb_Square  | kb_Recip   | kb_Math    | kb_Alpha   |
 * | 3      | kb_0       | kb_1       | kb_4       | kb_7       | kb_Comma   | kb_Sin     | kb_Apps    | kb_GraphVar|
 * | 4      | kb_DecPnt  | kb_2       | kb_5       | kb_8       | kb_LParen  | kb_Cos     | kb_Prgm    | kb_Stat    |
 * | 5      | kb_Chs     | kb_3       | kb_6       | kb_9       | kb_RParen  | kb_Tan     | kb_Vars    |            |
 * | 6      | kb_Enter   | kb_Add     | kb_Sub     | kb_Mul     | kb_Div     | kb_Power   | kb_Clear   |            |
 * | 7      | kb_Down    | kb_Left    | kb_Right   | kb_Up      |            |            |            |            |
 * 
 * These data registers can be indexed just like an array. For example, if you want to check the status of the '2nd' key:
 * @code
 *  if (kb_Data[1] & kb_2nd) {
 *      ...
 *  }
 * @endcode
 */

// Color Macro
// R 0-7
// G 0-7
// R 0-3

#include "gfx/tetris_gfx.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

enum Mode {
    logo,
    main,
    game,
    stop
};
typedef enum Mode mode_t;

mode_t mode = logo;
short time;
uint8_t originalBrightness;

gfx_sprite_t *tetris_logo_1;
gfx_sprite_t *tetris_logo_2;
gfx_sprite_t *tile;

void step();
void drawTetrisLogo();
void drawHighScores();

void main() {
    os_ClrHome();
    os_PutStrFull("Decompressing Sprites...");

    tetris_logo_1 = gfx_MallocSprite(tetris_logo_1_width, tetris_logo_1_height);
    zx7_Decompress(tetris_logo_1, tetris_logo_1_compressed);
    tetris_logo_2 = gfx_MallocSprite(tetris_logo_2_width, tetris_logo_2_height);
    zx7_Decompress(tetris_logo_2, tetris_logo_2_compressed);
    tile = gfx_MallocSprite(tile_width, tile_height);
    zx7_Decompress(tile, tile_compressed);

    originalBrightness = lcd_BacklightLevel;
    gfx_Begin();
    gfx_SetPalette(tetris_gfx_pal, sizeof_tetris_gfx_pal, 0);
    gfx_SetDrawBuffer();

    do {
        step();
        gfx_SwapDraw();
    } while (mode != stop);
    gfx_End();
    lcd_BacklightLevel = originalBrightness;
}

void step(){
    kb_Scan();
    if(kb_Data[6] & kb_Clear){
        mode = stop;
    }
    switch(mode){
        case logo:
            drawTetrisLogo();
            lcd_BacklightLevel = MAX(0, 255-(2*time));
            if (time > 128){
                mode = main;
            }
        break;
        case main:
            drawTetrisLogo();
            drawHighScores();
            if(kb_Data[6] & kb_Enter){
                mode = game;
            }
        break;
        case game:
            gfx_FillScreen(0x02);
            gfx_Sprite(tile, 0, 0);
            gfx_Sprite(tile, 16, 0);
            gfx_Sprite(tile, 0, 16);
            gfx_Sprite(tile, 16, 16);
        break;
    }
    time++;
}

void drawTetrisLogo(){
    gfx_Sprite(tetris_logo_1, 0, 0);
    gfx_Sprite(tetris_logo_2, 160, 0);
}

void drawHighScores(){
    gfx_SetTextFGColor(0x08);
    gfx_PrintStringXY("High Scores:", 120, 110);
    gfx_PrintStringXY("1: 0000 - JDG", 120, 120);
    if((time >> 5) & 1){
        gfx_PrintStringXY("Press    [enter]    to    start", 20, 220);
    }
}
