#ifndef FILES_H
#define FILES_H

#include <tice.h>
#include <fileioc.h>

void loadHighScores();
void saveHighScores();

typedef struct {
    uint24_t score;
    char c1;
    char c2;
    char c3;
} highscore_t;

extern highscore_t highscores[5];

#endif