/*
 * @Author: LZH
 * @Date: 2022-04-26 15:58:05
 * @LastEditTime: 2022-04-28 15:41:32
 * @Description: 
 * @FilePath: /MyFiles/6_高级数据结构/data_structure/5_string_match.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1000
#define DEFAULT_LEN 30

char s[MAX_N + 5], t[MAX_N + 5];

#define TSET_FUN(func) {\
    char temp[MAX_N + 5];\
    sprintf(temp, "%s match index :%3d.", #func, func(s, t));\
    int n = DEFAULT_LEN - strlen(temp);\
    while (n >= 0) n -= printf(" ");\
    printf("%s\n", temp);\
}

int brute_one_match(const char *s, const char *t) {
    int flag = 1;
    printf("call brute once.\n");
    for (int j = 0; t[j] && s[j]; j++) {
        if (s[j] != t[j]) {
            flag = 0;
            break;
        }
    }
    return flag;
}

int brute_force(const char *s, const char *t) {
    int ind = 0;
    for (int i = 0; s[i]; i++) {
        if (brute_one_match(s + i, t)) return i;
    }
    return -1;
}

int quick_mod(int a, int b, int c) {
    int ans = 1, temp = a;
    while (b) {
        if (b & 1) ans = ans * temp % c;
        temp = temp * temp % c;
        b >>= 1;
    }
    return ans;
}

int hash_match(const char *s, const char *t) {
    int len = strlen(t), base = 31, P = 9973, nbase = quick_mod(base, len, P);
    int h = 0, th = 0;
    for (int i = 0; t[i]; i++) th = (t[i] + th * base) % P;
    for (int i = 0; s[i]; i++) {
        h = (s[i] + h * base) % P;
        if (i >= len)  h = (h - (nbase * s[i - len] % P) + P) % P;
        if (i + 1 <  len) continue;
        if (h != th) continue;
        if (brute_one_match(s + i - len + 1, t)) return i - len + 1;
    }
    return -1;
}

int *getNext(const char *t, int *len) {
    *len = strlen(t);
    int *next = (int *)malloc(sizeof(int) * strlen(t));
    next[0] = -1;
    for (int i = 1, j = -1; i < *len; i++) {
        while (j != -1 && t[i] != t[j + 1]) j = next[j];
        if (t[i] == t[j + 1]) j += 1;
        next[i] = j;
    }
    return next;
}

void free_next(int *next) {
    free(next);
    return ;
}

void free_jump(int **jump, int n) {
    for (int i = 0; i < n; i++) free(jump[i - 1]);
    free(jump - 1);
    return ;
}

int **getJump(int *next,  const char *t, int n) {
    int **jump = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++) jump[i] = (int *)malloc(sizeof(int) * 26);
    jump += 1;
    for (int i = 0; i < 26; i++) jump[-1][i] = -1; 
    jump[-1][t[0] - 'a'] = 0;
    for (int i = 0, I = n - 1; i < I; i++) {
        for (int j = 0; j < 26; j++) jump[i][j] = jump[next[i]][j];
        jump[i][t[i + 1] - 'a'] = i + 1;
    }
    return jump;
}

int kmp(const char *s, const char *t) {
    int len;
    int *next = getNext(t, &len);
    for (int i = 0, j = -1; s[i]; i++) {
        while (j != -1 && t[j + 1] != s[i]) j = next[j];
        if (t[j + 1] == s[i]) j += 1;
        if (t[j + 1] == '\0') {
            free_next(next);
            return i - len + 1;
        }
    }
    free_next(next);
    return -1;
}


int kmp_opt(const char *s, const char *t) {
    int len;
    int *next = getNext(t, &len);
    int **jump = getJump(next, t, len);
    for (int i = 0, j = -1; s[i]; i++) {
        j = jump[j][s[i] - 'a'];
        if (j == len - 1) {
            free_next(next);
            free_jump(jump, len);
            return i - len + 1;
        }
    }
    free_next(next);
    free_jump(jump, len);
    return -1;
}

int sunday(const char *s, const char *t) {
    int tlen = strlen(t), slen = strlen(s);
    int jump[128] = {0};
    for (int i = 0; i < 128; i++) jump[i] = tlen + 1;
    for (int i = 0; t[i]; i++) jump[t[i]] = tlen - i;
    for (int i = 0; i + tlen <= slen;) {
        if (brute_one_match(s + i, t)) return i;
        i += jump[s[i + tlen]];
    }
    return -1;
}

int shift_and(const char *s, const char *t) {
    int code[128] = {0}, n = 0;
    for (; t[n]; n++) code[t[n]] |= (1 << n);
    int p = 0;
    for (int i = 0; s[i]; i++) {
        p = (p << 1 | 1) & code[s[i]];
        if (p & (1 << (n - 1))) return i - n + 1;
    }
    return -1;
}

int main(int argc, char const *argv[]) {
    scanf("%s %s", s, t);
    TSET_FUN(brute_force);
    TSET_FUN(hash_match);
    TSET_FUN(kmp);
    TSET_FUN(kmp_opt);
    TSET_FUN(sunday);
    TSET_FUN(shift_and);
    return 0;
}

