// main.cpp
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "library.h"
#include "playlist.h"
#include "history.h"
#include "favorites.h"

using namespace std;

void adminMenu(List &library, Playlist &playlist, History &history, FavoriteBST &favorites); // Perbarui parameter
void userMenu(List &library, Playlist &playlist, History &history, FavoriteBST &favorites);   // Perbarui parameter

void removeFromAllPlaylistsAndHistory(Playlist &playlist, History &history, FavoriteBST &favorites, address node); // Perbarui parameter
void updatePlayCountAndFavorite(FavoriteBST &favorites, address songNode); // Fungsi baru

address findSimilarSong(List &library, address lastPlayed);

int main() {
    srand((unsigned)time(NULL));
    List library;
    Playlist playlist;
    History history;
    FavoriteBST favorites; // Inisialisasi BST baru

    createList(library);
    createPlaylist(playlist);
    createHistory(history);
    createFavoriteBST(favorites); // Buat BST

    // Inisialisasi lagu dengan playCount = 0 (sudah di library.cpp)
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

    int role;
    do {
        cout << "\n=== SELAMAT DATANG DI NODEBEAT ===\n";
        cout << "Pilih peran:\n";
        cout << "1. Admin\n";
        cout << "2. User\n";
        cout << "0. Keluar Program\n";
        cout << "Pilih: ";
        cin >> role;
        cin.ignore();

        if (role == 1) {
            string password;
            cout << "Masukkan password Admin: ";
            getline(cin, password);

            if (password == "Kelompok 3") {
                adminMenu(library, playlist, history, favorites);
            } else {
                cout << "Password salah. Akses ditolak.\n";
            }
        }
        else if (role == 2) {
            userMenu(library, playlist, history, favorites);
        }
        else if (role == 0) {
            cout << "\nTerima kasih telah menggunakan NODEBEAT 🎵\n";
        }
        else {
            cout << "Pilihan tidak valid.\n";
        }

    } while (role != 0);
    
    cout << "\nProgram selesai.\n";
    return 0;
}

void updatePlayCountAndFavorite(FavoriteBST &favorites, address songNode) {
    if (songNode == NULL) return;

    // Hapus dari BST lama (berdasarkan playCount lama)
    deleteFavorite(favorites, songNode);

    // Tambahkan playCount
    songNode->info.playCount++;

    // Masukkan kembali ke BST dengan playCount baru
    insertFavorite(favorites, songNode);
}


// --------------------------- ADMIN MENU ---------------------------
void adminMenu(List &library, Playlist &playlist, History &history, FavoriteBST &favorites) { // Perbarui
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
                // Hapus dari BST sebelum update (karena playCount tetap)
                deleteFavorite(favorites, node);

                infotype s = node->info;
                cout << "Data lama: " << s.id << " | " << s.title << " | " << s.artist << endl;
                cout << "Masukkan data baru (kosongkan untuk tidak mengubah):\n";
                string tmp;
                cout << "Judul (" << s.title << "): "; getline(cin, tmp); if (!tmp.empty()) s.title = tmp;
                cout << "Artist (" << s.artist << "): "; getline(cin, tmp); if (!tmp.empty()) s.artist = tmp;
                cout << "Genre (" << s.genre << "): "; getline(cin, tmp); if (!tmp.empty()) s.genre = tmp;
                cout << "Tahun (" << s.year << "): "; getline(cin, tmp); if (!tmp.empty()) s.year = stoi(tmp);
                // Kita tidak perlu update playCount di sini karena tidak diubah oleh Admin.
                updateSongByID(library, id, s);

                // Masukkan kembali ke BST (karena playCount yang dipertahankan tetap sama)
                insertFavorite(favorites, node);

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
                removeFromAllPlaylistsAndHistory(playlist, history, favorites, node); // Perbarui
                deleteByTitle(library, judul); // Ini harus membebaskan memory
                cout << "Lagu dihapus dari library dan semua playlist/history/favorit diupdate.\n";
            }
        }
    } while (pilih != 0);
}

