/*
 * @Author: LZH
 * @Date: 2022-04-14 22:56:00
 * @LastEditTime: 2022-04-15 10:20:06
 * @Description: 
 * @FilePath: /MyFiles/6_高级数据结构/data_structure/2.AVL.c
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int key, h; 
    struct Node *lchild, *rchild;
} Node;

Node __NIL;
#define NIL &__NIL
__attribute__((constructor))
void init_NIL() {
    __NIL.h = 0;
    __NIL.key = 0;
    __NIL.lchild = (__NIL.rchild = NIL);
}

Node* getNode(int key) {
    Node * temp = (Node*)malloc(sizeof(Node));
    temp->key = key;
    temp->h = 1;
    temp->lchild = NIL;
    temp->rchild = NIL;
}

#define max(a, b) ((a) > (b) ? (a) : (b))
void update_height(Node *root) {
    root->h = max(root->lchild->h, root->rchild->h) + 1;
}

Node* left_rotate(Node *root) {
    Node* temp = root->rchild;
    root->rchild = temp->lchild;
    temp->lchild = root;
    update_height(root);
    update_height(temp);
    return temp;
}

Node* right_rotate(Node *root) {
    Node* temp = root->lchild;
    root->lchild = temp->rchild;
    temp->rchild = root;
    update_height(root);
    update_height(temp);
    return temp;
}


// debug  arrary
char type_of_rotate[4][3] = {"LL", "LR", "RR", "RL"};
Node *maintain(Node * root) {
    if (abs(root->lchild->h - root->rchild->h) < 2) return root;
    int type = -1;
    if (root->lchild->h > root->rchild->h) {
        if (root->lchild->lchild->h < root->lchild->rchild->h) {
            //LR
            root->lchild = left_rotate(root->lchild);
            type++;
        }
        // LL
        root = right_rotate(root);
        type++;
    } else if (root->lchild->h < root->rchild->h) {
        if (root->rchild->lchild->h > root->rchild->rchild->h ) {
            // RL
            root->rchild = right_rotate(root->rchild);
            type++;
        }
        // RR
        root = left_rotate(root);
        type++;
    }
    if (type != -1)
        printf("Unbalanced Type :[ %s ]\n", type_of_rotate[type]);
    return root;
}

Node *insert(Node *root, int key) {
    if (root == NIL) return getNode(key);
    if (root->key == key) return root;
    if (root->key < key) root->rchild = insert(root->rchild, key);
    else if (root->key > key) root->lchild = insert(root->lchild, key);
    update_height(root);
    return maintain(root);
}

Node *erase(Node *root,  int key) {
    if (root == NIL) return root;
    if (root->key > key) root->lchild = erase(root->lchild, key);
    else if (root->key < key) root->rchild = erase(root->rchild, key);
    else {
        if (root->lchild == NIL || root->rchild == NIL) {
            Node *temp = root->lchild == NIL?root->rchild : root->lchild;
            free(root);
            return temp;
        } else {
            Node *temp = root->rchild;
            while (temp->lchild != NIL) {
                temp = temp->lchild;
            }
            root->key = temp->key;
            root->rchild = erase(root->rchild, temp->key);
        }
    }
    update_height(root);
    root = maintain(root);
    return root;
}


void print_node(Node *root) {
    printf("( %4d [%4d] | %4d , %4d )\n", 
        root->key, root->h,
        root->lchild->key,
        root->rchild->key
    );
}

void out_put(Node *root) {
    if (root == NIL) return;
    print_node(root);
    out_put(root->lchild);
    out_put(root->rchild);
    return;
}

int main(int argc, char const *argv[]) {
    Node *root = NIL;
    // test insert
    int temp;
    while (~scanf("%d", &temp)) {
        if (temp == -1) break;
        printf("\nInsert %d to root.\n", temp);
        root = insert(root, temp);
        out_put(root);
    }

    // test erase
    while (~scanf("%d", &temp)) {
        printf("\nerase %d from root.\n", temp);
        root = erase(root, temp);
        out_put(root);
    }


    return 0;
}
