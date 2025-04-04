#pragma once

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

typedef struct {
    char* name;
    float price;
    char* state;
    int imageIndex; // Exemple: Rangée 1 = 0, 1, 2 -- Rangée 2 = 3, 4, 5 etc..
} Fruit;



void see_state_fruits(sqlite3* db,char* state_name);
int GetFruitCount(sqlite3* db);
int ReturnFruitIndexByName(const char* fruitName);
void LoadFruits(sqlite3* db, Fruit Fruits[50]);