/*
 * @Author: LZH
 * @Date: 2022-04-28 16:02:57
 * @LastEditTime: 2022-04-28 16:21:10
 * @Description: 
 * @FilePath: /MyFiles/6_高级数据结构/data_structure/6_trie.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 26
#define MAX_N 10000
char s[MAX_N + 5];

typedef struct Node {
    int flag;
    struct Node *next[BASE];
} Node;

Node *getNewNode() {
    Node *p = (Node *)malloc(sizeof(Node));
    p->flag = 0;
    memset(p->next, 0, sizeof(Node *) * BASE);
    return p;
}

inline int code(char c) {
    return c - 'a';
}

void insert(Node *root, const char *s) {
    Node *p = root;
    for (int i = 0; s[i]; i++) {
        int ind = code(s[i]);
        if (p->next[ind] == NULL) p->next[ind] = getNewNode();
        p = p->next[ind];
    }
    p->flag = 1;
    return ;
}

int search(Node *root, const char *s) {
    Node *p = root;
    for (int i = 0; s[i]; i++) {
        int ind = code(s[i]);
        p = p->next[ind];
        if (p == NULL) return 0;
    }
    return p->flag;
}

void __string_sort(Node *root, int k, char *s) {
    s[k] = '\0';
    if (root->flag) {
        printf("find string : %s\n", s);
    }
    for (int i = 0; i < BASE; i++) {
        if (root->next[i] == NULL) continue;
        s[k] = 'a' + i;
        __string_sort(root->next[i], k + 1, s);
    }
    return ;
}

void string_sort_print(Node *root) {
    __string_sort(root, 0, s);
    return ;
}

void clear(Node *root) {
    if (root == NULL) return ;
    for (int i = 0; i < BASE; i++) clear(root->next[i]);
    free(root);
    return ;
}

int main() {
    Node *root = getNewNode();
    // insert 
    while (~scanf("%s", s)) {
        if (strcmp(s, "__end__") == 0) break;
        insert(root, s);
        printf("insert %s done\n", s);
    }
    
    // search 
    while (~scanf("%s", s)) {
        if (strcmp(s, "__end__") == 0) break;
        printf("search(\"%s\") = %d\n", s, search(root, s));
    }

    string_sort_print(root);
    clear(root);
    return 0;
}
