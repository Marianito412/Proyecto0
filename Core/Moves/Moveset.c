#pragma once

#include "../MagicSquare/MagicSquare.c"


typedef struct Moveset{
    void (*Move)(MagicSquare*, Coordinate*);
    void (*BreakMove)(MagicSquare*, Coordinate*);
} Moveset;

//North east move
void NorthEast(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y -=1;
    Coordinate->x +=1;
    WrapAround(Square, Coordinate);
};

void NorthEastBr(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y +=1;
    WrapAround(Square, Coordinate);
};

//North west move
void NorthWest(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y -=1;
    Coordinate->x -=1;
    WrapAround(Square, Coordinate);
};

void NorthWestBr(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y +=1;
    WrapAround(Square, Coordinate);
};

// South east move
void SouthEast(MagicSquare* Square, Coordinate* Coordinate) {
    Coordinate->y += 1;
    Coordinate->x += 1;
    WrapAround(Square, Coordinate);
}

void SouthEastBr(MagicSquare* Square, Coordinate* Coordinate) {
    Coordinate->y -= 1;
    WrapAround(Square, Coordinate);
}

//South west move
void SouthWest(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y +=1;
    Coordinate->x -=1;
    WrapAround(Square, Coordinate);
};

void SouthWestBr(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y -=1;
    WrapAround(Square, Coordinate);
};

//L Move
void NorthEastL(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y -=2;
    Coordinate->x +=1;
    WrapAround(Square, Coordinate);
};

void NorthEeastLBr(MagicSquare* Square, Coordinate* Coordinate){
    Coordinate->y -=1;
    Coordinate->x -=1;
    WrapAround(Square, Coordinate);
};