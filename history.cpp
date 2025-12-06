#include "history.h"
#include <iostream>
using namespace std;

void createHistory(History &S) {
    S.top = 0;
    for (int i = 0; i <= MAXSONG; i++) S.data[i] = NULL;
}

bool isStackEmpty(History S) {
    return (S.top == 0);
}

bool isStackFull(History S) {
    return (S.top == MAXSONG);
}

void push(History &S, infotypeS x) {
    if (isStackFull(S)) {
        cout << "\nHistory penuh!\n";
        return;
    }
    S.top++;
    S.data[S.top] = x;
}

void pop(History &S) {
    if (isStackEmpty(S)) {
        cout << "\nHistory kosong!\n";
        return;
    }
    S.data[S.top] = NULL;
    S.top--;
}

void showHistory(History S) {
    if (isStackEmpty(S)) {
        cout << "\nHistory kosong!\n";
        return;
    }
    cout << "\n=== HISTORY PEMUTARAN LAGU ===\n";
    for (int i = S.top; i >= 1; i--) {
        if (S.data[i] != NULL)
            cout << i << ". " << S.data[i]->info.title << " - " << S.data[i]->info.artist << endl;
    }
}

void removeFromHistoryByNode(History &S, address node) {
    if (isStackEmpty(S)) return;
    int write = 0;
    for (int read = 1; read <= S.top; read++) {
        if (S.data[read] != node) {
            write++;
            S.data[write] = S.data[read];
        }
    }
    for (int k = write+1; k <= S.top; k++) S.data[k] = NULL;
    S.top = write;
}

address peekHistory(History S) {
    if (isStackEmpty(S)) return NULL;
    return S.data[S.top];
}
