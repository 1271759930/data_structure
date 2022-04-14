/*
 * @Author: LZH
 * @Date: 2022-04-11 19:23:02
 * @LastEditTime: 2022-04-12 17:16:36
 * @Description: 
 * @FilePath: /MyFiles/6_高级数据结构/1.binary_search_tree.c
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000005

#define KEY(r) (r?r->key:0)

typedef struct Node {
    int key;
    Node *lchicd, *rchild;
}Node;

Node *getNode(int key) {
    Node *p = (Node *)malloc(sizeof(Node));
    p->key = key;
    p->lchicd = p->rchild = NULL;
    return p;
}

Node *insert(Node *root, int key) {
    if (root == NULL) return getNode(key);
    if (root->key == key) return root;
    if (key > root->key) insert(root->lchicd, key);
    else insert(root->rchild, key);
    return root;
}

Node *predecessor(Node *root) {
    Node *temp = root->lchicd;
    while(temp->rchild) temp = temp->rchild;
    return temp;
}

Node *erase(Node *root, int key) {
    if (root == NULL) return root;
    if (key > root->key) erase(root->rchild, key);
    else if (key < root->key) erase(root->lchicd, key);
    else {
        if (root->lchicd == NULL && root->rchild == NULL) {
            free(root);
            return NULL;
        } else if (root->lchicd == NULL || root->rchild == NULL) {
            Node *p = root->lchicd?root->lchicd:root->rchild;
            free(root);
            return p;
        } else {
            Node *temp = predecessor(root);
            root->key = temp->key;
            root->lchicd = erase(root->lchicd, temp->key);
            return root;
        }
    }
}

void print_node(Node *root) {
    printf("(%d  |  %d, %d )\n", 
        KEY(root), KEY(root->lchicd), KEY(root->rchild));
}

void output(Node *root) {
    if (root == NULL) return;
    print_node(root);
    output(root->lchicd);
    output(root->rchild);
}

void inorder_output(Node *root) {
    if (root == NULL) return;
    inorder_output(root->lchicd);
    printf("%d ", root->key);
    inorder_output(root->rchild);
}


int main(int argc, char const *argv[]) {
    return 0;
}
