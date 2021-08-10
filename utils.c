#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sqlite3.h>
#include "utils.h"


char* Concat(const char *s1, const char *s2) // Utility function, could be replaced by the use of sprintf() later
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}