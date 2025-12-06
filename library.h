#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
using namespace std;

struct Song {
    int id;
    string title;
    string artist;
    string genre;
    int year;
};

typedef Song infotype;
typedef struct Node *address;

struct Node {
    infotype info;
    address next;
    address prev;
};

struct List {
    address first;
    address last;
};

void createList(List &L);
bool isEmpty(List L);
void insertSorted(List &L, infotype lagu);
void deleteByTitle(List &L, string title);
void displayForward(List L);
void reverseList(List &L);
address findByTitleNode(List L, string title);
address findByIDNode(List L, int id);
void updateSongByID(List &L, int id, infotype newData);

#endif
