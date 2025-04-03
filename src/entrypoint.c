#include "entrypoint.h"
#include "sqlite3.h"

/*
* On ajoute raylib pour creer la fenetre et raygui pour faire l'interface.
*/
#include "raylib.h"
#include "raygui.h"

#include <time.h>
#include <stdlib.h>
#include "Declarations.h"
#include <math.h>

void raylib_start(void){
    srand(time(NULL));
    sqlite3* db = NULL;
    sqlite3_open("./fruits.db",&db);

    int ROWS = 3;
    int COLUMNS = (int)ceil((float)GetFruitCount(db) / 3); //Convertir le nombre en float pour le ceil et ensuite le remettre en int

    InitWindow(720,480,"Fruits");
    SetTargetFPS(60);

    GuiLoadStyleDefault();

    while(!WindowShouldClose()){
        
        Image FruitImg = LoadImage("Fruits.png");
        Texture2D FruitSprite = LoadTextureFromImage(FruitImg);

        BeginDrawing();
        ClearBackground(BLACK);

        Vector2 cursor = {0};

        EndDrawing();

        UnloadTexture(FruitSprite);
        UnloadImage(FruitImg);

    }
    CloseWindow();

    // see_state_fruits(db,"FL");
    // see_state_fruits(db,"CA");
    // see_state_fruits(db,"NC");
    // add_random_fruit(db); //Uncomment to add a random fruit...
    sqlite3_close(db);
    return;
}