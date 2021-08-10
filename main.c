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




int main() {
    

    sqlite3 *db;
    sqlite3_open("test.db", &db);
    
    ScanFolder(db, testFolder);
    
    sqlite3_close(db);

    return 0;
}
