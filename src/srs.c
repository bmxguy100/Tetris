#include "srs.h"

// _WALL_KICK_LUT[oldRotation][newRotation][check #][x or y]
int8_t JLSTZ_WALL_KICK_LUT[4][4][5][2] = {
  {
    {{0,0},{0,0},{0,0},{0,0}}, // 0=>0
    { { 0, 0}, {-1, 0}, {-1,-1}, { 0, 2}, {-1, 2} }, // 0=>1
    {{0,0},{0,0},{0,0},{0,0}}, // 0=>2
    { { 0, 0}, {-1, 0}, {-1, 1}, { 0,-2}, {-1, 2} } // 0=>3
  },
  {
    { { 0, 0}, { 1, 0}, { 1, 1}, { 0,-2}, { 1,-2} }, // 1=>0
    {{0,0},{0,0},{0,0},{0,0}}, // 1=>1
    { { 0, 0}, { 1, 0}, { 1, 1}, { 0,-2}, { 1,-2} }, // 1=>2
    {{0,0},{0,0},{0,0},{0,0}} // 1=>3
  },
  {
    {{0,0},{0,0},{0,0},{0,0}}, // 2=>0
    { { 0, 0}, {-1, 0}, {-1,-1}, { 0, 2}, {-1, 2} }, // 2=>1
    {{0,0},{0,0},{0,0},{0,0}}, // 2=>2
    { { 0, 0}, { 1, 0}, { 1,-1}, { 0, 2}, { 1, 2} } // 2=>3
  },
  {
    { { 0, 0}, {-1, 0}, {-1, 1}, { 0,-2}, {-1,-2} }, // 3=>0
    {{0,0},{0,0},{0,0},{0,0}}, // 3=>1
    { { 0, 0}, {-1, 0}, {-1, 1}, { 0,-2}, {-1,-2} }, // 3=>2
    {{0,0},{0,0},{0,0},{0,0}} // 3=>3
  },
};

int8_t I_WALL_KICK_LUT[4][4][5][2] = {
  {
    {{0,0},{0,0},{0,0},{0,0}}, // 0=>0
    { { 0, 0}, {-2, 0}, { 1, 0}, {-2, 1}, { 1,-2} }, // 0=>1
    {{0,0},{0,0},{0,0},{0,0}}, // 0=>2
    { { 0, 0}, {-1, 0}, { 2, 0}, {-1,-2}, { 2, 1} } // 0=>3
  },
  {
    { { 0, 0}, { 2, 0}, {-1, 0}, { 2,-1}, {-1, 2} }, // 1=>0
    {{0,0},{0,0},{0,0},{0,0}}, // 1=>1
    { { 0, 0}, {-1, 0}, { 2, 0}, {-1,-2}, { 2, 1} }, // 1=>2
    {{0,0},{0,0},{0,0},{0,0}} // 1=>3
  },
  {
    {{0,0},{0,0},{0,0},{0,0}}, // 2=>0
    { { 0, 0}, { 1, 0}, {-2, 0}, { 1, 2}, {-2,-1} }, // 2=>1
    {{0,0},{0,0},{0,0},{0,0}}, // 2=>2
    { { 0, 0}, { 2, 0}, {-1, 0}, { 2,-1}, {-1, 2} } // 2=>3
  },
  {
    { { 0, 0}, { 1, 0}, {-2, 0}, { 1, 2}, {-2,-1} }, // 3=>0
    {{0,0},{0,0},{0,0},{0,0}}, // 3=>1
    { { 0, 0}, {-2, 0}, { 1, 0}, {-2, 1}, { 1,-2} }, // 3=>2
    {{0,0},{0,0},{0,0},{0,0}} // 3=>3
  },
};

// _ROTATION_LUT[rotation][y][x]
tetrimino_tile_t I_ROTATION_LUT[4][4][4] = {
  {
    {_,_,_,_},
    {I,I,I,I},
    {_,_,_,_},
    {_,_,_,_}
  },
  {
    {_,_,I,_},
    {_,_,I,_},
    {_,_,I,_},
    {_,_,I,_}
  },
  {
    {_,_,_,_},
    {_,_,_,_},
    {I,I,I,I},
    {_,_,_,_}
  },
  {
    {_,I,_,_},
    {_,I,_,_},
    {_,I,_,_},
    {_,I,_,_}
  }
};
tetrimino_tile_t T_ROTATION_LUT[4][3][3] = {
  {
    {_,T,_},
    {T,T,T},
    {_,_,_}
  },
  {
    {_,T,_},
    {_,T,T},
    {_,T,_}
  },
  {
    {_,_,_},
    {T,T,T},
    {_,T,_}
  },
  {
    {_,T,_},
    {T,T,_},
    {_,T,_}
  },
};
tetrimino_tile_t S_ROTATION_LUT[4][3][3] = {
  {
    {_,S,S},
    {S,S,_},
    {_,_,_}
  },
  {
    {_,S,_},
    {_,S,S},
    {_,_,S}
  },
  {
    {_,_,_},
    {_,S,S},
    {S,S,_}
  },
  {
    {S,_,_},
    {S,S,_},
    {_,S,_}
  },
};
tetrimino_tile_t Z_ROTATION_LUT[4][3][3] = {
  {
    {Z,Z,_},
    {_,Z,Z},
    {_,_,_}
  },
  {
    {_,_,Z},
    {_,Z,Z},
    {_,Z,_}
  },
  {
    {_,_,_},
    {Z,Z,_},
    {_,Z,Z}
  },
  {
    {_,Z,_},
    {Z,Z,_},
    {Z,_,_}
  },
};
tetrimino_tile_t J_ROTATION_LUT[4][3][3] = {
  {
    {J,_,_},
    {J,J,J},
    {_,_,_}
  },
  {
    {_,J,J},
    {_,J,_},
    {_,J,_}
  },
  {
    {_,_,_},
    {J,J,J},
    {_,_,J}
  },
  {
    {_,J,_},
    {_,J,_},
    {J,J,_}
  },
};
tetrimino_tile_t L_ROTATION_LUT[4][3][3] = {
  {
    {_,_,L},
    {L,L,L},
    {_,_,_}
  },
  {
    {_,L,_},
    {_,L,_},
    {_,L,L}
  },
  {
    {_,_,_},
    {L,L,L},
    {L,_,_}
  },
  {
    {L,L,_},
    {_,L,_},
    {_,L,_}
  },
};