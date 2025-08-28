#include "comm_treatment.h"
#include <stdlib.h>
#include <string.h>

void free_command_struct(CommandStruct *cmd){
    if(!cmd || !cmd->stringArray) return;
    for(int i = 0; cmd->stringArray[i]; i++){
        free(cmd->stringArray[i]); // free strdup’d string
    }
    free(cmd->stringArray);
}

CommandStruct command_separation(char** commArray, char** dirArray, int *index){
    CommandStruct commStrc;
    commStrc.background = 0; // default
    commStrc.modifier = 0; // default
    if(!strcmp(commArray[*index], "&&")) commStrc.modifier = 1, (*index)++;
    else if(!strcmp(commArray[*index], "||")) commStrc.modifier = 2, (*index)++;
    else if(!strcmp(commArray[*index], ";")) commStrc.modifier = 0, (*index)++;
    int idealSize = 5, indexArray = 0;
    commStrc.stringArray = malloc(sizeof(char*) * idealSize);
    for(;commArray[*index] && strcmp(commArray[*index], "&&") && strcmp(commArray[*index], "||") && strcmp(commArray[*index], "&") && strcmp(commArray[*index], ";"); (*index)++){
        commStrc.stringArray[indexArray++] = strdup(commArray[*index]);
        if(indexArray >= idealSize){
            idealSize *= 2;
            commStrc.stringArray = realloc(commStrc.stringArray, sizeof(char*) * idealSize);
        }
    }
    commStrc.stringArray[indexArray] = NULL;
    if(commArray[*index] && !strcmp(commArray[*index], "&")) commStrc.background = 1, (*index)++;
    commStrc.directories = dirArray; 
    return commStrc;
}

CommandStruct* command_treatment(char** commArray, char** dirArray){
    int idealSize = 4, indexArray = 0, index = 0;;
    CommandStruct *arrayTreated = malloc(sizeof(CommandStruct) * 4);
    CommandStruct command;
    while(commArray[index]){
        command = command_separation(commArray, dirArray, &index);
        arrayTreated[indexArray++] = command;
        if(indexArray == idealSize){
            idealSize *= 2;
            arrayTreated = realloc(arrayTreated, sizeof(CommandStruct) * idealSize);
        }
    }
    CommandStruct end = {0};
    end.stringArray = NULL;
    arrayTreated[indexArray] = end;
    return arrayTreated;
}
