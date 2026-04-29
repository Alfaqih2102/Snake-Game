#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <string>

using namespace std;

// --- KONFIGURASI & STRUKTUR DATA ---
const int lebar = 20; 
const int tinggi = 15;

struct Poin { int x, y; };

// Perbaikan Bug: Menambahkan keyword 'enum'
enum Arah { DIAM = 0, KIRI, KANAN, ATAS, BAWAH };
Arah dir;

// Variabel Global
string namaPlayer;
Poin kepala, buah;
vector<Poin> ekor;
bool gameOver;
int skor;
int delayTime = 250; // Kecepatan awal

// --- SISTEM UTILITAS (WINDOWS API) ---

// Fungsi: Mematikan kursor kedip agar visual map bersih
void SembunyikanKursor() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
}

// Fungsi: Memindahkan kursor ke 0,0 tanpa menghapus layar (Anti-Flicker)
void ResetKursor() {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
}

// --- LOGIKA GAME ---

// Penjelasan: Menu awal untuk input nama
void TampilkanMenu() {
    system("cls");
    cout << "==================================" << endl;
    cout << "        SNAKE GAME ENGINE         " << endl;
    cout << "==================================" << endl;
    cout << " MASUKKAN NAMA ANDA: "; 
    getline(cin, namaPlayer);
    if(namaPlayer == "") namaPlayer = "User";
    
    cout << "\n Halo, " << namaPlayer << "!" << endl;
    cout << " Kontrol: W, A, S, D | X: Keluar" << endl;
    cout << " Tekan tombol apa saja untuk MULAI..." << endl;
    cout << "==================================" << endl;
    _getch();
    system("cls");
}

// Penjelasan: Reset variabel ke kondisi awal
void Setup() {
    SembunyikanKursor();
    gameOver = false;
    dir = DIAM;
    kepala = {lebar / 2, tinggi / 2};
    buah = {rand() % lebar, rand() % tinggi};
    skor = 0;
    ekor.clear();
    delayTime = 250;
}

// Penjelasan: Logika Kecepatan 4 Tahap sesuai permintaan
void UpdateKecepatan() {
    if (skor >= 10 && skor <= 30)       delayTime = 250; // Super Lambat
    else if (skor >= 40 && skor <= 80)  delayTime = 150; // Lambat
    else if (skor >= 90 && skor <= 200) delayTime = 80;  // Cepat
    else if (skor >= 210)               delayTime = 40;  // Super Cepat
}

// Penjelasan: Menggambar Map (Skala 1:1 dengan double character)
void Render() {
    ResetKursor();
    string map = "";
    
    for (int i = 0; i < lebar + 2; i++) map += "##"; 
    map += "\n";

    for (int i = 0; i < tinggi; i++) {
        for (int j = 0; j < lebar; j++) {
            if (j == 0) map += "##"; 

            if (i == kepala.y && j == kepala.x) map += "[]"; // Kepala
            else if (i == buah.y && j == buah.x) map += " @"; // Buah
            else {
                bool isEkor = false;
                for (auto& e : ekor) {
                    if (e.x == j && e.y == i) { map += "()"; isEkor = true; break; }
                }
                if (!isEkor) map += "  "; 
            }
            if (j == lebar - 1) map += "##"; 
        }
        map += "\n";
    }

    for (int i = 0; i < lebar + 2; i++) map += "##";
    map += "\n PLAYER: " + namaPlayer;
    map += " | SKOR: " + to_string(skor);
    map += " | STATUS: " + (delayTime < 100 ? (string)"TURBO" : (string)"NORMAL") + "\n";
    
    cout << map;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': if(dir != KANAN) dir = KIRI; break;
            case 'd': if(dir != KIRI) dir = KANAN; break;
            case 'w': if(dir != BAWAH) dir = ATAS; break;
            case 's': if(dir != ATAS) dir = BAWAH; break;
            case 'x': gameOver = true; break;
        }
    }
}

void Logika() {
    if (dir == DIAM) return;

    Poin posisiLama = kepala;

    switch (dir) {
        case KIRI:  kepala.x--; break;
        case KANAN: kepala.x++; break;
        case ATAS:  kepala.y--; break;
        case BAWAH: kepala.y++; break;
    }

    // Tabrak Tembok
    if (kepala.x >= lebar || kepala.x < 0 || kepala.y >= tinggi || kepala.y < 0)
        gameOver = true;

    // Tabrak Ekor
    for (auto& e : ekor) if (e.x == kepala.x && e.y == kepala.y) gameOver = true;

    // Makan Buah
    if (kepala.x == buah.x && kepala.y == buah.y) {
        skor += 10;
        UpdateKecepatan(); // Update transmisi gigi
        buah = {rand() % lebar, rand() % tinggi};
        ekor.insert(ekor.begin(), posisiLama);
    } else if (!ekor.empty()) {
        ekor.insert(ekor.begin(), posisiLama);
        ekor.pop_back();
    }
}

int main() {
    TampilkanMenu();
    Setup();
    
    while (!gameOver) {
        Render();
        Input();
        Logika();
        Sleep(delayTime); 
    }

    system("cls");
    cout << "==================================" << endl;
    cout << "           GAME OVER!             " << endl;
    cout << " PLAYER    : " << namaPlayer << endl;
    cout << " SKOR AKHIR: " << skor << endl;
    cout << "==================================" << endl;
    cout << " Tekan tombol apapun untuk keluar." << endl;
    _getch();
    
    return 0;
}