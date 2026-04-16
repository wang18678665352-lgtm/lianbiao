#include "lb.h"

Node *head =NULL;
Node *create_node(char *vlaue){
     Node*p = (Node*)malloc(sizeof(Node));
     p->next = head;
     head = p;
};
void free_node(Node*p){
     while(p){     
        p = p->next;
        free(p);
        };
};
void print(Node*head){
     while(head){
        printf("%s",head->vlaue);
        head = head->next;
    };
};
int main(){
    char a[MAX_VLAUE];
    scanf("%S",&a);
    create_node(a);
    print(head);
    free_node(head);
    return 0;
    
};
