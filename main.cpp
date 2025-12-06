#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "library.h"
#include "playlist.h"
#include "history.h"

using namespace std;

void adminMenu(List &library, Playlist &playlist, History &history);
void userMenu(List &library, Playlist &playlist, History &history);

void removeFromAllPlaylistsAndHistory(Playlist &playlist, History &history, address node);

address findSimilarSong(List &library, address lastPlayed);

int main() {
    srand((unsigned)time(NULL));
    List library;
    Playlist playlist;
    History history;

    createList(library);
    createPlaylist(playlist);
    createHistory(history);


    insertSorted(library, {1, "Fix You", "Coldplay", "Pop", 2005});
    insertSorted(library, {2, "Bohemian Rhapsody", "Queen", "Rock", 1975});
    insertSorted(library, {3, "Shape of You", "Ed Sheeran", "Pop", 2017});
    insertSorted(library, {4, "Believer", "Imagine Dragons", "Rock", 2017});
    insertSorted(library, {5, "Attention", "Charlie Puth", "Pop", 2017});
    insertSorted(library, {6, "Lovely", "Billie Eilish", "Indie", 2018});
    insertSorted(library, {7, "Someone Like You", "Adele", "Pop", 2011});
    insertSorted(library, {8, "Night Changes", "One Direction", "Pop", 2014});
    insertSorted(library, {9, "Faded", "Alan Walker", "EDM", 2015});
    insertSorted(library, {10,"Sugar", "Maroon 5", "Pop", 2015});

    cout << "=== SELAMAT DATANG DI NODEBEAT ===\n";
    cout << "Pilih peran:\n1. Admin\n2. User\nPilih: ";
    int role;
    cin >> role;
    cin.ignore();

    if (role == 1) {

        string password;
        cout << "Masukkan password Admin: ";
        getline(cin, password);

        if (password == "Kelompok 3") {
            adminMenu(library, playlist, history);
        } else {
            cout << "Password salah. Akses ditolak.\n";
        }
    } else if (role == 2) {
        userMenu(library, playlist, history);
    } else {
        cout << "Pilihan tidak valid. Program keluar.\n";
    }

    cout << "\nProgram selesai.\n";
    return 0;
}

// --------------------------- ADMIN MENU ---------------------------
void adminMenu(List &library, Playlist &playlist, History &history) {
    int pilih;
    do {
        cout << "\n=== MENU ADMIN ===\n";
        cout << "1. Tambah lagu ke Library\n";
        cout << "2. Lihat semua lagu di Library\n";
        cout << "3. Ubah data lagu\n";
        cout << "4. Hapus data lagu\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> pilih;
        cin.ignore();

        if (pilih == 1) {
            infotype s;
            cout << "ID: "; cin >> s.id; cin.ignore();
            cout << "Judul: "; getline(cin, s.title);
            cout << "Artist: "; getline(cin, s.artist);
            cout << "Genre: "; getline(cin, s.genre);
            cout << "Tahun: "; cin >> s.year; cin.ignore();
            insertSorted(library, s);
            cout << "Lagu ditambahkan ke library.\n";
        }
        else if (pilih == 2) {
            displayForward(library);
        }
        else if (pilih == 3) {
            int id;
            cout << "Masukkan ID lagu yang ingin diubah: ";
            cin >> id; cin.ignore();
            address node = findByIDNode(library, id);
            if (node == NULL) {
                cout << "Lagu tidak ditemukan.\n";
            } else {
                infotype s = node->info;
                cout << "Data lama: " << s.id << " | " << s.title << " | " << s.artist << endl;
                cout << "Masukkan data baru (kosongkan untuk tidak mengubah):\n";
                string tmp;
                cout << "Judul (" << s.title << "): "; getline(cin, tmp); if (!tmp.empty()) s.title = tmp;
                cout << "Artist (" << s.artist << "): "; getline(cin, tmp); if (!tmp.empty()) s.artist = tmp;
                cout << "Genre (" << s.genre << "): "; getline(cin, tmp); if (!tmp.empty()) s.genre = tmp;
                cout << "Tahun (" << s.year << "): "; getline(cin, tmp); if (!tmp.empty()) s.year = stoi(tmp);
                updateSongByID(library, id, s);
                cout << "Data lagu berhasil diperbarui. Perubahan otomatis tercermin di playlist & history.\n";
            }
        }
        else if (pilih == 4) {
            string judul;
            cout << "Masukkan judul lagu yang ingin dihapus: ";
            getline(cin, judul);
            address node = findByTitleNode(library, judul);
            if (node == NULL) {
                cout << "Lagu tidak ditemukan.\n";
            } else {
                removeFromAllPlaylistsAndHistory(playlist, history, node);
                deleteByTitle(library, judul);
                cout << "Lagu dihapus dari library dan semua playlist/history diupdate.\n";
            }
        }

    } while (pilih != 0);
}

