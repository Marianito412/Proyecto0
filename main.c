#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>  // Para usar el tipo de dato 'bool'

#include "Core/MagicSquare/MagicSquare.c"
#include "Core/Moves/Moveset.c"

int CurrentNumber = 1;

Moveset NorthWestMoveset = {NorthWest, NorthWestBr};
Moveset NorthEastMoveset = {NorthEast, NorthEastBr};
Moveset SouthWestMoveset = {SouthWest, SouthWestBr};
Moveset SouthEastMoveset = {SouthEast, SouthEastBr};
Moveset NorthEastLMoveset = {NorthEastL, NorthEeastLBr};

MagicSquare* GlobalSquare = NULL;
Moveset* CurrentMoveset = &NorthWestMoveset;

GtkPaned* Paned = NULL;
GtkGrid* GlobalGrid = NULL;

GtkLabel* sums = NULL;  // Etiqueta para mostrar todas las sumas

// Función para actualizar las sumas en la interfaz
void UpdateSums(MagicSquare* Square) {
    int sum = 0;
    int magicConstant = Square->Width * (Square->Width * Square->Width + 1) / 2;

    // Buffer para almacenar el texto de las sumas
    char sumsText[1000] = "";  // Ajusta el tamaño según sea necesario
    char buffer[100];  // Buffer temporal para formatear cada suma

    // Sumar filas
    strcat(sumsText, "Sumas de filas:\n");
    for (int i = 0; i < Square->Height; i++) {
        sum = 0;
        for (int j = 0; j < Square->Width; j++) {
            sum += Square->Grid[i][j];
        }
        snprintf(buffer, sizeof(buffer), "Fila %d: %d\n", i + 1, sum);
        strcat(sumsText, buffer);
    }

    // Sumar columnas
    strcat(sumsText, "\nSumas de columnas:\n");
    for (int j = 0; j < Square->Width; j++) {
        sum = 0;
        for (int i = 0; i < Square->Height; i++) {
            sum += Square->Grid[i][j];
        }
        snprintf(buffer, sizeof(buffer), "Columna %d: %d\n", j + 1, sum);
        strcat(sumsText, buffer);
    }

    // Sumar diagonal principal
    sum = 0;
    for (int i = 0; i < Square->Height; i++) {
        sum += Square->Grid[i][i];
    }
    snprintf(buffer, sizeof(buffer), "\nSuma de la diagonal principal: %d\n", sum);
    strcat(sumsText, buffer);

    // Sumar diagonal secundaria
    sum = 0;
    for (int i = 0; i < Square->Height; i++) {
        sum += Square->Grid[i][Square->Width - 1 - i];
    }
    snprintf(buffer, sizeof(buffer), "Suma de la diagonal secundaria: %d\n", sum);
    strcat(sumsText, buffer);

    // Actualizar el texto del GtkLabel
    gtk_label_set_text(sums, sumsText);
}


void Test(GtkSpinButton* Button, gpointer UserData){
    printf("Value Changed");
}

Moveset* GetMoveset(gchararray moveset){
    if (strcmp(moveset, "NorthWest") == 0) return &NorthWestMoveset;
    if (strcmp(moveset, "NorthEast") == 0) return &NorthEastMoveset;
    if (strcmp(moveset, "SouthWest") == 0) return &SouthWestMoveset;
    if (strcmp(moveset, "SouthEast") == 0) return &SouthEastMoveset;
    if (strcmp(moveset, "NorthEastL") == 0) return &NorthEastLMoveset;
    printf("Failed to select moveset");
}

void ProcessNextMove(MagicSquare* Square, Moveset* MoveSet, int i){
    if (i > Square->Width* Square->Width) return;
    SetCurrent(Square, i);
    Coordinate NewCoords = Square->CurrentSlot;
    MoveSet->Move(Square, &NewCoords);

    if (GetValueAtCoords(Square, NewCoords) == 0){
        MoveSet->Move(Square, &Square->CurrentSlot);
    }
    else{
        MoveSet->BreakMove(Square, &Square->CurrentSlot);
    }
    printf("(%d, %d) -> %d\n", Square->CurrentSlot.x, Square->CurrentSlot.y, i);
}

void UpdateGrid(GtkGrid* Grid, MagicSquare* Square){
    for (int i = 0; i < Square->Height; i++) {

        for(int j = 0; j < Square->Width; j++) {

            GtkWidget* widget = gtk_grid_get_child_at(Grid, j, i);
            if (GTK_IS_LABEL(widget)) {
                char buffer[10];
                snprintf(buffer, sizeof(buffer), "%d", Square->Grid[j][i]);
                printf("(%d, %d) -> %d\n", j, i, Square->Grid[j][i]);
                gtk_label_set_text(GTK_LABEL(widget), buffer);
            }
        }
    }
}


void OnOrderChanged(GtkSpinButton* spinButton, gpointer UserData){
    int new_size = gtk_spin_button_get_value_as_int(spinButton);
    RegenerateGrid(new_size);
}

