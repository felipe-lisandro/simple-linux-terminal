#ifndef COMM_TREATMENT_H
#define COMM_TREATMENT_H

typedef struct CommandStruct {
    char** stringArray;
    char** directories;
    int background; // 1 = run in background
    int modifier;
    /*
    0 = execute without worries
    1 = execute if previous was successful
    2 = execute if previous wasn't successful
    */
} CommandStruct;

CommandStruct* command_treatment(char** commArray, char** dirArray);

void free_command_struct(CommandStruct *cmd);

#endif