#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "library.h"
#define MAXSONG 10

typedef address infotypeQ;

struct Playlist {
    infotypeQ data[MAXSONG + 1];
    int head;
    int tail;
};

void createPlaylist(Playlist &Q);
bool isQueueEmpty(Playlist Q);
bool isQueueFull(Playlist Q);
void enqueue(Playlist &Q, infotypeQ x);
void dequeue(Playlist &Q);
void showPlaylist(Playlist Q);
void removeFromPlaylistByNode(Playlist &Q, address node);
int findIndexInPlaylist(Playlist Q, address node);
#endif
