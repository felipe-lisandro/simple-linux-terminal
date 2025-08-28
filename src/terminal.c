#include "terminal.h"

int run(){
    char **arrayDir = get_sys_paths();
    if(!arrayDir){
        fprintf(stderr, "Could not get the System Paths.\n");
        return 1;
    }
    printf("Simple Shell initialized\n");
    while(1){
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            printf("%s_ ", cwd);
        }
        CommandStruct *cmds = command_treatment(input_com(), arrayDir);
        execute_all(cmds);
        for (int i = 0; cmds[i].stringArray; i++) {
            for (int j = 0; cmds[i].stringArray[j]; j++)
                free(cmds[i].stringArray[j]);
            free(cmds[i].stringArray);
        }
        free(cmds);
    }
}