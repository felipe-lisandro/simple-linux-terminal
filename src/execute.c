#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "execute.h"
#include "handler.h"

int execute(char **arrayDir, char **arrayCom, int background){
    int executed = 0;
    for(int i = 0; arrayDir[i]; i++){
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", arrayDir[i], arrayCom[0]);
        if(access(fullpath, X_OK) == 0){
            pid_t pid = fork();
            if(pid == 0){ // child
                execvp(fullpath, arrayCom);
                perror("execv failed");
                exit(EXIT_FAILURE);
            }
            else if(!background){
                waitpid(pid, NULL, 0);
            }
            executed = 1;
            break;
        }
    }
    if(!executed){
        printf("Command not found: %s\n", arrayCom[0]);
    }
    return executed;
}

// will try to implment the chain command options (&& || ;)
void receiveCommand(char **arrayDir, char **arrayCom){
    char **actualCommand = malloc(sizeof(char *) * 64);
    int background = 0, executed, index = 0;
    for(int i = 0; arrayCom[i]; i++){
        if(arrayCom[0] && strcmp(arrayCom[0], "cd") == 0){
            char *target = arrayCom[1] ? arrayCom[1] : getenv("HOME");
            if(chdir(target) != 0) perror("cd failed");
            return; // skip receiveCommand logic entirely
        }
        if(!strcmp(arrayCom[i], "&")){
            background = 1;
            continue;
        }
        if(!strcmp(arrayCom[i], "&&") || !strcmp(arrayCom[i], "||") || !strcmp(arrayCom[i], ";")){
            actualCommand[index] = NULL;
            executed = execute(arrayDir, actualCommand, background);
            background = 0;
            // checks for the chain operators behavior now
            // "&&" case runs if the previous worked
            if(!strcmp(arrayCom[i], "&&")){
                if(!executed) break;
            }
            // "||" case runs if the previous failed
            if(!strcmp(arrayCom[i], "||")){
                if(executed) break;
            }
            // ";" case just runs the next command
            free(actualCommand);
            index = 0;
            continue;
        }
        actualCommand[index] = arrayCom[i];
        index++;
    }
    actualCommand[index] = NULL;
    execute(arrayDir, actualCommand, background);
    background = 0;
    index = 0;
    free(actualCommand);
}