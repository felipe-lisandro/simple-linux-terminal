#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "input.h"
#include "execute.h"
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
        // finally, executes the command
        receiveCommand(arrayDir, arrayCom);
    }
    // as the program exits, the arrayDir should be fred
    for(int i = 0; arrayDir[i]; i++) free(arrayDir[i]);
    free(arrayDir);
    return 0;
}
