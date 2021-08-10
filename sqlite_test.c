#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sqlite3.h>
#include "sqlite_utils.h"
#include "utils.h"


// Get the number of rows in specified SQL table
int TableCount(sqlite3 *db, char *tableName) {
    sqlite3_stmt *res;
    char *sqlTemplate = "SELECT count(*) FROM ";
    char *sql;
    int rc;
    int output = -1;
    
    
    sql = Concat(sqlTemplate, tableName);
    
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    
    rc = sqlite3_step(res);
    
    if (rc == SQLITE_ROW) {
        output = sqlite3_column_int(res, 0);
    }
    
    sqlite3_finalize(res);
    free(sql);
    
    return output;
}


// Create a record for a given image file in the specified table
int CreateRecord(sqlite3 *db, char *tableName, char *filename, char *fileType, long int fileSize) {
    char *sql;
    char *zErrMsg = 0;
    int rc;
    
    sql = Concat("INSERT INTO ", tableName);
    sql = Concat(sql, " (FILENAME, FILETYPE, FILESIZE) VALUES('");
    sql = Concat(sql, filename);
    sql = Concat(sql, "', '");
    sql = Concat(sql, fileType);
    
    char size[20];
    sprintf(size, "%ld", fileSize);
    
    sql = Concat(sql, "', ");
    sql = Concat(sql, size);
    sql = Concat(sql, ");");
    
    printf("%s\n", sql);
    
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    
    
    
    free(sql);
    
    return 0;
}



// Check if a file is in the DB already, and creates a record otherwise
int UpdateDatabase(sqlite3 *db, char *tableName, char *filename, char *fileType, long int fileSize) {
    if (!TableCount(db, tableName)) {
        printf("Debug..................\n");
        CreateRecord(db, tableName, filename, fileType, fileSize);
        return 0;
    }
    
    char *sql;
    sqlite3_stmt *res;
    int rc;
    
    sql = Concat("SELECT FILENAME FROM ", tableName);
    sql = Concat(sql, ";");
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);    
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }    
    
    do {
        rc = sqlite3_step(res);
        char *row_filename = 0;
        sprintf(row_filename, "%s", sqlite3_column_text(res, 0));
        // Compare Strings
        if (!strcmp(filename, row_filename)) {
            printf("File in DB\n");
            sqlite3_finalize(res);
            free(sql);
            return 0;
        }
        free(row_filename);
        
    } while (rc == SQLITE_ROW);
    
    CreateRecord(db, tableName, filename, fileType, fileSize);
    
    sqlite3_finalize(res);
    
    free(sql);
    
    
    return 0;
}