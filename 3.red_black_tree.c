/*
 * @Author: LZH
 * @Date: 2022-04-19 20:44:32
 * @LastEditTime: 2022-04-26 15:20:18
 * @Description: 
 * @FilePath: /MyFiles/6_高级数据结构/data_structure/3.red_black_tree.c
 */


#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key, color;   // 0 is red, 1 is black, 2 is double black;
    struct Node *lchild, *rchild;
} Node;

Node __NIL;
#define NIL (&__NIL)

__attribute__((constructor))
void init_node() {
    __NIL.key = 0;
    __NIL.color = 1;
    __NIL.lchild = __NIL.rchild = NIL;
}

Node *getNewNode(int key) {
    Node *p = (Node *)malloc(sizeof(Node));
    p->key = key;
    p->lchild = p->rchild = NIL;
    p->color = 0;
    return p;
}

int has_red_child(Node *root) {
    return root->lchild->color == 0 || root->rchild->color == 0;
}

Node *left_rotate(Node *root) {
    printf("left rotate : %d\n", root->key);
    Node *temp = root->rchild;
    root->rchild = temp->lchild;
    temp->lchild = root;
    return temp;
}

Node *right_rotate(Node *root) {
    printf("right rotate : %d\n", root->key);
    Node *temp = root->lchild;
    root->lchild = temp->rchild;
    temp->rchild = root;
    return temp;
}

const char *insert_maintain_type_str[] = {
    "1 : change color",
    "2 : LL", 
    "2 : LR", 
    "2 : RR", 
    "2 : RL"
};

Node * insert_maintain(Node *root) {
    if (!has_red_child(root)) return root;
    if (  //  如果不是两种红色相邻的情况，则退出
        !(root->lchild->color == 0 && has_red_child(root->lchild)) &&
        !(root->rchild->color == 0 && has_red_child(root->rchild))
    ) return root;
    int type = 0;
    if (root->rchild->color == 1) {
        if (root->lchild->rchild->color == 0) {
            type++;
            root->lchild = left_rotate(root->lchild);
        }
        type++;
        root = right_rotate(root);
    } else if (root->lchild->color == 1) {
        type = 2;
        if (root->rchild->lchild->color == 0) {
            type++;
            root->rchild = right_rotate(root->rchild);
        }
        type++;
        root = left_rotate(root);
    }
    printf("insert maintain type = %s\n", insert_maintain_type_str[type]);
    root->color = 0;
    root->lchild->color = root->rchild->color = 1;
    return root;
}

Node *__insert(Node *root, int key) {
    if (root == NIL) return getNewNode(key);
    if (root->key == key) return root;
    if (root->key > key) root->lchild = __insert(root->lchild, key);
    else root->rchild = __insert(root->rchild, key);
    return insert_maintain(root);
}

Node *insert(Node *root, int key) {
    root = __insert(root, key);
    root->color = 1;
    return root;
}

const char *erase_maintain_type_str[] = {
    "red 1(right): change color",
    "red 2(left) : change color",
    "black 1 : no red child",
    "black 2 : LL",
    "black 2 : LR",
    "black 2 : RR",
    "black 2 : RL",
};

Node *erase_maintain(Node *root) {
    if (root->lchild->color != 2 && root->rchild->color != 2) return root;
    // 兄弟是红色
    int type = 0;
    if (root->lchild->color == 0) {
        root->color = 0;
        root = right_rotate(root);
        root->color = 1;
        root->rchild = erase_maintain(root->rchild);
    printf("brother is %s\n", erase_maintain_type_str[0]);
        return root;
    } else if (root->rchild->color == 0) {
        root->color = 0;
        root = left_rotate(root);
        root->color = 1;
        root->lchild = erase_maintain(root->lchild);
    printf("brother is %s\n", erase_maintain_type_str[1]);
        return root;
    }
    // 兄弟黑，兄弟孩子黑
    if ((root->lchild->color == 1 && !has_red_child(root->lchild)) ||
        (root->rchild->color == 1 && !has_red_child(root->rchild))
    ) {
        type = 2;
        root->color++;
        root->lchild->color--;
        root->rchild->color--;
        return root;
    }
    type = 2;
    // 兄弟黑，兄弟有红孩子
    if (root->lchild->color == 1) {
        if (root->lchild->lchild->color != 0) {
            type += 1;
            // root->lchild->color = 0;
            // root->lchild->rchild->color = 1;
            root->lchild = left_rotate(root->lchild);
        }
        type += 1;
        root->rchild->color = 1;
        root->lchild->color = root->color;
        root = right_rotate(root);
    } else {
        type = 4;
        if (root->rchild->rchild->color != 0) {
            type += 1;
            // root->rchild->color = 0;
            // root->rchild->lchild->color = 1;
            root->rchild = right_rotate(root->rchild);
        }
        type += 1;
        root->lchild->color = 1;
        root->rchild->color = root->color;
        root = left_rotate(root);
    }
    root->lchild->color = root->rchild->color = 1;
    printf("brother is %s\n", erase_maintain_type_str[type]);
    return root;
}

Node *__erase(Node *root, int key) {
    if (root == NIL) return root;
    if (root->key > key) root->lchild = __erase(root->lchild, key);
    else if (root->key < key) root->rchild = __erase(root->rchild, key);
    else {  // 度为2的情况
        if (root->lchild == NIL || root->rchild == NIL) {
            Node *temp = root->lchild == NIL ? root->rchild : root->lchild;
            temp->color += root->color;
            free(root);
            return temp;
        } else {
            Node *temp = root->lchild;
            // 注意！这里要使用NIL
            while (temp->rchild != NIL) temp = temp->rchild;
            root->key = temp->key;
            root->lchild = __erase(root->lchild, temp->key);
        }
    }
    return erase_maintain(root);
}

Node *erase(Node* root, int key) {
    root = __erase(root, key);
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
        if (temp == -1) break;
        printf("====insert %d to tree====\n", temp);
        root = insert(root, temp);
        output(root);
    }


    while (~scanf("%d", &temp)) {
        if (temp == -1) break;
        printf("====erase %d from tree====\n", temp);
        root = erase(root, temp);
        output(root);
    }

    return 0;
}
