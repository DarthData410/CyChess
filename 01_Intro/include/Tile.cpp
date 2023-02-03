#include <iostream>
#include "Tile.hpp"

Tile::Tile(std::string new_col, std::string new_row, int new_idx) {
    set_col(new_col);
    set_row(new_row);
    idx = new_idx;
}

void Tile::set_highlight(char set) {
    hightlight = set;
}

char Tile::get_highlight() {
    return hightlight;
}

void Tile::set_col(std::string set_col) {
        
        if (set_col == "1") {
            icol = 1;
            col = "A";
        }
        else if (set_col == "2") {
            icol = 2;
            col = "B";
        }
        else if (set_col == "3") {
            icol = 3;
            col = "C";
        }
        else if (set_col == "4") {
            icol = 4;
            col = "D";
        }
        else if (set_col == "5") {
            icol = 5;
            col = "E";
        }
        else if (set_col == "6") {
            icol = 6;
            col = "F";
        }
        else if (set_col == "7") {
            icol = 7;
            col = "G";
        }
        else if (set_col == "8") {
            icol = 8;
            col = "H";
        }
}

std::string Tile::get_col() {
    return col;
}

int Tile::get_icol() {
    return icol;
}

void Tile::set_row(std::string set_row) {
        
        row = set_row;

        if (set_row == "1") {
            irow = 1;
        }
        else if (set_row == "2") {
            irow = 2;
        }
        else if (set_row == "3") {
            irow = 3;
        }
        else if (set_row == "4") {
            irow = 4;
        }
        else if (set_row == "5") {
            irow = 5;
        }
        else if (set_row == "6") {
            irow = 6;
        }
        else if (set_row == "7") {
            irow = 7;
        }
        else if (set_row == "8") {
            irow = 8;
        }
}

std::string Tile::get_row() {
    return row;
}

int Tile::get_irow() {
    return irow;
}

std::string Tile::get_colrow() {
    return "[" + col + "::" + row + "]";
}

int Tile::get_idx() {
    return idx;
}