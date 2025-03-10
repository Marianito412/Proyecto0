
struct Coordinate{
    int x;
    int y;
};

typedef Coordinate;

struct MagicSquare
{
    int Width;
    int Height;
    Coordinate CurrentSlot;
    int** Grid;
};

typedef MagicSquare;

/*Constructor*/

MagicSquare* NewSquare(int Width, int Height);

/*Private*/
void WrapAround(MagicSquare* Square);

/*Public*/
void Move(MagicSquare* Square, int horizontal, int vertical);
int GetCurrent(MagicSquare* Square);
void SetCurrent(MagicSquare* Square, int NewValue);

void Print(MagicSquare* Square);