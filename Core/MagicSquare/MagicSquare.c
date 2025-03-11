//#include "MagicSquare.h"

typedef struct Coordinate{
    int x;
    int y;
} Coordinate;

typedef struct MagicSquare
{
    int Width;
    int Height;
    Coordinate CurrentSlot;
    int** Grid;
} MagicSquare;

/*Constructor*/
MagicSquare* NewSquare(int Width, int Height){
    MagicSquare* Square = malloc(sizeof(MagicSquare));

    Square->Width = Width;
    Square->Height = Height;

    Square->CurrentSlot.x = 0;
    Square->CurrentSlot.y = 0;

    Square->Grid = (int**)malloc(Height*sizeof(int*));
    for (int i = 0; i<Height;i++){
        Square->Grid[i] = (int*)malloc(sizeof(int)*Width);
    }

    for (int i = 0; i<Height;i++){
        for (int j = 0; j<Width; j++){
            Square->Grid[j][i] = 0;
        }
    }
    return Square;
}

/*Private*/
void WrapAround(MagicSquare* Square){
    if (Square->CurrentSlot.x >= Square->Width){
        Square->CurrentSlot.x -= Square->Width;
    }
    if (Square->CurrentSlot.x < 0){
        Square->CurrentSlot.x += Square->Width;
    }

    if (Square->CurrentSlot.y >= Square->Height){
        Square->CurrentSlot.x -= Square->Height;
    }
    if (Square->CurrentSlot.x < 0){
        Square->CurrentSlot.x += Square->Height;
    }
}

/*Public*/
void Move(MagicSquare* Square, int horizontal, int vertical){
    Square->CurrentSlot.x += horizontal;
    Square->CurrentSlot.y += vertical;
    WrapAround(Square);
}

int GetCurrent(MagicSquare* Square){
    int a = Square->Grid[Square->CurrentSlot.x][Square->CurrentSlot.y];
    return a;
}

void SetCurrent(MagicSquare* Square, int NewValue){
    Square->Grid[Square->CurrentSlot.x][Square->CurrentSlot.y] = NewValue;
}

void Print(MagicSquare* Square){
    for (int i = 0; i < Square->Height;i++){
        for (int j = 0; j < Square->Width; j++){
            printf("%d\t", Square->Grid[j][i]);
        }
        printf("\n");
    }
}