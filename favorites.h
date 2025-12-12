// favorite_song.h
#ifndef FAVORITE_H
#define FAVORITE_H

#include "library.h"

// Node untuk Binary Search Tree
typedef struct BSTNode *BSTAddress;

struct BSTNode {
    address songNode; // Pointer ke Node lagu di Library (Double Linked List)
    BSTAddress left;
    BSTAddress right;
};

// Struktur BST
struct FavoriteBST {
    BSTAddress root;
};

void createFavoriteBST(FavoriteBST &T);
BSTAddress createBSTNode(address songNode);
void insertFavorite(FavoriteBST &T, address songNode);
void insertNode(BSTAddress &root, BSTAddress newNode);
void displayInOrder(BSTAddress root);
void displayFavoriteSongs(FavoriteBST T);
void deleteFavorite(FavoriteBST &T, address songNode);
void deleteNode(BSTAddress &root, address songNode);
BSTAddress findMin(BSTAddress root);

#endif

