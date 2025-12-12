#ifndef HISTORY_H
#define HISTORY_H

#include "library.h"
#define MAXSONG 10

typedef address infotypeS;

struct History {
    infotypeS data[MAXSONG + 1];
    int top;
};

void createHistory(History &S);
bool isStackEmpty(History S);
bool isStackFull(History S);
void push(History &S, infotypeS x);
void pop(History &S);
void showHistory(History S);
void removeFromHistoryByNode(History &S, address node);
address peekHistory(History S);
#endif
