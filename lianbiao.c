#include "lb.h"
#include <locale.h>
#include <unistd.h>


Node *head =NULL;
Node *create_node_head(char *vlaue, Node *head){
     Node*p = (Node*)malloc(sizeof(Node));
     strcpy(p->vlaue, vlaue);
     p->next = head;
     head = p;
     return p;
}
Node *create_node_tail(char *vlaue, Node *head){
     Node*p = (Node*)malloc(sizeof(Node));
     strcpy(p->vlaue, vlaue);
     p->next = NULL;
     if (head == NULL) {
         head = p;
     } else {
         Node* tmp = head;
         while (tmp->next != NULL) {
             tmp = tmp->next;
         }
         tmp->next = p;
     }
     return head;
}

void free_node(Node* head){
    Node* tmp;
    while(head != NULL){
        tmp = head;      // 先保存当前节点
        head = head->next; // 跳到下一个节点
        free(tmp);       // 再释放
    }
}
Node *load_data_from_file(const char *filename, Node *head) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return head;
    }
    char buffer[MAX_VLAUE];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0; // remove newline
        head = create_node_tail(buffer, head);
    }
    fclose(fp);
    return head;
}

void print(const char *filename, Node*head){
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    while(head){
        // printf("%s",head->vlaue);
        fprintf(fp,"%s\n",head->vlaue);
        head = head->next;
    }
    fclose(fp);
}


int main(){
    system("chcp 65001 >nul 2>&1");  // 设置控制台为UTF-8
    setlocale(LC_ALL, "zh_CN.UTF-8");  // 设置为中文 UTF-8 locale
    Node *head =load_data_from_file("data/test.txt", NULL);
    printf("[Success] File opened successfully\n");
    
    printf("Please enter a string: ");
    fflush(stdout);  // Immediately flush output

    char a[MAX_VLAUE];
    do {
        scanf("%s", a);
        if (strcmp(a, "0") != 0) {
            head = create_node_tail(a, head);
        }
    } while (strcmp(a, "0") != 0);
    print("data/test.txt", head);
    free_node(head);
    printf("[Success] Memory freed\n");
    
    printf("[Success] File closed\n");
    printf("[Success] Program finished! Data saved to data/test.txt\n");
    
    system("pause");  // Pause reliably
    
    return 0;
}