// --------------------------- USER MENU ---------------------------
void userMenu(List &library, Playlist &playlist, History &history) {
    int pilih;
    bool isPlaying = false;
    address nowPlaying = NULL;
    int nowPlayingPlaylistIndex = 0;
    do {
        cout << "\n=== MENU USER ===\n";
        cout << "1. Cari lagu\n";
        cout << "2. Tambah lagu ke Playlist\n";
        cout << "3. Hapus Lagu dari Playlist (dequeue)\n";
        cout << "4. Putar / Stop lagu\n";
        cout << "5. Next Lagu\n";
        cout << "6. Prev Lagu\n";
        cout << "7. Tampilkan Playlist\n";
        cout << "8. Tampilkan History\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> pilih;
        cin.ignore();

        if (pilih == 1) {
            cout << "Cari berdasarkan:\n";
            cout << "1. ID\n";
            cout << "2. Judul\n";
            cout << "3. Artist\n";
            cout << "4. Genre\n";
            cout << "5. Tahun\n";
            cout << "\nPilahan: ";
            string t; cin >> t; cin.ignore();
            if (t == "ID") {
                int id; cout << "Masukkan ID: "; cin >> id; cin.ignore();
                address n = findByIDNode(library, id);
                if (n) cout << n->info.id << " | " << n->info.title << " | " << n->info.artist << endl;
                else cout << "Tidak ditemukan.\n";
            } else if (t == "Judul") {
                string judul; cout << "Masukkan Judul: "; getline(cin, judul);
                address n = findByTitleNode(library, judul);
                if (n) cout << n->info.id << " | " << n->info.title << " | " << n->info.artist << endl;
                else cout << "Tidak ditemukan.\n";
            } else if (t == "Artist") {
                string art; cout << "Masukkan Artist: "; getline(cin, art);
                address p = library.first;
                bool found=false;
                while (p) {
                    if (p->info.artist == art) {
                        cout << p->info.id << " | " << p->info.title << " | " << p->info.artist << endl;
                        found=true;
                    }
                    p=p->next;
                }
                if (!found) cout << "Tidak ditemukan.\n";
            } else if (t == "Genre") {
                string g; cout << "Masukkan Genre: "; getline(cin, g);
                address p = library.first; bool found=false;
                while (p) {
                    if (p->info.genre == g) {
                        cout << p->info.id << " | " << p->info.title << " | " << p->info.artist << endl;
                        found=true;
                    }
                    p=p->next;
                }
                if (!found) cout << "Tidak ditemukan.\n";
            } else if (t == "Tahun") {
                int thn; cout << "Masukkan Tahun: "; cin >> thn; cin.ignore();
                address p = library.first; bool found=false;
                while (p) {
                    if (p->info.year == thn) {
                        cout << p->info.id << " | " << p->info.title << " | " << p->info.artist << endl;
                        found=true;
                    }
                    p=p->next;
                }
                if (!found) cout << "Tidak ditemukan.\n";
            } else {
                cout << "\nPilihan anda tidak valid, silahkan coba lagi!\n";
            }
        }
        else if (pilih == 2) {
            cout << "Masukkan judul lagu untuk ditambahkan ke playlist: ";
            string judul; getline(cin, judul);
            address node = findByTitleNode(library, judul);
            if (node == NULL) cout << "Lagu tidak ditemukan di library.\n";
            else enqueue(playlist, node);
        }
        else if (pilih == 3) {
            dequeue(playlist);
        }
        else if (pilih == 4) {
            if (!isPlaying) {
                if (!isQueueEmpty(playlist)) {
                    nowPlaying = playlist.data[playlist.head];
                    nowPlayingPlaylistIndex = playlist.head;
                    cout << "Now Playing: " << nowPlaying->info.title << " - " << nowPlaying->info.artist << endl;
                    push(history, nowPlaying);
                    dequeue(playlist);
                    isPlaying = true;
                } else {
                    cout << "Playlist kosong. Mau putar lagu dari library? (y/n): ";
                    char c; cin >> c; cin.ignore();
                    if (c == 'y' || c == 'Y') {
                        cout << "Masukkan judul: "; string jud; getline(cin, jud);
                        address node = findByTitleNode(library, jud);
                        if (node) {
                            nowPlaying = node;
                            nowPlayingPlaylistIndex = 0;
                            cout << "Now Playing: " << nowPlaying->info.title << " - " << nowPlaying->info.artist << endl;
                            push(history, nowPlaying);
                            isPlaying = true;
                        } else {
                            cout << "Lagu tidak ditemukan.\n";
                        }
                    }
                }
            } else {
                cout << "Stop: ";
                if (nowPlaying) cout << nowPlaying->info.title << " - " << nowPlaying->info.artist << endl;
                isPlaying = false;
            }
        }
        else if (pilih == 5) {
            if (isPlaying && nowPlaying != NULL) {
                if (nowPlayingPlaylistIndex != 0) {
                    if (!isQueueEmpty(playlist)) {
                        nowPlaying = playlist.data[playlist.head];
                        nowPlayingPlaylistIndex = playlist.head;
                        cout << "Now Playing (Next): " << nowPlaying->info.title << " - " << nowPlaying->info.artist << endl;
                        push(history, nowPlaying);
                        dequeue(playlist);
                        isPlaying = true;
                    } else {
                        cout << "Tidak ada lagu berikutnya di playlist.\n";
                        isPlaying = false;
                        nowPlaying = NULL;
                        nowPlayingPlaylistIndex = 0;
                    }
                } else {
                    address sim = findSimilarSong(library, nowPlaying);
                    if (sim) {
                        nowPlaying = sim;
                        cout << "Now Playing (Next similar): " << nowPlaying->info.title << " - " << nowPlaying->info.artist << endl;
                        push(history, nowPlaying);
                        isPlaying = true;
                        nowPlayingPlaylistIndex = 0;
                    } else {
                        address rnd = library.first;
                        int count = 0;
                        address p = library.first;
                        while (p) { count++; p = p->next; }
                        if (count == 0) { cout << "Library kosong.\n"; isPlaying=false; nowPlaying=NULL; }
                        else {
                            int r = rand() % count;
                            p = library.first;
                            for (int i=0;i<r;i++) p = p->next;
                            nowPlaying = p;
                            cout << "Now Playing (Random fallback): " << nowPlaying->info.title << " - " << nowPlaying->info.artist << endl;
                            push(history, nowPlaying);
                            isPlaying = true;
                            nowPlayingPlaylistIndex = 0;
                        }
                    }
                }
            } else {
                cout << "Belum ada lagu yang sedang diputar.\n";
            }
        }
        else if (pilih == 6) {
            if (isStackEmpty(history) || history.top < 2) {
                cout << "Tidak ada lagu sebelumnya.\n";
            } else {
                address current = peekHistory(history);
                address prev = history.data[history.top - 1];
                if (prev) {
                    nowPlaying = prev;
                    cout << "Now Playing (Previous): " << nowPlaying->info.title << " - " << nowPlaying->info.artist << endl;
                    isPlaying = true;
                    nowPlayingPlaylistIndex = 0;
                } else {
                    cout << "Tidak ada lagu sebelumnya.\n";
                }
            }
        }
        else if (pilih == 7) {
            showPlaylist(playlist);
        }
        else if (pilih == 8) {
            showHistory(history);
        }

    } while (pilih != 0);
}

void removeFromAllPlaylistsAndHistory(Playlist &playlist, History &history, address node) {
    removeFromPlaylistByNode(playlist, node);
    removeFromHistoryByNode(history, node);
}

address findSimilarSong(List &library, address lastPlayed) {
    if (lastPlayed == NULL) return NULL;
    address p = library.first;
    while (p) {
        if (p != lastPlayed && p->info.artist == lastPlayed->info.artist) return p;
        p = p->next;
    }
    p = library.first;
    while (p) {
        if (p != lastPlayed && p->info.genre == lastPlayed->info.genre) return p;
        p = p->next;
    }
    return NULL;
}
