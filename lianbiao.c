#include "lb.h"
#include <locale.h>
#include <unistd.h>
#include <windows.h>
#include <stdio.h>



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
    
    Node *tail = NULL;
    if (head != NULL) {
        tail = head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
    }
    
    char buffer[MAX_VLAUE];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0; // remove newline
        
        Node *p = (Node*)malloc(sizeof(Node));
        strcpy(p->vlaue, buffer);
        p->next = NULL;
        
        if (head == NULL) {
            head = p;
            tail = p;
        } else {
            tail->next = p;
            tail = p;
        }
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

Node *find_data(char*vlaue,Node*p){
	if(!p){
		printf("啥也没有");
		return NULL;
	}
	while(p){
		if(strcmp(p->vlaue,vlaue) ==0){
			return p;
		}
		p =p->next;
	}
	return NULL;
}

Node *delet_data(const char*vlaue, Node*p){
	if(!p){
		printf("数据不存在");
		return p;
	}
	// 如果要删除的是头节点
	if(strcmp(p->vlaue, vlaue) == 0){
		Node *temp = p->next;
		free(p);
		return temp;
	}
	// 如果要删除的不是头节点
	Node *temp = p;
	while(temp->next){
		if(strcmp(temp->next->vlaue, vlaue) == 0){
			Node *to_delete = temp->next;
			temp->next = to_delete->next;
			free(to_delete);
			return p;
		}
		temp = temp->next;
	}
	printf("数据不存在");
	return p;
}
int main(){
    printf("[Success] Program started\n");
    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    
    // 用Windows API设置UTF-8，比system()快得多
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    // setlocale(LC_ALL, "zh_CN.UTF-8");  // 移除可能慢的locale设置
    
    Node *head =load_data_from_file("data/test.txt", NULL);
    
    QueryPerformanceCounter(&end);
    double time_taken = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    printf("初始化时间: %.6f 秒\n", time_taken);
    
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
    printf("可以查找了，输入要查找的字符串：");
    char search[MAX_VLAUE];
    scanf("%s", search);
    Node *found = find_data(search, head);
    if (found) {
        printf("找到了: %s\n", found->vlaue);
    } else {
        printf("没找到\n");
    }
    printf("可以删除了，输入要删除的字符串：");
    char del[MAX_VLAUE];
    scanf("%s", del);
    head = delet_data(del, head);
    print("data/test.txt", head);
    printf("删除完成，数据已保存\n");
    free_node(head);
    printf("[Success] Memory freed\n");
    
    printf("[Success] File closed\n");
    printf("[Success] Program finished! Data saved to data/test.txt\n");
    
    system("pause");  // Pause reliably
    
    return 0;
}
