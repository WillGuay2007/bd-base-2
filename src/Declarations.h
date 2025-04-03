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



void see_state_fruits(sqlite3* db,char* state_name);
void add_random_fruit(sqlite3* db);
int GetFruitCount(sqlite3* db);