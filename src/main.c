#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "input.h"
#include <string.h>

int main(void){
    char **arrayDir = getSysPaths();
    if(!arrayDir){
        fprintf(stderr, "Could not get the System Paths.\n");
        return 1;
    }
    printf("Simple Shell initialized\n");
    while(1){
        printf("@simpleshell ");
        char **arrayCom = inputCom();
        if(!arrayCom) continue; // check if fail
        if(!arrayCom[0]){
            free(arrayCom);
            continue; // check if nothing was inputed
        }
        if(strcmp(arrayCom[0], "exit") == 0){ // check if program should exit
            for(int i = 0; arrayCom[i]; i++) free(arrayCom[i]);
            free(arrayCom);
            break;
        }
        // check to see if the command should be run on background
        int background = 0;
        int nCom = 0;
        while(arrayCom[nCom]) nCom++;
        if(nCom > 1 && !strcmp(arrayCom[nCom - 1], "&")){
            background = 1;
            free(arrayCom[nCom - 1]);
            arrayCom[nCom] = NULL;
        }
        // finally, executes the command
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
        for(int i = 0; arrayCom[i] != NULL; i++) free(arrayCom[i]);
        free(arrayCom);
    }
    // as the program exits, the arrayDir should be fred
    for(int i = 0; arrayDir[i]; i++) free(arrayDir[i]);
    free(arrayDir);
    return 0;
}
