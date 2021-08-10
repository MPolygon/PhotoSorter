#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sqlite3.h>
#include "sqlite_utils.h"
#include "utils.h"
#include "folder_utils.h"


// Defined headers for specific image formats
int png_header[8] = {137, 80, 78, 71, 13, 10, 26, 10};
int jpeg_header[2] = {255, 216};

char fileTypes[3][8] = {"Unknown", "PNG", "JPEG"};


// Scans a specified folders, and adds any supported images to the DB
int ScanFolder(sqlite3 *db, char *folder) {
    // Open folder

    struct dirent *dp;
    DIR *dfd;

    if ((dfd = opendir(folder)) == NULL) {
        fprintf(stderr, "Can't open %s\n", folder);
        return 1;
    }

    // Loop through files

    while ((dp = readdir(dfd)) != NULL) {
        if (dp->d_type == DT_REG) {
            printf("File found: %s\n", dp->d_name);
            int formatIndex = TestFormat(folder, dp->d_name);
            long int fileSize = GetFileSize(folder, dp->d_name);

            printf("File size: %ld B\n", fileSize);
            printf("Type: %s\n", fileTypes[formatIndex]);
            
            if (formatIndex) {
                printf("--------Updating DB---------\n");
                UpdateDatabase(db, "PHOTO", dp->d_name, fileTypes[formatIndex], fileSize);
            }

        }
    }

    closedir(dfd);
    

    return 0;
}


// Look through header to determine the file format (needs optimization)
int TestFormat(char *path, char *filename) {
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


// Get file size
long int GetFileSize(char *path, char *filename) {
    FILE *fp;
    char fullPath[strlen(path) + strlen(filename) + 1];

    strcpy(fullPath, path);
    strcat(fullPath, "/");
    strcat(fullPath, filename);


    fp = fopen(fullPath, "rb");
    if (fp == NULL) {
        fprintf(stderr, "cannot open input file\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    
    long int size = ftell(fp);


    fclose(fp);
    
    return size;
}

// Get full path for a given directory and file name
void GetFullPath(char *OUTPUT, char *path, char *filename) {
    strcpy(OUTPUT, path);
    strcat(OUTPUT, "/");
    strcat(OUTPUT, filename);
}