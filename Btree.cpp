/*** 
 * @Author: LZH
 * @Date: 2022-06-01 20:13:12
 * @LastEditTime: 2022-06-01 21:21:51
 * @Description: 
 * @FilePath: /MyFiles/6_高级数据结构/data_structure/Btree.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_M 3
typedef struct Node {
    int n;
    int key[MAX_M + 1];
    struct Node *next[MAX_M + 1];
} Node;

Node *getNewNode() {
    Node *p = (Node *)malloc(sizeof(Node));
    p->n = 0;
    memset(p->next, 0, sizeof(Node *) * (MAX_M + 1));
    return p;
}

Node *insert_maintain(Node *root, Node *child, int pos) {
    if (child->n < MAX_M) return root;
    int spos = MAX_M / 2;
    Node *node1 = getNewNode();
    Node *node2 = getNewNode();
    node1->n = spos;
    node2->n = MAX_M - 1 - spos;
    for (int i = 0; i < node1->n; i++) {
        node1->key[i] = child->key[i];
        node1->next[i] = child->next[i];
    }
    node1->next[node1->n] = child->next[node1->n];
    for (int i = 0; i < node2->n; i++) {
        node2->key[i] = child->key[spos + 1 + i];
        node2->next[i] = child->next[spos + 1 + i];
    }
    node2->next[node2->n] = child->next[child->n];
    // insert child key to root
    for (int i = root->n; i >= pos; i--) {
        root->key[i + 1] = root->key[i];
        root->next[i + 1] = root->next[i];
    }
    root->key[pos] = child->key[spos];
    root->next[pos] = node1;
    root->next[pos + 1] = node2;
    root->n += 1;
    free(child);
    return root;
}

Node *insert_key(Node *root, int key) {
    if (root == NULL) {
        root = getNewNode();
        root->key[(root->n)++] = key;
        return root;
    }
    int pos = 0;
    while (pos < root->n && root->key[pos] < key) pos += 1;
    if (pos < root->n && root->key[pos] == key) return root;
    for (int i = root->n - 1; i >= pos; i--) {
        root->key[i + 1] = root->key[i];
    }
    root->key[pos] = key;
    root->n += 1;
    return root;
}

Node *__insert(Node *root, int key) {
    if (root == NULL || root->next[0] == NULL) {
        return insert_key(root, key);
    }
    int pos = 0;
    while (pos < root->n && root->key[pos] < key) pos += 1;
    if (pos < root->n && root->key[pos] == key) return root;
    __insert(root->next[pos], key);
    return insert_maintain(root, root->next[pos], pos);
}

Node *insert(Node *root, int key) {
    root = __insert(root, key);
    if (root->n == MAX_M) {
        Node *p = getNewNode();
        p->next[0] = root;
        root = insert_maintain(p, root, 0);
    }
    return root;
}

void clear(Node *root) {
    if (root == NULL) return ;
    if (root->next[0] != NULL) {
        for (int i = 0; i <= root->n; i++) {
            clear(root->next[i]);
        }
    }
    free(root);
    return ;
}

void print_node(Node *root) {
    printf("%d : ", root->n);
    for (int i = 0; i < root->n; i++) {
        printf("%4d", root->key[i]);
    }
    printf(" |");
    if (root->next[0] != NULL) {
        for (int i = 0; i <= root->n; i++) {
            printf("%4d", root->next[i]->key[0]);
        }
    }
    printf("\n");
    return ;
}

void output(Node *root) {
    if (root == NULL) return ;
    print_node(root);
    for (int i = 0; i <= root->n; i++) {
        output(root->next[i]);
    }
    return ;
}

int main() {
    srand(time(0));
    Node *root = NULL;
    #define MAX_OP 20
    for (int i = 0; i < MAX_OP; i++) {
        int val = rand() % 100;
        root = insert(root, val);
        printf("\ninsert %d to BTree : \n", val);
        output(root);
    }
    #undef MAX_OP
    return 0;
}

