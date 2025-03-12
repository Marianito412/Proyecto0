#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>  // Para usar el tipo de dato 'bool'

#include "Core/MagicSquare/MagicSquare.c"
#include "Core/Moves/Moveset.c"

void Test(GtkSpinButton* Button, gpointer UserData){
    printf("Value Changed");
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
    for (int i = 0; i<Square->Height;i++){
        for(int j = 0; j<Square->Width;j++){
            GtkWidget* Test = gtk_grid_get_child_at(Grid, j, i);
            GtkSpinButton* Button = GTK_SPIN_BUTTON(Test);
            gtk_spin_button_set_value(Button, Square->Grid[i][j]);
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));  // Inicializar la semilla del generador de números aleatorios
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new_from_file ("Proyecto0.glade");

    // Ventana principal
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "Window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget* Grid = GTK_WIDGET(gtk_builder_get_object(builder, "Grid"));

    for(int i = 0; i<10; i++)
    {
        gtk_grid_insert_column(GTK_GRID(Grid), 1);
        gtk_grid_insert_row(GTK_GRID(Grid), 1);
    }

    for(int i = 0; i<5; i++)
    {
        for(int j = 0; j<5; j++){
            GtkWidget* Number = gtk_spin_button_new_with_range(0,10000,1);
            g_signal_connect(Number, "value-changed", G_CALLBACK(Test), NULL);
            gtk_grid_attach(GTK_GRID(Grid), Number, i, j, 1, 1);
        }
    }

    //Setup Movesets
    Moveset NorthWestMoveset = {NorthWest, NorthWestBr};
    Moveset SouthWestMoveset = {SouthWest, SouthWestBr};
    Moveset NorthEastLMoveset = {NorthEastL, NorthEeastLBr};

    MagicSquare* Test = NewSquare(5,5);
    Test->CurrentSlot.x = 1;
    Test->CurrentSlot.y = 1;

    Print(Test);    

    for (int i = 1; i<=5*5; i++){
        printf("\n\n");
        ProcessNextMove(Test, &NorthWestMoveset, i);
        Print(Test);
    }

    //UpdateGrid(GTK_GRID(Grid), Test);

    //gtk_widget_show_all(window);
    //gtk_main();

    free(Test);

    return 0;   
}