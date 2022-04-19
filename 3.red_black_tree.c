/*
 * @Author: LZH
 * @Date: 2022-04-19 20:44:32
 * @LastEditTime: 2022-04-19 22:44:12
 * @Description: 
 * @FilePath: /MyFiles/6_高级数据结构/data_structure/3.red_black_tree.c
 */


#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    int key, color;   // 0 is red, 1 is black, 2 is double black;
    struct Node *lchild, *rchild;
} Node;

#define NIL &__NIL
Node __NIL;
__attribute__((constructor))
void init_node() {
    __NIL.key = 0;
    __NIL.color = 1;
    __NIL.lchild = __NIL.rchild = NIL;
}

Node* getNode(int key) {
    Node *temp = (Node*)malloc(sizeof(Node));
    temp->key = key;
    temp->lchild = temp->rchild = NIL;
    temp->color = 0;
    return temp;
}

int has_red_children(Node *root) {
    if (root == NIL) return 0;
    return (root->lchild->color == 0 || root->rchild->color == 0);
}

Node *left_rotate(Node *root) {
    Node *temp = root->rchild;
    root->rchild = temp->lchild;
    temp->lchild = root;
    return temp;
}

Node *right_rotate(Node *root) {
    Node *temp = root->lchild;
    root->lchild = temp->rchild;
    temp->rchild = root;
    return temp;
}


Node * maintain(Node *root) {
    if (!has_red_children(root)) return root;
    if ( root->lchild->color == 0 && !has_red_children(root->lchild) ||
         root->rchild->color == 0 && !has_red_children(root->rchild)
    ) return root;
    if (root->lchild->color == 1) {
        if (root->rchild->lchild->color == 0) {
            root->rchild = right_rotate(root->rchild);
        }
        root = left_rotate(root);
    } else if (root->rchild->color == 1) {
        if (root->lchild->rchild->color == 0) {
            root->lchild = left_rotate(root->lchild);
        }
        root = right_rotate(root);
    }
    root->color = 0;
    root->lchild->color = root->rchild->color = 1;
    return root;
}

Node* __insert(Node* root, int key) {
    if (root == NIL) return getNode(key);
    if (root->key == key) return root;
    if (root->key > key) root->lchild = __insert(root->lchild, key);
    else if (root->key < key) root->rchild = __insert(root->rchild, key);
    return  maintain(root);
}

Node * insert(Node* root, int key) {
    root = __insert(root, key);
    root->color = 1;
    return root;
}


void output(Node* root) {
    if (root == NIL) return;
    printf("%4d[%d] (%4d,%4d)\n", 
        root->key, root->color,
        root->lchild->key, 
        root->rchild->key
    );
    output(root->lchild);
    output(root->rchild);
}

int main(int argc, char const *argv[]) {

    int temp;
    Node* root = NIL;
    while (~scanf("%d", &temp)) {
        printf("====insert %d to tree====\n", temp);
        root = insert(root, temp);
        output(root);
    }

    return 0;
}
