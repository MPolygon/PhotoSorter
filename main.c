#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sqlite3.h>


int png_header[8] = {137, 80, 78, 71, 13, 10, 26, 10};
int jpeg_header[2] = {255, 216};

char fileTypes[3][8] = {"Unknown", "PNG", "JPEG"};

char dbFile[] = "test.db";
char testFolder[] = "/home/mstrpolygon";
// char TestFormat[] = "/mnt/d/Blender/Textures/drive-download-20210427T201332Z-001";


void ScanFolder(char folder[]);
int TestFormat(char path[], char filename[]);
void UpdateDatabase(char path[], char filename[]);

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i=0; i<argc, i++;) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    return  0;
}

int main() {
    
    /* FILE *fp;
    int c, i, max;
    int header[8];

    int png_header[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    int jpeg_header[2] = {255, 216};

    // File: /mnt/d/Blender/Textures/bark_brown_02_2k_jpg/bark_brown_02_diff_2k.jpg
    // File: /mnt/d/Blender/Textures/drive-download-20210427T201332Z-001/Lace_001_height.png

    fp = fopen("/mnt/d/Blender/Textures/drive-download-20210427T201332Z-001/Lace_001_height.png", "rb");
    if (fp == NULL) {
        fprintf(stderr, "cannot open input file\n");
        return 1;
    }
    for (i = 0, max = 8; i < max && (c = getc(fp)) != EOF; i++) {
        header[i] = c;
        printf("%02x/%d\n", c, header[i]);
        // printf("%d", c);
        if (i % 16 == 15)
            putchar('\n');  // 16 bytes per line
        // else if (i % 2 == 1)
        else {
            putchar(' ');   // group bytes in pairs
        }
    }
    if (i % 16 != 0)
        putchar('\n');  // output a newline if needed

    fclose(fp);*/ 

    // ScanFolder(testFolder);
    
    UpdateDatabase(NULL, NULL);

    return 0;
}

void ScanFolder(char folder[]) {
    // Open folder

    struct dirent *dp;
    DIR *dfd;

    if ((dfd = opendir(folder)) == NULL) {
        fprintf(stderr, "Can't open %s\n", folder);
        return;
    }

    // Loop through files

    while ((dp = readdir(dfd)) != NULL) {
        if (dp->d_type == DT_REG) {
            printf("File found: %s\n", dp->d_name);
            int formatIndex = TestFormat(folder, dp->d_name);
            printf("File format index: %d\n", formatIndex);
            printf("Type: %s\n", fileTypes[formatIndex]);
            
            // UpdateDatabase(folder, dp->d_name);
        }
    }


    // Test for file format
    // Add to DB

    // Close folder
}


int TestFormat(char path[], char filename[]) {
    FILE *fp;
    int c, i, max;
    int header[8];
    char fullPath[strlen(path) + strlen(filename) + 1];

    strcpy(fullPath, path);
    strcat(fullPath, "/");
    strcat(fullPath, filename);


    fp = fopen(fullPath, "rb");
    if (fp == NULL) {
        fprintf(stderr, "cannot open input file\n");
        return 0;
    }

    for (i = 0, max = 8; i < max && (c = getc(fp)) != EOF; i++) {
        header[i] = c;
    }

    //Test fo JPEG
    if (header[0] == 255 && header[1] == 216) {
        return 2;
    }

    for (int i = 0; i < 8; i++) {
        if (header[i] != png_header[i]) {
            break;
        }

        return 1;
    } 


    fclose(fp);
    return 0;
}

void UpdateDatabase(char path[], char filename[]) {
    sqlite3  *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    
    rc = sqlite3_open(dbFile, &db);
    
    if (rc) {
        printf("Can't open  database\n");
        return;
    } else {
        printf("Opened database successfully\n");
    }
    
    sql = "CREATE TABLE PHOTO(" \
    "ID INT PRIMARY KEY NOT NULL," \
    "FILENAME TEXT NOT NULL," \
    "TYPE TEXT NOT NULL," \
    "SIZE INT NOT NULL);";
    
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if (rc != SQLITE_OK) {
        printf("SQL Error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("Table created successfully\n");
    }
    
    
    
    
    printf("Closing database\n");
    sqlite3_close(db);
    
}