#include "builtin.h"

int builtin_cd(char **args);
int builtin_exit(char **args);
int builtin_pwd(char **args);
int builtin_echo(char **args);

Builtin builtins[] = {
    {"cd", builtin_cd},
    {"exit", builtin_exit},
    {"pwd", builtin_pwd},
    {"echo", builtin_echo},
    {NULL, NULL} // sentinel
};

int builtin_cd(char **args){
    if(!args[1]){
        fprintf(stderr, "cd: missing argument\n");
        return 1;
    }
    if(chdir(args[1]) != 0){
        perror("cd");
        return 1;
    }
    return 0;
}

int builtin_exit(char **args){
    exit(0);
}

int builtin_pwd(char **args){
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s\n", cwd);
        return 0;
    }
    else{
        perror("pwd");
        return 1;
    }
}

int builtin_echo(char **args){
    for(int i = 1; args[i]; i++){
        printf("%s", args[i]);
        if(args[i + 1]) printf(" ");
    }
    printf("\n");
    return 0;
}

int run_builtin(char **args){
    for(int i = 0; builtins[i].name; i++){
        if(!strcmp(args[0], builtins[i].name)) return builtins[i].func(args);
    }
    return -1;
}