#include "favorites.h"
#include <iostream>

using namespace std;

void createFavoriteBST(FavoriteBST &T) {
    T.root = NULL;
}

BSTAddress createBSTNode(address songNode) {
    BSTAddress newNode = new BSTNode;
    newNode->songNode = songNode;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void insertNode(BSTAddress &root, BSTAddress newNode) {
    if (root == NULL) {
        root = newNode;
    } else if (newNode->songNode->info.playCount < root->songNode->info.playCount) {
        insertNode(root->left, newNode);
    } else {
        insertNode(root->right, newNode);
    }
}

void insertFavorite(FavoriteBST &T, address songNode) {
    if (songNode == NULL) return;
    BSTAddress newNode = createBSTNode(songNode);
    insertNode(T.root, newNode);
}

void displayInOrder(BSTAddress root) {
    if (root != NULL) {
        displayInOrder(root->left);
        cout << root->songNode->info.playCount << " plays | "
             << root->songNode->info.title << " - "
             << root->songNode->info.artist << endl;
        displayInOrder(root->right);
    }
}

void displayFavoriteSongs(FavoriteBST T) {
    if (T.root == NULL) {
        cout << "\nDaftar Lagu Favorit kosong.\n";
        return;
    }
    cout << "\n=== LAGU FAVORIT ===\n";
    displayInOrder(T.root);
}

BSTAddress findMin(BSTAddress root) {
    while (root && root->left != NULL) {
        root = root->left;
    }
    return root;
}

void deleteNode(BSTAddress &root, address songNode) {
    if (root == NULL) return;

    if (songNode->info.playCount < root->songNode->info.playCount) {
        deleteNode(root->left, songNode);
    } else if (songNode->info.playCount > root->songNode->info.playCount) {
        deleteNode(root->right, songNode);
    } else {
        if (root->songNode != songNode) {
            deleteNode(root->right, songNode);
            return;
        }

        BSTAddress temp;
        if (root->left == NULL) {
            temp = root->right;
            delete root;
            root = temp;
        } else if (root->right == NULL) {
            temp = root->left;
            delete root;
            root = temp;
        } else {
            temp = findMin(root->right);

            root->songNode = temp->songNode;

            deleteNode(root->right, temp->songNode);
        }
    }
}

void deleteFavorite(FavoriteBST &T, address songNode) {
    deleteNode(T.root, songNode);
}

