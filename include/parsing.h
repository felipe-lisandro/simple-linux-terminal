#ifndef PARSING_H
#define PARSING_H

typedef struct commNode {
    char **comm;
    int conditional; // "||" = -1, "&&" = 1, none = 0
    int background;
} commNode;

commNode* parsing(char **arrayCom);

#endif