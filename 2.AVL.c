/*
 * @Author: LZH
 * @Date: 2022-04-14 22:56:00
 * @LastEditTime: 2022-04-15 09:56:43
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


Node *maintain(Node * root) {
    if (root == NIL) return root;
    // R
    if (root->lchild->h == root->rchild->h + 2) {
        if (root->lchild->lchild->h == root->lchild->rchild->h + 1) {
            //RL
            root->lchild = left_rotate(root->lchild);
        }
        root = right_rotate(root);
    //  L
    } else if (root->lchild->h + 2 == root->rchild->h) {
        if (root->rchild->lchild->h == root->rchild->rchild->h + 1) {
            root->rchild = right_rotate(root->rchild);
        }
        root = left_rotate(root);
    }
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
    printf("( %d [%d] | %d , %d )\n", 
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
        printf("Insert %d to root.\n", temp);
        root = insert(root, temp);
        out_put(root);
    }

    // test erase
    while (~scanf("%d", &temp)) {
        printf("erase %d from root.\n", temp);
        root = erase(root, temp);
        out_put(root);
    }


    return 0;
}