// --------------------------- USER MENU ---------------------------
void userMenu(List &library, Playlist &playlist, History &history, FavoriteBST &favorites) { // Perbarui
    int pilih;
    bool isPlaying = false;
    address nowPlaying = NULL;
    int nowPlayingPlaylistIndex = 0;
    do {
        cout << "\n=== MENU USER ===\n";
        cout << "1. Cari lagu\n";
        cout << "2. Tambah lagu ke Playlist\n";
        cout << "3. Hapus Lagu dari Playlist\n";
        cout << "4. Putar / Stop lagu\n";
        cout << "5. Next Lagu\n";
        cout << "6. Prev Lagu\n";
        cout << "7. Tampilkan Playlist\n";
        cout << "8. Tampilkan History\n";
        cout << "9. Tampilkan Lagu Favorit\n"; // Tambah menu
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> pilih;
        cin.ignore();

        // ... (Logika cari lagu tetap sama)
        if (pilih == 1) {
            cout << "Cari berdasarkan:\n";
            cout << "1. ID\n";
            cout << "2. Judul\n";
            cout << "3. Artist\n";
            cout << "4. Genre\n";
            cout << "5. Tahun\n";
            cout << "\nPilihan: ";
            string t; getline(cin, t); // Pindah cin >> t; cin.ignore(); menjadi getline(cin, t)

            if (t == "1" || t == "ID") {
                int id; cout << "Masukkan ID: "; cin >> id; cin.ignore();
                address n = findByIDNode(library, id);
                if (n) cout << n->info.id << " | " << n->info.title << " | " << n->info.artist << " | Plays: " << n->info.playCount << endl;
                else cout << "Tidak ditemukan.\n";
            } else if (t == "2" || t == "Judul") {
                string judul; cout << "Masukkan Judul: "; getline(cin, judul);
                address n = findByTitleNode(library, judul);
                if (n) cout << n->info.id << " | " << n->info.title << " | " << n->info.artist << " | Plays: " << n->info.playCount << endl;
                else cout << "Tidak ditemukan.\n";
            } else if (t == "3" || t == "Artist") {
                string art; cout << "Masukkan Artist: "; getline(cin, art);
                address p = library.first;
                bool found=false;
                while (p) {
                    if (p->info.artist == art) {
                        cout << p->info.id << " | " << p->info.title << " | " << p->info.artist << " | Plays: " << p->info.playCount << endl;
                        found=true;
                    }
                    p=p->next;
                }
                if (!found) cout << "Tidak ditemukan.\n";
            } else if (t == "4" || t == "Genre") {
                string g; cout << "Masukkan Genre: "; getline(cin, g);
                address p = library.first; bool found=false;
                while (p) {
                    if (p->info.genre == g) {
                        cout << p->info.id << " | " << p->info.title << " | " << p->info.artist << " | Plays: " << p->info.playCount << endl;
                        found=true;
                    }
                    p=p->next;
                }
                if (!found) cout << "Tidak ditemukan.\n";
            } else if (t == "5" || t == "Tahun") {
                int thn; cout << "Masukkan Tahun: "; cin >> thn; cin.ignore();
                address p = library.first; bool found=false;
                while (p) {
                    if (p->info.year == thn) {
                        cout << p->info.id << " | " << p->info.title << " | " << p->info.artist << " | Plays: " << p->info.playCount << endl;
                        found=true;
                    }
                    p=p->next;
                }
                if (!found) cout << "Tidak ditemukan.\n";
            } else {
                cout << "\nPilihan anda tidak valid, silahkan coba lagi!\n";
            }
        }
        // ... (Logika enqueue, dequeue tetap sama)
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
        // ... (Logika Putar/Stop, Next, Prev, Tampilkan tetap sama, dengan penambahan updatePlayCountAndFavorite)
        else if (pilih == 4) {
            if (!isPlaying) {
                if (!isQueueEmpty(playlist)) {
                    nowPlaying = playlist.data[playlist.head];
                    nowPlayingPlaylistIndex = playlist.head;
                    cout << "Now Playing: " << nowPlaying->info.title << " - " << nowPlaying->info.artist << endl;
                    push(history, nowPlaying);
                    updatePlayCountAndFavorite(favorites, nowPlaying); // UPDATE PLAY COUNT & FAVORIT
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
                            updatePlayCountAndFavorite(favorites, nowPlaying); // UPDATE PLAY COUNT & FAVORIT
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
                        updatePlayCountAndFavorite(favorites, nowPlaying); // UPDATE PLAY COUNT & FAVORIT
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
                        updatePlayCountAndFavorite(favorites, nowPlaying); // UPDATE PLAY COUNT & FAVORIT
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
                            updatePlayCountAndFavorite(favorites, nowPlaying); // UPDATE PLAY COUNT & FAVORIT
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
                // Jangan panggil peekHistory, ambil 2 dari atas untuk "balik"
                // Pop lagu yang sedang diputar (yang terakhir didorong)
                pop(history);
                address prev = peekHistory(history);
                if (prev) {
                    nowPlaying = prev;
                    cout << "Now Playing (Previous): " << nowPlaying->info.title << " - " << nowPlaying->info.artist << endl;
                    // Tidak perlu push atau update playCount lagi karena sudah ada di history
                    isPlaying = true;
                    nowPlayingPlaylistIndex = 0;
                } else {
                    // Jika setelah pop stack kosong, reset nowPlaying
                    nowPlaying = NULL;
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
        else if (pilih == 9) { // Menu baru
            displayFavoriteSongs(favorites);
        }

    } while (pilih != 0);
}

void removeFromAllPlaylistsAndHistory(Playlist &playlist, History &history, FavoriteBST &favorites, address node) { // Perbarui
    removeFromPlaylistByNode(playlist, node);
    removeFromHistoryByNode(history, node);
    deleteFavorite(favorites, node); // Hapus dari BST favorit
}

address findSimilarSong(List &library, address lastPlayed) {
    if (lastPlayed == NULL) return NULL;
    // Cari yang genre dan artist sama (diutamakan)
    address p = library.first;
    while (p) {
        if (p != lastPlayed && p->info.artist == lastPlayed->info.artist && p->info.genre == lastPlayed->info.genre) return p;
        p = p->next;
    }
    // Cari yang artist sama
    p = library.first;
    while (p) {
        if (p != lastPlayed && p->info.artist == lastPlayed->info.artist) return p;
        p = p->next;
    }
    // Cari yang genre sama
    p = library.first;
    while (p) {
        if (p != lastPlayed && p->info.genre == lastPlayed->info.genre) return p;
        p = p->next;
    }
    return NULL;
}
