#include "../MagicSquare/MagicSquare.c"


//Mover-Moves
void NorthEast(Coordinate* Coordinate){
    Coordinate->y -=1;
    Coordinate->x +=1;
};
void NorthWest(Coordinate* Coordinate){
    Coordinate->y -=1;
    Coordinate->x -=1;
};
void SouthEast(Coordinate* Coordinate){
    Coordinate->y +=1;
    Coordinate->x +=1;
};
void SouthWest(Coordinate* Coordinate){
    Coordinate->y +=1;
    Coordinate->x -=1;
};

void NorthEastL(Coordinate* Coordinate){
    Coordinate->y -=2;
    Coordinate->x +=1;
};

//Break-Moves