void RegenerateGrid(int new_size){
    CurrentNumber = 1;
    
    if (GlobalSquare != NULL) {
        DestroySquare(GlobalSquare);
        GlobalSquare = NULL;
    }

    GlobalSquare = NewSquare(new_size, new_size);

    GList* children;
    GList* iter;
    children = gtk_container_get_children(GTK_CONTAINER(GlobalGrid));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);


    for (int i = 0; i < GlobalSquare->Height; i++) {
        for (int j = 0; j < GlobalSquare->Width; j++) {
            char buffer[10];
            snprintf(buffer, sizeof(buffer), "%d", 0); 
            GtkWidget *label = gtk_label_new(buffer);

            PangoAttrList* Attrs = pango_attr_list_new();
            PangoAttribute* font_desc = pango_attr_scale_new(2);
            pango_attr_list_insert(Attrs, font_desc);

            gtk_label_set_attributes(GTK_LABEL(label), Attrs);
            
            gtk_grid_attach(GTK_GRID(GlobalGrid), label, j, i, 1, 1);
        }
    }


    gtk_widget_show_all(GTK_WIDGET(GlobalGrid));

    GlobalSquare->CurrentSlot.y = 0;
    GlobalSquare->CurrentSlot.x = (GlobalSquare->Width/2);
}

static void OnNextPressed(GtkButton* Button, gpointer UserData){
    g_print("Test");
    ProcessNextMove(GlobalSquare, CurrentMoveset, CurrentNumber);
    CurrentNumber++;
    UpdateGrid(GlobalGrid, GlobalSquare);
    UpdateSums(GlobalSquare);  

}

static void OnCompletePressed(GtkButton* Button, gpointer UserData){
    while (CurrentNumber <= GlobalSquare->Width*GlobalSquare->Width){
        ProcessNextMove(GlobalSquare, CurrentMoveset, CurrentNumber);
        CurrentNumber++;
    }
    UpdateGrid(GlobalGrid, GlobalSquare);
    UpdateSums(GlobalSquare);  


    Print(GlobalSquare);
}

void OnMovesetChanged(GtkComboBoxText* ComboBox){
    CurrentMoveset = GetMoveset(gtk_combo_box_text_get_active_text(ComboBox));
    g_print("New Moveset: %s", gtk_combo_box_text_get_active_text(ComboBox));

    RegenerateGrid(GlobalSquare->Width);
}

void SetGridAttributes(GtkGrid* Grid, MagicSquare* Square){
    for (int i = 0; i < Square->Height; i++) {
        for(int j = 0; j < Square->Width; j++) {
            GtkWidget* widget = gtk_grid_get_child_at(Grid, j, i);

            if (GTK_IS_LABEL(widget)) {
                PangoAttrList *const Attrs = pango_attr_list_new();

                PangoFontDescription * font_desc = pango_font_description_new();
                pango_font_description_set_size(font_desc, 36 * PANGO_SCALE);
                PangoAttribute * attr = pango_attr_font_desc_new(font_desc);
                pango_attr_list_insert(Attrs, font_desc);
                gtk_label_set_attributes(GTK_LABEL(widget), Attrs);

                //char buffer[10];
                //snprintf(buffer, sizeof(buffer), "%d", Square->Grid[j][i]);
                //printf("(%d, %d) -> %d\n", j, i, Square->Grid[j][i]);
                //gtk_label_set_text(GTK_LABEL(widget), buffer);
            }
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

    sums = GTK_LABEL(gtk_builder_get_object(builder, "sums"));

    //Grid
    Paned = GTK_PANED(gtk_builder_get_object(builder, "Paned"));
    GlobalGrid = GTK_GRID(gtk_builder_get_object(builder, "Grid"));

    //Order field
    GtkSpinButton* Order = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "Order"));
    gtk_spin_button_set_range(Order, 3, 21);
    g_signal_connect(Order, "value-changed", G_CALLBACK(OnOrderChanged), NULL);

    gtk_spin_button_set_range(GTK_SPIN_BUTTON(Order), 3, 21);
    gtk_spin_button_set_increments(GTK_SPIN_BUTTON(Order), 1, 5);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(Order), 5);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(Order), TRUE);
    gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(Order), FALSE);
    gtk_spin_button_set_update_policy(GTK_SPIN_BUTTON(Order), GTK_UPDATE_ALWAYS);

    //Moveset
    GtkComboBoxText* MovesetBox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "Moveset"));
    g_signal_connect(MovesetBox, "changed", G_CALLBACK(OnMovesetChanged), NULL);


    //Buttons
    GtkButton* Next = GTK_BUTTON(gtk_builder_get_object(builder, "NextButton"));
    g_signal_connect(Next, "clicked", G_CALLBACK(OnNextPressed), NULL);

    GtkButton* Complete = GTK_BUTTON(gtk_builder_get_object(builder, "CompleteButton"));
    g_signal_connect(Complete, "clicked", G_CALLBACK(OnCompletePressed), NULL);

    UpdateSums(GlobalSquare);


    GlobalSquare->CurrentSlot.y = 0;
    GlobalSquare->CurrentSlot.x = (GlobalSquare->Width/2);
    Print(GlobalSquare);

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