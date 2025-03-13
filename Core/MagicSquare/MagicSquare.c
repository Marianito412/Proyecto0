#pragma once

struct Coordinate{
    int x;
    int y;
};

typedef struct Coordinate Coordinate;

struct MagicSquare{
    int Width;
    int Height;
    Coordinate CurrentSlot;
    int** Grid;
};

typedef struct MagicSquare MagicSquare;

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

void DestroySquare(MagicSquare* Square){
    for (int i = 0; i<Square->Height; i++){
        free(Square->Grid[i]);
    }
    free(Square->Grid);
    free(Square);
}

/*Private*/
void WrapAround(MagicSquare* Square, Coordinate* Coords){
    //Horizontal borders
    if (Coords->x >= Square->Width){
        Coords->x -= Square->Width;
    }
    if (Coords->x < 0){
        Coords->x += Square->Width;
    }

    //Vertical borders
    if (Coords->y >= Square->Height){
        Coords->y -= Square->Height;
    }
    if (Coords->y < 0){
        Coords->y += Square->Height;
    }
}

/*Public*/
void Move(MagicSquare* Square, int horizontal, int vertical){
    Square->CurrentSlot.x += horizontal;
    Square->CurrentSlot.y += vertical;
    WrapAround(Square, &Square->CurrentSlot);
}

void ApplyMoveToCoords(MagicSquare* Square, Coordinate* Coords, int horizontal, int vertical){
    Coords->x += horizontal;
    Coords->y += vertical;
    WrapAround(Square, Coords);
}

int GetCurrent(MagicSquare* Square){
    int a = Square->Grid[Square->CurrentSlot.x][Square->CurrentSlot.y];
    return a;
}

int GetValueAtCoords(MagicSquare* Square, Coordinate Coords){
    int a = Square->Grid[Coords.x][Coords.y];
    return a;
}

void SetCurrent(MagicSquare* Square, int NewValue){
    Square->Grid[Square->CurrentSlot.x][Square->CurrentSlot.y] = NewValue;
}

void Print(MagicSquare* Square){
    for (int i = 0; i < Square->Height; i++){
        for (int j = 0; j < Square->Width; j++){
            if (Square->CurrentSlot.y == i && Square->CurrentSlot.x == j){
                printf("%d*\t", Square->Grid[j][i]);
            }
            else{
                printf("%d\t", Square->Grid[j][i]);
            }
        }
        printf("\n");
    }
}