#include <stdio.h>
#include <stdlib.h>
#define MAX_VLAUE 50
typedef struct node{
        char *vlaue[MAX_VLAUE];
        struct node *next;
}Node;
Node *create_node(char *vlaue);
void free_node(Node*p);
void print(Node*head);              
