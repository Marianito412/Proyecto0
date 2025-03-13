#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>  // Para usar el tipo de dato 'bool'

#include "Core/MagicSquare/MagicSquare.c"
#include "Core/Moves/Moveset.c"

int CurrentNumber = 0;

Moveset NorthWestMoveset = {NorthWest, NorthWestBr};
Moveset NorthEastMoveset = {NorthEast, NorthEastBr};
Moveset SouthWestMoveset = {SouthWest, SouthWestBr};
Moveset SouthEastMoveset = {SouthEast, SouthEastBr};
Moveset NorthEastLMoveset = {NorthEastL, NorthEeastLBr};

MagicSquare* GlobalSquare = NULL;

GtkPaned* Paned = NULL;
GtkGrid* GlobalGrid = NULL;


void Test(GtkSpinButton* Button, gpointer UserData){
    printf("Value Changed");
}

Moveset GetMoveset(gchararray moveset){
    if (moveset == "NorthWest") return NorthWestMoveset;
    if (moveset == "NorthEast") return NorthEastMoveset;
    if (moveset == "SouthWest") return SouthWestMoveset;
    if (moveset == "SouthEast") return SouthEastMoveset;
    if (moveset == "NorthEastL") return NorthEastLMoveset;
}

void ProcessNextMove(MagicSquare* Square, Moveset* MoveSet, int i){
    Coordinate NewCoords = Square->CurrentSlot;
    MoveSet->Move(Square, &NewCoords);

    if (GetValueAtCoords(Square, NewCoords) == 0){
        MoveSet->Move(Square, &Square->CurrentSlot);
    }
    else{
        MoveSet->BreakMove(Square, &Square->CurrentSlot);
    }
    printf("(%d, %d) -> %d\n", Square->CurrentSlot.x, Square->CurrentSlot.y, i);
    SetCurrent(Square, i);
}

void UpdateGrid(GtkGrid* Grid, MagicSquare* Square){
    for (int i = 0; i < Square->Height; i++) {
        for(int j = 0; j < Square->Width; j++) {
            GtkWidget* widget = gtk_grid_get_child_at(Grid, j, i);
            if (GTK_IS_LABEL(widget)) {
                char buffer[10];
                snprintf(buffer, sizeof(buffer), "%d", Square->Grid[i][j]);
                printf("(%d, %d) -> %d\n", j, i, Square->Grid[i][j]);
                gtk_label_set_text(GTK_LABEL(widget), buffer);
            }
        }
    }
}

/* Función para calcular la suma de filas, columnas y diagonales */
void CalculateSums(MagicSquare* Square) {
    int sum = 0;
    int magicConstant = Square->Width * (Square->Width * Square->Width + 1) / 2;

    // Sumar filas
    for (int i = 0; i < Square->Height; i++) {
        sum = 0;
        for (int j = 0; j < Square->Width; j++) {
            sum += Square->Grid[i][j];
        }
        printf("Fila %d: %d\n", i + 1, sum);
    }

    // Sumar columnas
    for (int j = 0; j < Square->Width; j++) {
        sum = 0;
        for (int i = 0; i < Square->Height; i++) {
            sum += Square->Grid[i][j];
        }
        printf("Columna %d: %d\n", j + 1, sum);
    }

    // Sumar diagonal principal
    sum = 0;
    for (int i = 0; i < Square->Height; i++) {
        sum += Square->Grid[i][i];
    }
    printf("\nSuma de la diagonal principal: %d\n", sum);

    // Sumar diagonal secundaria
    sum = 0;
    for (int i = 0; i < Square->Height; i++) {
        sum += Square->Grid[i][Square->Width - 1 - i];
    }
    printf("Suma de la diagonal secundaria: %d\n", sum);
}

void OnOrderChanged(GtkSpinButton* spinButton, gpointer UserData){
    
    int NewOrder = gtk_spin_button_get_value_as_int(spinButton);
    int OldOrder = 0;
    printf("GridUpdated: %d\n", NewOrder);
    if (GlobalSquare){
        OldOrder = GlobalSquare->Width;
        if (NewOrder == OldOrder) return;
        DestroySquare(GlobalSquare);
    }

    GlobalSquare = NewSquare(NewOrder, NewOrder);
    Print(GlobalSquare);
    /*
    gtk_widget_destroy(GlobalGrid);
    GlobalGrid = gtk_grid_new();
    */

    for (int i=OldOrder-1; i>=0; i--){
        printf("removed: %d\n", i);
        gtk_grid_remove_row(GlobalGrid, i);
        gtk_grid_remove_column(GlobalGrid, i);
    }

    for(int i = 0; i<NewOrder; i++)
    {
        printf("added: %d\n", i);
        gtk_grid_insert_column(GlobalGrid, i);
        gtk_grid_insert_row(GlobalGrid, i);
    }

    for(int i = 0; i < NewOrder; i++) {
        for(int j = 0; j < NewOrder; j++) {
            GtkLabel* Number = gtk_label_new("0");
            gtk_grid_attach(GlobalGrid, Number, j, i, 1, 1);
            //GtkWidget* Widget = (GTK_GRID(GlobalGrid), i, j);
        }
    }

    gtk_grid_set_row_homogeneous(GlobalGrid, true);
    gtk_grid_set_column_homogeneous(GlobalGrid, true);

    gtk_widget_queue_draw(GlobalGrid);

    //UpdateGrid(GlobalGrid, GlobalSquare);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));  // Inicializar la semilla del generador de números aleatorios
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new_from_file ("Proyecto0.glade");

    // Ventana principal
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "Window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //Grid
    Paned = GTK_PANED(gtk_builder_get_object(builder, "Paned"));
    GlobalGrid = GTK_GRID(gtk_builder_get_object(builder, "Grid"));

    //Order field
    GtkSpinButton* Order = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "Order"));
    gtk_spin_button_set_range(Order, 3, 21);
    g_signal_connect(Order, "value-changed", G_CALLBACK(OnOrderChanged), NULL);
    gtk_spin_button_set_value(Order, 5);

    //

    /*
    GlobalGrid = GTK_GRID(gtk_builder_get_object(builder, "Grid"));
    for(int i = 0; i<10; i++)
    {
        gtk_grid_insert_column(GTK_GRID(GlobalGrid), 1);
        gtk_grid_insert_row(GTK_GRID(GlobalGrid), 1);
    }

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            GtkWidget* Number = gtk_label_new("0");
            GtkWidget* Widget = (GTK_GRID(GlobalGrid), i, j);
            if (Widget){
                gtk_widget_destroy(Widget);
            }
            
            gtk_grid_attach(GTK_GRID(GlobalGrid), Number, i, j, 1, 1);
        }
    }
    */

   /*
    MagicSquare* Test = NewSquare(5,5);
    Test->CurrentSlot.x = 3;
    Test->CurrentSlot.y = 1;

    Print(Test);    

    for (int i = 1; i<=5*5; i++){
        printf("\n\n");
        ProcessNextMove(Test, &NorthWestMoveset, i);
        Print(Test);
    }

    CalculateSums(Test);
    UpdateGrid(GTK_GRID(Grid), Test);
    DestroySquare(Test);
    */

    gtk_widget_show_all(window);
    gtk_main();

    return 0;   
}