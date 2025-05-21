Tentu, ini dia contoh README.md untuk proyek Pemecah & Generator Labirin C++ yang sudah kita buat:
# Proyek Pemecah & Generator Labirin C++ (Maze Solver & Generator)

Proyek ini adalah implementasi program konsol C++ yang dapat menghasilkan labirin secara acak dan kemudian menemukan solusi (jalur) dari titik awal ke titik akhir menggunakan algoritma DFS (Depth-First Search) atau BFS (Breadth-First Search).

## Fitur Utama

* *Generator Labirin:* Menggunakan algoritma Recursive Backtracker (berbasis DFS) untuk membuat labirin secara acak.
* *Pemecah Labirin:*
    * Implementasi *Depth-First Search (DFS)* untuk menemukan jalur.
    * Implementasi *Breadth-First Search (BFS)* untuk menemukan jalur terpendek.
* *Representasi Grid 2D:* Labirin direpresentasikan sebagai grid sel.
* *Input Dinamis:* Pengguna dapat menentukan dimensi labirin.
* *Pilihan Algoritma Solver:* Pengguna dapat memilih antara DFS atau BFS untuk memecahkan labirin.
* *Visualisasi Konsol:* Labirin dan jalur solusi ditampilkan di konsol, dengan penggunaan warna (menggunakan ANSI escape codes) untuk membedakan tembok, jalan, titik Start (S), titik End (E), dan jalur solusi (\*).
* *Penyesuaian Dimensi:* Otomatis menyesuaikan dimensi input menjadi ganjil untuk kualitas generasi labirin yang lebih baik dengan algoritma yang digunakan.

## Struktur Data yang Digunakan

* **std::vector<std::vector<Cell>> (Grid 2D):** Untuk merepresentasikan struktur utama labirin. Setiap Cell adalah objek yang menyimpan statusnya (tembok/jalan, dikunjungi, koordinat, parent untuk path).
* **std::stack<Cell*>:** Digunakan dalam implementasi algoritma DFS untuk solving.
* **std::queue<Cell*>:** Digunakan dalam implementasi algoritma BFS untuk solving.
* *Rekursi (Call Stack Implisit):* Digunakan dalam algoritma Recursive Backtracker untuk generasi labirin.
* **Cell* parent (dalam struct Cell):** Digunakan untuk melacak jalur saat solving dan untuk merekonstruksi jalur solusi.
* **std::vector<Cell*>:** Digunakan untuk menyimpan dan mengelola jalur solusi yang telah direkonstruksi.

## Algoritma yang Digunakan

* *Generasi Labirin:*
    * *Recursive Backtracker (Depth-First Search):* Algoritma klasik untuk menghasilkan labirin yang sempurna (setiap sel dapat dijangkau).
* *Pemecahan Labirin:*
    * *Depth-First Search (DFS):* Menemukan jalur dari start ke end, namun tidak selalu jalur terpendek.
    * *Breadth-First Search (BFS):* Menjamin penemuan jalur terpendek (dalam hal jumlah langkah/sel).

## Cara Kompilasi

Pastikan Anda memiliki compiler C++ (seperti g++). Simpan kode sebagai maze_project.cpp (atau nama lain) dan kompilasi menggunakan perintah berikut di terminal:

```
g++ maze_project.cpp -o maze_project -std=c++11
```

Jika Anda menggunakan fitur C++ yang lebih baru, Anda bisa mengganti -std=c++11 dengan -std=c++14, -std=c++17, dst.
Cara Menjalankan Program
Setelah kompilasi berhasil, jalankan program yang telah dieksekusi:
./maze_project

(Pada Windows, Anda mungkin menjalankan maze_project.exe)
Penggunaan Program
 * Input Dimensi: Program akan meminta Anda memasukkan dimensi labirin (jumlah baris dan kolom). Disarankan menggunakan angka ganjil (misalnya, 21 31) untuk hasil generasi terbaik.
 * Labirin Dihasilkan: Program akan secara otomatis menghasilkan dan menampilkan labirin. Titik Start (S) dan End (E) default akan ditentukan (biasanya S di (1,1) dan E di dekat pojok kanan bawah).
 * Pilih Solver: Anda akan diminta memilih algoritma untuk memecahkan labirin:
   * Ketik 1 untuk DFS.
   * Ketik 2 untuk BFS.
 * Hasil Solusi: Program akan menampilkan labirin lagi, dengan jalur solusi (jika ditemukan) ditandai dengan karakter * berwarna hijau. Jika tidak ada solusi, pesan akan ditampilkan.
Tampilan Output
 * #: Merepresentasikan tembok.
 * (spasi): Merepresentasikan jalan.
 * S: Titik awal (Start), ditampilkan dengan warna biru.
 * E: Titik akhir (End), ditampilkan dengan warna merah.
 * *: Jalur solusi yang ditemukan, ditampilkan dengan warna hijau.
