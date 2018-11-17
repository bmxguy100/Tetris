// http://tetris.wikia.com/wiki/Tetris_Guideline
// Game logic goes in this file

enum Tile {
    none,
    cyan,
    yellow,
    purple,
    green,
    red,
    blue,
    orange
};
typedef enum Tile tile_t;

tile_t board[400];