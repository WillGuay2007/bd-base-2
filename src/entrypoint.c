#include "entrypoint.h"
#include "sqlite3.h"

/*
* On ajoute raylib pour creer la fenetre et raygui pour faire l'interface.
*/
#include "raylib.h"
#include "raygui.h"

#include <time.h>
#include <stdlib.h>

#define LOG_SQLITE3_ERROR(db) TraceLog(LOG_ERROR, "%s:%d: SQLITE3 ERROR: %s\n", __FILE__, __LINE__, sqlite3_errmsg(db))
#define NUMBER_OF_FRUITS = 9
#define ROWS 3
#define COLUMNS 3

void see_state_fruits(sqlite3* db,char* state_name){
    sqlite3_stmt* stmt = NULL;
    char content[] =
    "SELECT fruit,price\n"
    "FROM FruitsForSale\n"
    "WHERE etat=?;";// <---------------IMPORTANT: Le ? seras défini plus tard avec la fonction bind_text (ligne 33)
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);

    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }

    if(sqlite3_bind_text(stmt,1,state_name,-1,SQLITE_STATIC) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }

    for(int ret = sqlite3_step(stmt);ret != SQLITE_DONE;ret = sqlite3_step(stmt)){
        if(ret != SQLITE_ROW){
            LOG_SQLITE3_ERROR(db);
        }
        int column = 0;
        const unsigned char* fruit = sqlite3_column_text(stmt,column++);
        float price = sqlite3_column_double(stmt,column);
        TraceLog(LOG_INFO,"Etat %s a le fruit %s avec le prix %.2f$",state_name,fruit,price);
    }

    sqlite3_finalize(stmt);
}
const char* fruits[] = {
    "Banana",
    "Orange",
    "Grape",
    "Pear",
    "Kumqat"
};
const char* states[] = {
    "CA",
    "SC",
    "NC",
    "FL",
    "HA"
};
void add_random_fruit(sqlite3* db){
    /*
        Commence une TRANSACTIONS SQL. sqlite3_exec permet d'exécuter
        une requête SQL qui n'a pas à être préparée vu quel n'a pas de valeur à recevoir du programme C.
    */
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return;
    }
    sqlite3_stmt* stmt = NULL;
    char content[] =
    "INSERT INTO FruitsForSale(fruit,etat,price)\n"
    "VALUES\n"
    "(?,?,?);";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    int fruit = rand() %5;
    int state = rand() %5;
    double price = ((rand()+1) % 100) /100 + rand() % 2;
    if(sqlite3_bind_text(stmt,1,fruits[fruit],-1,SQLITE_STATIC) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_text(stmt,2,states[state],-1,SQLITE_STATIC) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_double(stmt,3,price) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    ret = sqlite3_step(stmt);
    /*
        Ici on fait encore une requête SQL qui roule le SQL directement,
        mais maintenant on termine la TRANSACTION avec le COMMIT.
    */
    if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }
}

void raylib_start(void){
    srand(time(NULL));
    sqlite3* db = NULL;
    sqlite3_open("./fruits.db",&db);

    InitWindow(720,480,"Fruits");
    SetTargetFPS(60);

    GuiLoadStyleDefault();

    while(!WindowShouldClose()){
        
        Image FruitImg = LoadImage("Fruits.png");
        Texture2D FruitSprite = LoadTextureFromImage(FruitImg);

        BeginDrawing();
        ClearBackground(BLACK);

        Vector2 cursor = {0};

        for (int row = 1; row <= ROWS; row++) {
            for (int col = 1; col <= COLUMNS; col++) {
                int StepsCols = 128 / 4;
                int StepsRows = 128 / 4;
                int ColumnSize = 720/3;
                int RowSize = 480 / 3;
                int FruitRowSize = RowSize - 50;
                int InfoRowSize = RowSize - FruitRowSize;
                Rectangle src = {StepsCols * (col - 1), StepsRows * (row - 1), 32, 32};
                Rectangle dest = {cursor.x, cursor.y, ColumnSize, FruitRowSize};
                DrawTexturePro(FruitSprite, src, dest, CLITERAL(Vector2){0}, 0, WHITE);
                DrawText("Prix:", cursor.x, cursor.y + FruitRowSize, 30, WHITE);
                cursor.x += ColumnSize;
                if (col == 3) {cursor.y += RowSize; cursor.x = 0;}; //Changement de rangée
            }
        }

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