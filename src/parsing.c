#include "parsing.h"
#include <string.h>
#include <stdlib.h>

char *chains[] = {"&&", "||", ";", NULL}; // missing the '|'

char **getCommandString(char **arrayCom, int initialIndex, int finalIndex) {
    int len = finalIndex - initialIndex + 1;
    char **retString = malloc((len + 1) * sizeof(char*));

    for(int i = 0; i < len; i++){
        retString[i] = strdup(arrayCom[initialIndex + i]);
    }
    retString[len] = NULL;
    return retString;
}

commNode* parsing(char **arrayCom) {
    int size = 4, indexFinal = 0, monitor = 0;
    commNode *finalArray = malloc(sizeof(commNode) * size);
    for (int i = 0; arrayCom[i]; i++) {
        for (int c = 0; chains[c]; c++) {
            if (strcmp(arrayCom[i], chains[c]) == 0){
                if(strcmp(chains[c], "&&") == 0){
                    finalArray[indexFinal].conditional = 1;
                    finalArray[indexFinal].comm = getCommandString(arrayCom, monitor, i - 1);
                    finalArray[indexFinal].background = 0;
                }
                else if(strcmp(chains[c], "||") == 0){
                    finalArray[indexFinal].conditional = -1;
                    finalArray[indexFinal].comm = getCommandString(arrayCom, monitor, i - 1);
                    finalArray[indexFinal].background = 0;
                }
                else if(strcmp(chains[c], "&") == 0){
                    finalArray[indexFinal].conditional = 0;
                    finalArray[indexFinal].comm = getCommandString(arrayCom, monitor, i - 1);
                    finalArray[indexFinal].background = 1;
                }
                else{
                    finalArray[indexFinal].conditional = 0;
                    finalArray[indexFinal].comm = getCommandString(arrayCom, monitor, i - 1);
                    finalArray[indexFinal].background = 0;
                }
                monitor = i + 1;
                indexFinal++;
            }
        }
    }
    if (monitor < size) {
        finalArray[indexFinal].conditional = 0; // last command has no chain operator
        finalArray[indexFinal].comm = getCommandString(arrayCom, monitor, size - 1);
        indexFinal++;
    }
    return finalArray;
}
