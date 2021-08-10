#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sqlite3.h>

int TableCount(sqlite3 *db, char *tableName);

int CreateRecord(sqlite3 *db, char *tableName, char *filename, char *fileType, long int fileSize);

int UpdateDatabase(sqlite3 *db, char *tableName, char *filename, char *fileType, long int fileSize);