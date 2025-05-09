#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME 10

typedef struct Node
{
    int id;
    char name[MAX_NAME];
    struct Node *next;
    struct Node *pre;
} Node;