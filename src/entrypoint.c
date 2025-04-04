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

Fruit Fruits[50];

void raylib_start(void){
    srand(time(NULL));
    sqlite3* db = NULL;
    sqlite3_open("./fruits.db",&db);

    int ROWS = 3;
    int COLUMNS = (int)ceil((float)GetFruitCount(db) / 3); //Convertir le nombre en float pour le ceil et ensuite le remettre en int
    int Width = 720;
    int Height = 480;
    int fruitCount = GetFruitCount(db);

    LoadFruits(db, Fruits);

    InitWindow(Width,Height,"Fruits");
    SetTargetFPS(60);

    GuiLoadStyleDefault();

    while(!WindowShouldClose()){
        
        Image FruitImg = LoadImage("Fruits.png");
        Texture2D FruitSprite = LoadTextureFromImage(FruitImg);
        int FruitSpriteSize = 32;

        BeginDrawing();
        ClearBackground(BLACK);

        Vector2 cursor = {0};

        int ColumnWidth = Width / COLUMNS;
        int RowHeight = Height / ROWS;

            for (int i = 0; i < fruitCount; i++) {
                Fruit CurrentFruit = Fruits[i];

                int row = i / COLUMNS;
                int col = i % COLUMNS;

                Rectangle src = {
                    (CurrentFruit.imageIndex % 4) * FruitSpriteSize, 
                    (CurrentFruit.imageIndex / 4) * FruitSpriteSize, 
                    FruitSpriteSize, 
                    FruitSpriteSize
                };

                Rectangle dst = {
                    col * ColumnWidth, 
                    row * RowHeight, 
                    RowHeight, 
                    RowHeight - 50
                };

                DrawTexturePro(FruitSprite, src, dst, CLITERAL(Vector2){0, 0}, 0.0f, WHITE);
                DrawText("Price:", col * ColumnWidth, row * RowHeight, 25, WHITE);

            }

        EndDrawing();

        UnloadTexture(FruitSprite);
        UnloadImage(FruitImg);

    }
    CloseWindow();

    // see_state_fruits(db,"FL");
    // see_state_fruits(db,"CA");
    // see_state_fruits(db,"NC");
    sqlite3_close(db);
    return;
}