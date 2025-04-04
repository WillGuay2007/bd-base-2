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
#include <string.h>

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
int GetFruitCount(sqlite3* db)  {
    sqlite3_stmt* stmt = NULL;
    char content[] = "SELECT COUNT(*) FROM (SELECT * FROM FruitsForSale GROUP BY fruit);";
    if (sqlite3_prepare_v2(db,content,-1,&stmt,NULL) != SQLITE_OK) {LOG_SQLITE3_ERROR(db); sqlite3_finalize(stmt); return 0;}
    sqlite3_step(stmt);
    int Quantite = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return Quantite;
}
int ReturnFruitIndexByName(const char* fruitName) {
    int imageIndex = 0;
    if (strcmp(fruitName, "Apple") == 0) imageIndex = 0;
    else if (strcmp(fruitName, "Pear") == 0) imageIndex = 1;
    else if (strcmp(fruitName, "Lemon") == 0) imageIndex = 2;
    else if (strcmp(fruitName, "Orange") == 0) imageIndex = 3;
    else if (strcmp(fruitName, "Plum") == 0) imageIndex = 4;
    else if (strcmp(fruitName, "Kumqat") == 0) imageIndex = 5;
    else if (strcmp(fruitName, "Pomegranate") == 0) imageIndex = 6;
    else if (strcmp(fruitName, "Strawberry") == 0) imageIndex = 7;
    else if (strcmp(fruitName, "Grape") == 0) imageIndex = 8;
    else if (strcmp(fruitName, "Cherry") == 0) imageIndex = 9;
    else if (strcmp(fruitName, "Peach") == 0) imageIndex = 10;
    else if (strcmp(fruitName, "Raspberry") == 0) imageIndex = 11;

    return imageIndex;
}
void LoadFruits(sqlite3* db, Fruit Fruits[50]) {
    sqlite3_stmt* stmt = NULL;
    char content[] = "SELECT * FROM FruitsForSale GROUP BY fruit;";
    if (sqlite3_prepare_v2(db,content,-1,&stmt,NULL) != SQLITE_OK) {LOG_SQLITE3_ERROR(db); sqlite3_finalize(stmt); return;}

    int Counter = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        
        const char* fruitName = (const char*)sqlite3_column_text(stmt, 0);
        float price = sqlite3_column_int(stmt, 1);
        const char* etat = (const char*)sqlite3_column_text(stmt, 2);
        int ImageIndex = ReturnFruitIndexByName(fruitName);
        
        Fruits[Counter].name = strdup(fruitName);
        Fruits[Counter].price = price;
        Fruits[Counter].state = strdup(etat);
        Fruits[Counter].imageIndex = ImageIndex;

        Counter++;
    }
}