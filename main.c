#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>  // Para usar el tipo de dato 'bool'

#include "Core/MagicSquare/MagicSquare.c"

void Test(GtkSpinButton* Button, gpointer UserData){
    printf("Value Changed");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));  // Inicializar la semilla del generador de números aleatorios
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new_from_file ("Proyecto0.glade");

    if (!builder){
        printf("Builder failed");
    }

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

    MagicSquare* Test = NewSquare(5,5);
    Move(Test, -2, 2);
    SetCurrent(Test, 4);
    Print(Test);

    gtk_widget_show_all(window);
    gtk_main();

    free(Test);

    return 0;   
}