#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <chrono>

using namespace std;

array<char, 81> import_board(char *path);
void print_board(array<char, 81> &board);
bool check_board_validity(array<char, 81> &board);
bool solve(array<char, 81> &board);
unordered_map<int, unordered_set<char>> get_possible_values(array<char, 81> &board);

int main(int argc, char *argv[]) {
    array<char, 81> board = import_board(argv[1]);
    
    // Print out initial board
    cout << "Input Board:\n";
    print_board(board);

    auto t1 = chrono::high_resolution_clock::now();

    if (check_board_validity(board)) {
        solve(board);
        print_board(board);
    } else {
        cout << "Impossible board.\n";
    }

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> ms_double = t2 - t1;
    cout << "Time execution " << ms_double.count() / 1000 << "s\n";
}

array<char, 81> import_board(char *path) {
    array<char, 81> board;
    ifstream sudokuFile(path);
    string line;

    if (!sudokuFile.is_open()) {
        cout << "Unable to open sudoku file." << endl;
        exit(1);
    }

    for (int i = 0; i < 9; i++) {
        getline(sudokuFile, line);
        line.resize(9, ' ');
        for (int j = 0; j < 9; j++) {
            board[9 * i + j] = (line[j] > '0' && line[j] <= '9') ? line[j] : 0;
        }
    }
    return board;
}

void print_board(array<char, 81> &board) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << '|' << (board[9 * i + j] ? board[9 * i + j] : ' ');
        }
        cout << "|\n";
    }
    return;
}

bool check_board_validity(array<char, 81> &board) {
    for (int idx = 0; idx < 81; idx++) {
        if (!board[idx]) continue;

        int row = idx / 9;
        int col = idx % 9;
        for (int i = 0; i < 9; i++) {
            // Check row
            if (i != col && board[idx] == board[row * 9 + i]) return false;
            // Check column
            if (i != row && board[idx] == board[i * 9 + col]) return false;
        }

        // Check 3x3 square
        int squareRow = row / 3 * 3;
        int squareCol = col / 3 * 3;
        for (int i = squareRow; i < squareRow + 3; i++) {
            for (int j = squareCol; j < squareCol + 3; j++) {
                if ((i != row || j != col) && board[idx] == board[i * 9 + j]) return false;
            }
        }
    }
    return true;
}

bool solve(array<char, 81> &board) {
    unordered_map<int, unordered_set<char>> cell_possible_values = get_possible_values(board);

    // No more unfilled cells. Sudoku is solved!
    if (cell_possible_values.empty()) return true;

    // We find the cell with the least number of possible values
    int index = -1;
    int biggest_possible_values = 10;
    for (auto &p : cell_possible_values) {
        if (p.second.size() < biggest_possible_values) {
            biggest_possible_values = p.second.size();
            index = p.first;
        }
    }

    // Solve from that
    for (auto &c : cell_possible_values[index]) {
        board[index] = c;
        if (solve(board)) return true;
    }
    board[index] = 0;
    return false;
}

unordered_map<int, unordered_set<char>> get_possible_values(array<char, 81> &board) {
    // For each unfilled cell, fill it with a set of all possible values
    unordered_map<int, unordered_set<char>> cell_possible_values;
    for (int i = 0; i < 81; i++) {
        if (!board[i]) {
            cell_possible_values.insert({i, unordered_set<char>{'1', '2', '3', '4', '5', '6', '7', '8', '9'}});
        }
    }

    // For each empty cell, reduce the number of possible values
    for (auto &p : cell_possible_values) {
        int row = p.first / 9;
        int col = p.first % 9;

        // Check row and column
        for (int i = 0; i < 9; i++) {
            p.second.erase(board[9 * row + i]);
            p.second.erase(board[9 * i + col]);
        }

        // Check 3x3 square
        int squareRow = row / 3 * 3;
        int squareCol = col / 3 * 3;
        for (int i = squareRow; i < squareRow + 3; i++) {
            for (int j = squareCol; j < squareCol + 3; j++) {
                p.second.erase(board[9 * i + j]);
            }
        }
    }
    return cell_possible_values;
}