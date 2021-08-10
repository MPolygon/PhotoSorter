#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sqlite3.h>
#include "sqlite_utils.h"
#include "utils.h"

int ScanFolder(sqlite3 *db, char *folder);
int TestFormat(char *path, char *filename);
long int GetFileSize(char *path, char *filename);
void GetFullPath(char *OUTPUT, char *path, char *filename);