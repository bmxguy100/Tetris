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

void step();
void startGame();
void endGame();
void drawTetrisLogo();
void drawHighScores();
void drawUI();
void drawTiles();
void drawGameOver();

#include "gfx/tetris_gfx.h"
#include "game.h"
#include "queue.h"

void drawTetriminoPreview(tetrimino_tile_t tetrimino, uint16_t x, uint8_t y);

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define DECOMPRESS(name) \
    name = gfx_MallocSprite(name##_width, name##_height); \
    zx7_Decompress(name, name##_compressed)


enum Mode {
    logo,
    menu,
    game,
    gameOver,
    stop
};
typedef enum Mode mode_t;

mode_t mode = logo;
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
gfx_sprite_t *tile_outline;

void main() {
    uint16_t i = 0;

    srandom(rtc_Time());

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
    DECOMPRESS(tile_outline);

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
    free(tile_outline);
}

void step(){
    kb_Scan();
    if(kb_Data[6] & kb_Clear){
        mode = stop;
    }
    switch(mode){
        case logo:
            drawTetrisLogo();
            lcd_BacklightLevel = MAX(0, 255-(4*frameCount));
            if (frameCount > 70){
                mode = menu;
            }
        break;
        case menu:
            drawTetrisLogo();
            drawHighScores();
            if(kb_Data[6] & kb_Enter){
                startGame();
            }
        break;
        case game:
            processInput();
            drawUI();            
            drawTiles();
            if(kb_Data[1] & kb_Del){
                mode = menu;
            }
        break;
        case gameOver:
            drawGameOver();
            if(kb_Data[1] & kb_Del){
                mode = menu;
            }
        break;
    }
    frameCount++;
}

void startGame(){
    while(kb_Data[6] & kb_Enter){
        kb_Scan();
    }
    memset(board, _, sizeof(board));
    initializeQueue();
    resetTetriminoPostion();
    mode = game;
}

void endGame(){
    mode = gameOver;
    // TODO: Add to high scores
}

void drawTetrisLogo(){
    gfx_Sprite(tetris_logo_1, 0, 0);
    gfx_Sprite(tetris_logo_2, 160, 0);
}

void drawHighScores(){
    gfx_SetTextFGColor(0x02);
    gfx_PrintStringXY("High Scores:", 120, 120);
    gfx_PrintStringXY("1: 0000 - JDG", 120, 130);
    if((frameCount >> 5) & 1){
        gfx_PrintStringXY("Press    [enter]    to    start", 20, 225);
    }
}

void drawUI(){
    gfx_FillScreen(0x01);
    gfx_SetColor(0x03);
    
    gfx_FillRectangle_NoClip(100, 0, 120, 240); // Game Board

    gfx_FillRectangle_NoClip(225, 5, 60, 60); // Stored Piece BG
    gfx_FillRectangle_NoClip(35, 5, 60, 170); // Queue BG

    gfx_SetColor(0x04);

    gfx_FillRectangle_NoClip(230, 10, 50, 50); // Stored Piece

    drawTetriminoPreview(asideTetrimino, 231, 11);

    // Queue
    gfx_FillRectangle_NoClip(40, 10, 50, 50);
    gfx_FillRectangle_NoClip(40, 65, 50, 50);
    gfx_FillRectangle_NoClip(40, 120, 50, 50);

    drawTetriminoPreview(queue[1], 41, 11);
    drawTetriminoPreview(queue[2], 41, 66);
    drawTetriminoPreview(queue[3], 41, 121);
}

void drawTetriminoPreview(tetrimino_tile_t tetrimino, uint16_t x, uint8_t y){
    switch (tetrimino){
        case _:
        break;
        case I:
            gfx_Sprite(tile_cyan, x, y + 18);
            gfx_Sprite(tile_cyan, x + 12, y + 18);
            gfx_Sprite(tile_cyan, x + 24, y + 18);
            gfx_Sprite(tile_cyan, x + 36, y + 18);
        break;
        case O:
            gfx_Sprite(tile_yellow, x + 12, y + 12);
            gfx_Sprite(tile_yellow, x + 24, y + 12);
            gfx_Sprite(tile_yellow, x + 12, y + 24);
            gfx_Sprite(tile_yellow, x + 24, y + 24);
        break;
        case T:
            gfx_Sprite(tile_purple, x + 18, y + 12);
            gfx_Sprite(tile_purple, x + 6, y + 24);
            gfx_Sprite(tile_purple, x + 18, y + 24);
            gfx_Sprite(tile_purple, x + 30, y + 24);
        break;
        case S:
            gfx_Sprite(tile_green, x + 18, y + 12);
            gfx_Sprite(tile_green, x + 30, y + 12);
            gfx_Sprite(tile_green, x + 6, y + 24);
            gfx_Sprite(tile_green, x + 18, y + 24);
        break;
        case Z:
            gfx_Sprite(tile_red, x + 6, y + 12);
            gfx_Sprite(tile_red, x + 18, y + 12);
            gfx_Sprite(tile_red, x + 18, y + 24);
            gfx_Sprite(tile_red, x + 30, y + 24);
        break;
        case J:
            gfx_Sprite(tile_blue, x + 6, y + 12);
            gfx_Sprite(tile_blue, x + 6, y + 24);
            gfx_Sprite(tile_blue, x + 18, y + 24);
            gfx_Sprite(tile_blue, x + 30, y + 24);
        break;
        case L:
            gfx_Sprite(tile_orange, x + 30, y + 12);
            gfx_Sprite(tile_orange, x + 6, y + 24);
            gfx_Sprite(tile_orange, x + 18, y + 24);
            gfx_Sprite(tile_orange, x + 30, y + 24);
        break;
    }
}

void drawTiles(){
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t xPos = 0;
    uint8_t yPos = 0;
    for(y = 0; y < 20; y++){
        for(x = 0; x < 10; x++){
            xPos = 12 * x + 100;
            yPos = 12 * y;
            switch(getTileAt(x, y+20)){
                case _:
                break;
                case I:
                    gfx_Sprite(tile_cyan, xPos, yPos);
                break;
                case O:
                    gfx_Sprite(tile_yellow, xPos, yPos);
                break;
                case T:
                    gfx_Sprite(tile_purple, xPos, yPos);
                break;
                case S:
                    gfx_Sprite(tile_green, xPos, yPos);
                break;
                case Z:
                    gfx_Sprite(tile_red, xPos, yPos);
                break;
                case J:
                    gfx_Sprite(tile_blue, xPos, yPos);
                break;
                case L:
                    gfx_Sprite(tile_orange, xPos, yPos);
                break;
                case outline:
                    gfx_Sprite(tile_outline, xPos, yPos);
                break;
            }
        }
    }
}

void drawGameOver(){
    gfx_FillScreen(0x01);
    gfx_SetColor(0x03);
    
    gfx_FillRectangle_NoClip(50, 30, 220, 200);

    gfx_SetTextFGColor(0x01);
    gfx_SetTextScale(3, 4);
    gfx_PrintStringXY("Game Over!", 55, 35);

    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Score:", 55, 75);
    gfx_PrintStringXY("Lines Cleared:", 55, 85);
    gfx_PrintStringXY("Level:", 55, 95);
}