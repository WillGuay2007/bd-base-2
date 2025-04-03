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
int GetFruitCount(sqlite3* db)  {
    sqlite3_stmt* stmt = NULL;
    char content[] = "SELECT COUNT(*) FROM (SELECT * FROM FruitsForSale GROUP BY fruit);";
    if (sqlite3_prepare_v2(db,content,-1,&stmt,NULL) != SQLITE_OK) {LOG_SQLITE3_ERROR(db); sqlite3_finalize(stmt); return 0;}
    sqlite3_step(stmt);
    int Quantite = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return Quantite;
}