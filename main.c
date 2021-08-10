#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sqlite3.h>
#include "sqlite_utils.h"
#include "folder_utils.h"




char dbFile[] = "test.db";
char testFolder[] = "/home/mstrpolygon";
// char TestFormat[] = "/mnt/d/Blender/Textures/drive-download-20210427T201332Z-001";

// Database functions
void CleanDatabase();
void RefreshDatabase();


//Utils




int main() {
    

    // ScanFolder(testFolder);
    sqlite3 *db;
    sqlite3_open("test.db", &db);
    
    // int count = TableCount(db, "PHOTO");
    ScanFolder(db, testFolder);
    
    sqlite3_close(db);
    // printf("%d\n", count);

    return 0;
}


// File functions definitions



// Database functions definitions


// Utils functions definitions

