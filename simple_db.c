#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COLS 10
#define MAX_ROWS 1000
#define MAX_STR_LEN 100

typedef struct {
    char name[MAX_STR_LEN];
    char type[10]; // "int", "float", "string"
} Column;

typedef struct {
    Column columns[MAX_COLS];
    int num_columns;
    char data[MAX_ROWS][MAX_COLS][MAX_STR_LEN];
    int num_rows;
} Table;

Table* create_table(const char* name) {
    Table* table = (Table*)malloc(sizeof(Table));
    table->num_columns = 0;
    table->num_rows = 0;
    return table;
}

void add_column(Table* table, const char* name, const char* type) {
    if (table->num_columns >= MAX_COLS) return;
    strcpy(table->columns[table->num_columns].name, name);
    strcpy(table->columns[table->num_columns].type, type);
    table->num_columns++;
}

void insert_row(Table* table, const char* values) {
    if (table->num_rows >= MAX_ROWS) return;

    char temp[MAX_STR_LEN * MAX_COLS];
    strcpy(temp, values);

    char* token = strtok(temp, ",");
    int col = 0;
    while (token && col < table->num_columns) {
        // 去除前后空格
        while (*token == ' ') token++;
        char* end = token + strlen(token) - 1;
        while (end > token && *end == ' ') *end-- = '\0';

        strcpy(table->data[table->num_rows][col], token);
        token = strtok(NULL, ",");
        col++;
    }
    table->num_rows++;
}

void save_table(Table* table, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    // 写列名
    for (int i = 0; i < table->num_columns; i++) {
        fprintf(fp, "%s", table->columns[i].name);
        if (i < table->num_columns - 1) fprintf(fp, ",");
    }
    fprintf(fp, "\n");

    // 写数据
    for (int r = 0; r < table->num_rows; r++) {
        for (int c = 0; c < table->num_columns; c++) {
            fprintf(fp, "%s", table->data[r][c]);
            if (c < table->num_columns - 1) fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

Table* load_table(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return NULL;

    Table* table = create_table("loaded");

    char line[MAX_STR_LEN * MAX_COLS];
    int is_first_line = 1;

    while (fgets(line, sizeof(line), fp)) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;

        if (is_first_line) {
            // 第一行是列名
            char* token = strtok(line, ",");
            while (token) {
                while (*token == ' ') token++;
                char* end = token + strlen(token) - 1;
                while (end > token && *end == ' ') *end-- = '\0';
                add_column(table, token, "string"); // 简单起见都设为string
                token = strtok(NULL, ",");
            }
            is_first_line = 0;
        } else {
            // 数据行
            insert_row(table, line);
        }
    }
    fclose(fp);
    return table;
}

void select_all(Table* table) {
    // 打印列名
    for (int i = 0; i < table->num_columns; i++) {
        printf("%s\t", table->columns[i].name);
    }
    printf("\n");

    // 打印数据
    for (int r = 0; r < table->num_rows; r++) {
        for (int c = 0; c < table->num_columns; c++) {
            printf("%s\t", table->data[r][c]);
        }
        printf("\n");
    }
}

void select_where(Table* table, const char* column, const char* value) {
    int col_index = -1;
    for (int i = 0; i < table->num_columns; i++) {
        if (strcmp(table->columns[i].name, column) == 0) {
            col_index = i;
            break;
        }
    }
    if (col_index == -1) {
        printf("Column not found\n");
        return;
    }

    // 打印列名
    for (int i = 0; i < table->num_columns; i++) {
        printf("%s\t", table->columns[i].name);
    }
    printf("\n");

    // 打印匹配的行
    for (int r = 0; r < table->num_rows; r++) {
        if (strcmp(table->data[r][col_index], value) == 0) {
            for (int c = 0; c < table->num_columns; c++) {
                printf("%s\t", table->data[r][c]);
            }
            printf("\n");
        }
    }
}

int main() {
    // 创建表
    Table* users = create_table("users");
    add_column(users, "id", "int");
    add_column(users, "name", "string");
    add_column(users, "age", "int");

    // 插入数据
    insert_row(users, "1, Alice, 25");
    insert_row(users, "2, Bob, 30");
    insert_row(users, "3, Charlie, 35");

    // 保存到文件
    save_table(users, "users.csv");

    // 从文件加载
    Table* loaded = load_table("users.csv");

    printf("All users:\n");
    select_all(loaded);

    printf("\nUsers with age 30:\n");
    select_where(loaded, "age", "30");

    free(users);
    free(loaded);
    return 0;
}