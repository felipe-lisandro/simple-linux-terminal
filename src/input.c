#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

#define INPUT_SIZE 1024

// returns a null-termined array of strings 
char **get_array_tokenized(char* source, char strSeparation){
    char strDelim[2] = {strSeparation, '\0'};
    int idealSize = 16;
    char **tokenizedArray = malloc(sizeof(char*) * idealSize);
    if(!tokenizedArray) return NULL;
    int c = 0;
    char *token = strtok(source, strDelim);
    while(token){
        if(c >= idealSize){
            idealSize *= 2;
            tokenizedArray = realloc(tokenizedArray, sizeof(char*) * idealSize);
        }
        tokenizedArray[c] = strdup(token);
        token = strtok(NULL, strDelim);
        c++;
    }
    tokenizedArray[c] = NULL;
    return tokenizedArray;
}

char **get_sys_paths(void){
    char *sysPath = getenv("PATH");
    if(!sysPath) return NULL;
    char *sysPathCopy = strdup(sysPath);
    if(!sysPathCopy) return NULL;
    char **arrayDir = get_array_tokenized(sysPathCopy, ':');
    free(sysPathCopy);
    return arrayDir;
}

char **input_com(void){
    char input[INPUT_SIZE];
    if (!fgets(input, sizeof(input), stdin)) return NULL;
    int inputSize = strlen(input);
    if(inputSize > 0 && input[inputSize - 1] == '\n') input[inputSize - 1] = '\0';
    char **arrayCom = get_array_tokenized(input, ' ');
    return arrayCom;
}