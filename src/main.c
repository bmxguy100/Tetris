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

#include <compression.h>

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

#include "gfx/tetris_gfx.h"
#include "game.c"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define DECOMPRESS(name) \
    name = gfx_MallocSprite(name##_width, name##_height); \
    zx7_Decompress(name, name##_compressed)

enum Mode {
    logo,
    menu,
    game,
    stop
};
typedef enum Mode mode_t;

mode_t mode = logo;
short time;
uint8_t originalBrightness;

gfx_sprite_t *tetris_logo_1;
gfx_sprite_t *tetris_logo_2;
gfx_sprite_t *tile_cyan;
gfx_sprite_t *tile_yellow;
gfx_sprite_t *tile_purple;
gfx_sprite_t *tile_green;
gfx_sprite_t *tile_red;
gfx_sprite_t *tile_blue;
gfx_sprite_t *tile_orange;

void step();
void drawTetrisLogo();
void drawHighScores();
void drawTiles();

void main() {
    uint16_t i = 0;

    srandom(rtc_Time());
    for (i = 200; i < 400; i++){
        board[i] = random()&7;
    }

    os_ClrHome();

    DECOMPRESS(tetris_logo_1);
    DECOMPRESS(tetris_logo_2);
    DECOMPRESS(tile_cyan);
    DECOMPRESS(tile_yellow);
    DECOMPRESS(tile_purple);
    DECOMPRESS(tile_green);
    DECOMPRESS(tile_red);
    DECOMPRESS(tile_blue);
    DECOMPRESS(tile_orange);

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

    free(tetris_logo_1);
    free(tetris_logo_2);
    free(tile_cyan);
    free(tile_yellow);
    free(tile_purple);
    free(tile_green);
    free(tile_red);
    free(tile_blue);
    free(tile_orange);
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
                mode = menu;
            }
        break;
        case menu:
            drawTetrisLogo();
            drawHighScores();
            if(kb_Data[6] & kb_Enter){
                mode = game;
            }
        break;
        case game:
            gfx_FillScreen(0x01);
            gfx_SetColor(0x03);
            
            gfx_FillRectangle_NoClip(100, 0, 120, 240);

            gfx_FillRectangle_NoClip(49, 5, 46, 46);
            gfx_FillRectangle_NoClip(49, 58, 46, 128);
            gfx_SetColor(0x04);
            gfx_FillRectangle_NoClip(54, 10, 36, 36);
	
            gfx_FillRectangle_NoClip(54, 63, 36, 36);
            gfx_FillRectangle_NoClip(54, 104, 36, 36);
            gfx_FillRectangle_NoClip(54, 145, 36, 36);
            
            drawTiles();
        break;
    }
    time++;
}

void drawTetrisLogo(){
    gfx_Sprite(tetris_logo_1, 0, 0);
    gfx_Sprite(tetris_logo_2, 160, 0);
}

void drawHighScores(){
    gfx_SetTextFGColor(0x02);
    gfx_PrintStringXY("High Scores:", 120, 110);
    gfx_PrintStringXY("1: 0000 - JDG", 120, 120);
    if((time >> 5) & 1){
        gfx_PrintStringXY("Press    [enter]    to    start", 20, 220);
    }
}

void drawTiles(){
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t xPos = 0;
    uint8_t yPos = 0;
    uint16_t i = 200;
    for(y = 0; y < 20; y++){
        for(x = 0; x < 10; x++){
            xPos = 12 * x + 100;
            yPos = 12 * y;
            switch(board[i]){
                case none:
                break;
                case cyan:
                    gfx_Sprite(tile_cyan, xPos, yPos);
                break;
                case yellow:
                    gfx_Sprite(tile_yellow, xPos, yPos);
                break;
                case purple:
                    gfx_Sprite(tile_purple, xPos, yPos);
                break;
                case green:
                    gfx_Sprite(tile_green, xPos, yPos);
                break;
                case red:
                    gfx_Sprite(tile_red, xPos, yPos);
                break;
                case blue:
                    gfx_Sprite(tile_blue, xPos, yPos);
                break;
                case orange:
                    gfx_Sprite(tile_orange, xPos, yPos);
                break;
            }
            i++;
        }
    }
}
