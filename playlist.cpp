#include "playlist.h"
#include <iostream>
using namespace std;

void createPlaylist(Playlist &Q) {
    Q.head = 0;
    Q.tail = 0;
    for (int i = 0; i <= MAXSONG; i++) Q.data[i] = NULL;
}

bool isQueueEmpty(Playlist Q) {
    return (Q.head == 0 && Q.tail == 0);
}

bool isQueueFull(Playlist Q) {
    return (Q.tail == MAXSONG);
}

void enqueue(Playlist &Q, infotypeQ x) {
    if (isQueueFull(Q)) {
        cout << "\nPlaylist penuh!\n";
        return;
    }

    if (isQueueEmpty(Q)) {
        Q.head = 1;
        Q.tail = 1;
    } else {
        Q.tail++;
    }
    Q.data[Q.tail] = x;
}

void dequeue(Playlist &Q) {
    if (isQueueEmpty(Q)) {
        cout << "\nPlaylist kosong!\n";
        return;
    }

    if (Q.head == Q.tail) {
        Q.head = Q.tail = 0;
    } else {
        for (int i = Q.head; i < Q.tail; i++)
            Q.data[i] = Q.data[i+1];
        Q.data[Q.tail] = NULL;
        Q.tail--;
    }
}

void showPlaylist(Playlist Q) {
    if (isQueueEmpty(Q)) {
        cout << "\nPlaylist kosong!\n";
        return;
    }

    cout << "\n=== PLAYLIST USER ===\n";
    for (int i = Q.head; i <= Q.tail; i++) {
        if (Q.data[i] != NULL)
            cout << i << ". " << Q.data[i]->info.title << " - " << Q.data[i]->info.artist << endl;
    }
}

void removeFromPlaylistByNode(Playlist &Q, address node) {
    if (isQueueEmpty(Q)) return;
    int i = Q.head;
    while (i <= Q.tail) {
        if (Q.data[i] == node) {
            // shift left from i
            for (int j = i; j < Q.tail; j++) Q.data[j] = Q.data[j + 1];
            Q.data[Q.tail] = NULL;
            Q.tail--;
        } else {
            i++;
        }
        if (Q.head > Q.tail) { Q.head = Q.tail = 0; break; }
    }
}

int findIndexInPlaylist(Playlist Q, address node) {
    if (isQueueEmpty(Q)) return 0;
    for (int i = Q.head; i <= Q.tail; i++) {
        if (Q.data[i] == node) return i;
    }
    return 0;
}
