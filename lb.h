#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_VLAUE 50
typedef struct node{
        char vlaue[MAX_VLAUE];
        struct node *next;
}Node;

Node *create_node_head(char *vlaue, Node *head);
Node *create_node_tail(char *vlaue, Node *head);
Node *load_data_from_file(const char *filename, Node *head);
void free_node(Node*p);

void print(const char *filename, Node*head);              


