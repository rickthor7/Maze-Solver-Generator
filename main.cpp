#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <random>   // Untuk std::mt19937, std::uniform_int_distribution
#include <algorithm> // Untuk std::shuffle
#include <chrono>    // Untuk seed RNG

// Struktur untuk merepresentasikan satu sel dalam labirin
struct Cell {
    int row, col;
    bool is_wall = true; // Awalnya semua adalah tembok
    bool visited = false; // Untuk algoritma generator & solver
    Cell* parent = nullptr; // Untuk rekonstruksi jalur pada solver

    Cell(int r = 0, int c = 0) : row(r), col(c) {}
};

class Maze {
public:
    Maze(int r, int c) : rows(r), cols(c) {
        // Pastikan dimensi ganjil untuk generator Recursive Backtracker agar lebih rapi
        if (rows % 2 == 0) rows++;
        if (cols % 2 == 0) cols++;

        grid.resize(rows, std::vector<Cell>(cols));
        initialize_grid();

        // Initialize Random Number Generator
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        rng = std::mt19937(seed);
    }

    // Inisialisasi grid, semua sel adalah tembok dan belum dikunjungi
    void initialize_grid() {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                grid[r][c] = Cell(r, c);
                grid[r][c].is_wall = true;
                grid[r][c].visited = false;
                grid[r][c].parent = nullptr;
            }
        }
    }

    // Reset status visited dan parent untuk sel, berguna sebelum menjalankan solver
    void reset_cell_states() {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                grid[r][c].visited = false;
                grid[r][c].parent = nullptr;
            }
        }
    }

    // Menampilkan labirin ke konsol
    void display_maze(const std::vector<Cell*>& path = {}) const {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                bool is_path_cell = false;
                for (const auto& p_cell : path) {
                    if (p_cell->row == r && p_cell->col == c) {
                        is_path_cell = true;
                        break;
                    }
                }

                if (is_path_cell) {
                    std::cout << "\033[1;32m*\033[0m "; // Path cell (hijau)
                } else if (grid[r][c].is_wall) {
                    std::cout << "# "; // Tembok
                } else if (start_node && start_node->row == r && start_node->col == c) {
                    std::cout << "\033[1;34mS\033[0m "; // Start (biru)
                } else if (end_node && end_node->row == r && end_node->col == c) {
                    std::cout << "\033[1;31mE\033[0m "; // End (merah)
                }
                else {
                    std::cout << "  "; // Jalan
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // Cek apakah koordinat (r, c) valid (dalam batas grid)
    bool is_safe(int r, int c) const {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }

    // Algoritma Recursive Backtracker untuk menghasilkan labirin
    void generate_maze_recursive_backtracker(int r, int c) {
        grid[r][c].visited = true;
        grid[r][c].is_wall = false; // Sel ini adalah jalan

        // Arah pergerakan: Atas, Kanan, Bawah, Kiri (dalam langkah 2 sel)
        int dr[] = {-2, 0, 2, 0};
        int dc[] = {0, 2, 0, -2};
        
        // Acak urutan arah
        std::vector<int> directions = {0, 1, 2, 3};
        std::shuffle(directions.begin(), directions.end(), rng);

        for (int i = 0; i < 4; ++i) {
            int dir_idx = directions[i];
            int nr = r + dr[dir_idx]; // next row
            int nc = c + dc[dir_idx]; // next col

            if (is_safe(nr, nc) && !grid[nr][nc].visited) {
                // Hancurkan tembok di antara sel saat ini dan sel tetangga
                int wall_r = r + dr[dir_idx] / 2;
                int wall_c = c + dc[dir_idx] / 2;
                grid[wall_r][wall_c].is_wall = false;
                grid[wall_r][wall_c].visited = true; // Tandai tembok yg dihancurkan sbg visited juga (optional)

                generate_maze_recursive_backtracker(nr, nc);
            }
        }
    }
    
    void generate_maze() {
        // Reset grid ke kondisi awal (semua tembok, belum dikunjungi)
        initialize_grid(); 
        // Mulai dari sel (1,1) agar ada border tembok di sekeliling
        generate_maze_recursive_backtracker(1, 1);
        reset_cell_states(); // Penting untuk solver nanti
    }


    // Rekonstruksi jalur dari sel akhir ke sel awal menggunakan parent pointers
    std::vector<Cell*> reconstruct_path(Cell* end_cell_ptr) {
        std::vector<Cell*> path;
        Cell* current = end_cell_ptr;
        while (current != nullptr) {
            path.push_back(current);
            current = current->parent;
        }
        std::reverse(path.begin(), path.end()); // Balikkan agar dari start ke end
        return path;
    }

    // Menyelesaikan labirin menggunakan DFS
    std::vector<Cell*> solve_dfs(int start_r, int start_c, int end_r, int end_c) {
        reset_cell_states();
        if (!is_safe(start_r, start_c) || !is_safe(end_r, end_c) || 
            grid[start_r][start_c].is_wall || grid[end_r][end_c].is_wall) {
            std::cout << "Start atau End point tidak valid atau berada di tembok." << std::endl;
            return {};
        }

        start_node = &grid[start_r][start_c];
        end_node = &grid[end_r][end_c];

        std::stack<Cell*> s;
        s.push(start_node);
        start_node->visited = true;

        // Arah: Bawah, Kanan, Atas, Kiri (urutan bisa mempengaruhi path yang ditemukan)
        int dr[] = {1, 0, -1, 0}; 
        int dc[] = {0, 1, 0, -1};

        while (!s.empty()) {
            Cell* current = s.top();
            // Tidak langsung pop, agar bisa backtrack jika tidak ada tetangga valid dari current

            if (current->row == end_r && current->col == end_c) {
                return reconstruct_path(current); // Solusi ditemukan
            }

            bool moved = false;
            for (int i = 0; i < 4; ++i) {
                int nr = current->row + dr[i];
                int nc = current->col + dc[i];

                if (is_safe(nr, nc) && !grid[nr][nc].is_wall && !grid[nr][nc].visited) {
                    Cell* neighbor = &grid[nr][nc];
                    neighbor->visited = true;
                    neighbor->parent = current;
                    s.push(neighbor);
                    moved = true;
                    break; // Pindah ke tetangga baru
                }
            }
            
            if (!moved) { // Jika tidak ada tetangga valid untuk dikunjungi dari current
                s.pop(); // Backtrack
            }
        }
        return {}; // Tidak ada solusi
    }

    // Menyelesaikan labirin menggunakan BFS
    std::vector<Cell*> solve_bfs(int start_r, int start_c, int end_r, int end_c) {
        reset_cell_states();
         if (!is_safe(start_r, start_c) || !is_safe(end_r, end_c) || 
            grid[start_r][start_c].is_wall || grid[end_r][end_c].is_wall) {
            std::cout << "Start atau End point tidak valid atau berada di tembok." << std::endl;
            return {};
        }
        
        start_node = &grid[start_r][start_c];
        end_node = &grid[end_r][end_c];

        std::queue<Cell*> q;
        q.push(start_node);
        start_node->visited = true;

        // Arah: Atas, Bawah, Kiri, Kanan
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        while (!q.empty()) {
            Cell* current = q.front();
            q.pop();

            if (current->row == end_r && current->col == end_c) {
                return reconstruct_path(current); // Solusi ditemukan
            }

            for (int i = 0; i < 4; ++i) {
                int nr = current->row + dr[i];
                int nc = current->col + dc[i];

                if (is_safe(nr, nc) && !grid[nr][nc].is_wall && !grid[nr][nc].visited) {
                    Cell* neighbor = &grid[nr][nc];
                    neighbor->visited = true;
                    neighbor->parent = current;
                    q.push(neighbor);
                }
            }
        }
        return {}; // Tidak ada solusi
    }
    
    Cell* get_start_node() const { return start_node; }
    Cell* get_end_node() const { return end_node; }

private:
    int rows, cols;
    std::vector<std::vector<Cell>> grid;
    std::mt19937 rng; // Mersenne Twister random number generator
    Cell* start_node = nullptr;
    Cell* end_node = nullptr;
};


int main() {
    int r_dim, c_dim;
    std::cout << "Masukkan dimensi labirin (baris kolom, disarankan ganjil misal 21 31): ";
    std::cin >> r_dim >> c_dim;

    Maze maze(r_dim, c_dim);

    std::cout << "\nMenghasilkan labirin..." << std::endl;
    maze.generate_maze();
    std::cout << "Labirin berhasil dibuat:" << std::endl;
    maze.display_maze();

    int start_r = 1, start_c = 1; // Default start
    int end_r = r_dim - (r_dim % 2 == 0 ? 2 : 2), end_c = c_dim - (c_dim % 2 == 0 ? 2 : 2) ; // Default end
    // Jika dimensi input genap, maze.rows/cols akan +1.
    // Sesuaikan end_r dan end_c agar berada dalam path yang mungkin (biasanya sel ganjil jika start (1,1))
    // dan bukan di border tembok terluar yang sengaja dibuat oleh penyesuaian dimensi ganjil.
    // Contoh: jika input 20x30 -> jadi 21x31. End bisa di (19,29)

    std::cout << "Titik Start (S) default: (" << start_r << "," << start_c << ")" << std::endl;
    std::cout << "Titik End (E) default: (" << end_r << "," << end_c << ")" << std::endl;
    
    // Jika ingin custom start/end:
    // std::cout << "Masukkan koordinat Start (baris kolom): "; std::cin >> start_r >> start_c;
    // std::cout << "Masukkan koordinat End (baris kolom): "; std::cin >> end_r >> end_c;


    int choice;
    std::cout << "\nPilih algoritma solver:" << std::endl;
    std::cout << "1. DFS (Depth-First Search)" << std::endl;
    std::cout << "2. BFS (Breadth-First Search)" << std::endl;
    std::cout << "Pilihan Anda: ";
    std::cin >> choice;

    std::vector<Cell*> solution_path;
    if (choice == 1) {
        std::cout << "\nMenyelesaikan dengan DFS..." << std::endl;
        solution_path = maze.solve_dfs(start_r, start_c, end_r, end_c);
    } else if (choice == 2) {
        std::cout << "\nMenyelesaikan dengan BFS..." << std::endl;
        solution_path = maze.solve_bfs(start_r, start_c, end_r, end_c);
    } else {
        std::cout << "Pilihan tidak valid." << std::endl;
        return 1;
    }

    if (!solution_path.empty()) {
        std::cout << "Solusi ditemukan!" << std::endl;
        maze.display_maze(solution_path);
    } else {
        std::cout << "Tidak ada solusi yang ditemukan." << std::endl;
        // Tampilkan maze tanpa path jika tidak ada solusi, tapi start/end tetap ada
        maze.display_maze(); 
    }

    return 0;
}
