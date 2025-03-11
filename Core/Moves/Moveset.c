#pragma once

#include "../MagicSquare/MagicSquare.c"


typedef struct Moveset{
    void (*Move)(MagicSquare*, Coordinate*);
    void (*BreakMove)(MagicSquare*, Coordinate*);
} Moveset;

void NorthEast(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y -=1;
    Coordinate->x +=1;
    WrapAround(Square, Coordinate);
};

void NorthEastBr(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y +=1;
    WrapAround(Square, Coordinate);
};

void NorthWest(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y -=1;
    Coordinate->x -=1;
    WrapAround(Square, Coordinate);
};

void NorthWestBr(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y +=1;
    WrapAround(Square, Coordinate);
};