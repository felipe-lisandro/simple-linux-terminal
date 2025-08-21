#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "execute.h"
#include "handler.h"
#include "parsing.h"

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
    commNode* commParsed = parsing(arrayCom);
    int lastExecution;
    for(int i = 0; i < 16; i++){
        if(i == 0) lastExecution = execute(arrayDir, commParsed[i].comm, commParsed[i].background);
        else{
            if(commParsed[i].conditional == 1){
                if(lastExecution) execute(arrayDir, commParsed[i].comm, commParsed[i].background);
            }
            else if(commParsed[i].conditional == -1){
                if(!lastExecution) execute(arrayDir, commParsed[i].comm, commParsed[i].background);
            }
        }
    }
}