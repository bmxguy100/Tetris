#include "files.h"

const char *appvarName = "TetrisHS";

highscore_t highscores[5] = {
    {10000, 'J', 'D', 'G'},
    {0, ' ', ' ', ' '},
    {0, ' ', ' ', ' '},
    {0, ' ', ' ', ' '},
    {0, ' ', ' ', ' '}
};

ti_var_t highscoresAppVar;

void loadHighScores(){
    ti_CloseAll();
    highscoresAppVar = ti_Open(appvarName, "r");
    ti_Read(&highscores, sizeof(highscores), 1, highscoresAppVar);
    ti_SetArchiveStatus(true, highscoresAppVar);
    ti_Close(highscoresAppVar);
}

void saveHighScores(){
    highscoresAppVar = ti_Open(appvarName, "w");
    ti_Write(&highscores, sizeof(highscores), 1, highscoresAppVar);
    ti_SetArchiveStatus(true, highscoresAppVar);
    ti_CloseAll();
}
