#include "library.h"

void createList(List &L) {
    L.first = NULL;
    L.last = NULL;
}

bool isEmpty(List L) {
    return (L.first == NULL);
}

void insertSorted(List &L, infotype lagu) {
    address P = new Node;
    P->info = lagu;
    P->next = NULL;
    P->prev = NULL;

    if (isEmpty(L)) {
        L.first = L.last = P;
    }
    else if (lagu.id < L.first->info.id) {
        P->next = L.first;
        L.first->prev = P;
        L.first = P;
    }
    else {
        address Q = L.first;
        while (Q->next != NULL && Q->next->info.id < lagu.id) {
            Q = Q->next;
        }
        P->next = Q->next;
        P->prev = Q;
        if (Q->next != NULL)
            Q->next->prev = P;
        else
            L.last = P;
        Q->next = P;
    }
}

void deleteByTitle(List &L, string title) {
    if (isEmpty(L)) return;

    address P = L.first;
    while (P != NULL && P->info.title != title)
        P = P->next;

    if (P == NULL) return;

    if (P == L.first && P == L.last) {
        L.first = L.last = NULL;
    }
    else if (P == L.first) {
        L.first = P->next;
        if (L.first) L.first->prev = NULL;
    }
    else if (P == L.last) {
        L.last = P->prev;
        if (L.last) L.last->next = NULL;
    }
    else {
        P->prev->next = P->next;
        P->next->prev = P->prev;
    }
}

void displayForward(List L) {
    if (isEmpty(L)) {
        cout << "\nTidak ada lagu di library\n";
        return;
    }
    address P = L.first;
    cout << "\n=== LIBRARY (ASC ID) ===\n";
    while (P != NULL) {
        cout << P->info.id << " | " << P->info.title
             << " | " << P->info.artist
             << " | " << P->info.genre
             << " | " << P->info.year << endl;
        P = P->next;
    }
}

void reverseList(List &L) {
    if (isEmpty(L)) return;
    address P = L.first;
    address temp = NULL;

    while (P != NULL) {
        temp = P->prev;
        P->prev = P->next;
        P->next = temp;
        P = P->prev;
    }
    temp = L.first;
    L.first = L.last;
    L.last = temp;
}

address findByTitleNode(List L, string title) {
    address P = L.first;
    while (P != NULL) {
        if (P->info.title == title) return P;
        P = P->next;
    }
    return NULL;
}

address findByIDNode(List L, int id) {
    address P = L.first;
    while (P != NULL) {
        if (P->info.id == id) return P;
        P = P->next;
    }
    return NULL;
}

void updateSongByID(List &L, int id, infotype newData) {
    address P = findByIDNode(L, id);
    if (P != NULL) {
        P->info = newData;
    }
}
