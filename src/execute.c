#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "execute.h"
#include "builtin.h"

int execute(CommandStruct *cmd){
    int status = -1;
    char **dirs = cmd->directories;
    for(int i = 0; dirs[i]; i++){
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirs[i], cmd->stringArray[0]);
        if (access(fullpath, X_OK) == 0) {
            pid_t pid = fork();
            if(pid == 0){ // child
                execvp(fullpath, cmd->stringArray);
                perror("execvp failed");
                _exit(127); // standard “command not found” exit
            }
            else if(pid > 0){ // parent
                if(!cmd->background){
                    int wstatus;
                    if(waitpid(pid, &wstatus, 0) == -1){
                        perror("waitpid failed");
                        return -1;
                    }
                    if(WIFEXITED(wstatus)) status = WEXITSTATUS(wstatus);
                    else if(WIFSIGNALED(wstatus)) status = 128 + WTERMSIG(wstatus);
                }
                else{
                    // background job -> consider started successfully
                    status = 0;
                }
            }
            else{
                perror("fork failed");
                return -1;
            }
            fflush(stdout);
            return status; // command executed, exit code ready
        }
    }
    fprintf(stderr, "Command not found: %s\n", cmd->stringArray[0]);
    return 127; // standard “command not found” code
}

void execute_all(CommandStruct *cmds){
    int lastStatus = 0, status = 0;
    for(int i = 0; cmds[i].stringArray; i++){
        CommandStruct *cmd = &cmds[i];
        int run = 1;
        // decide if this command should run
        if (cmd->modifier == 1 && lastStatus != 0) run = 0; // &&
        if (cmd->modifier == 2 && lastStatus == 0) run = 0; // ||-
        if(run && (status != -1)) status = run_builtin(cmd->stringArray), lastStatus = status; 
        if (run && (status == -1)) lastStatus = execute(cmd);
        else continue; 
    }
}