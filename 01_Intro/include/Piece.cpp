#include <iostream>
#include "Piece.hpp"

Piece::Piece(std::string new_type, std::string new_side, std::string new_key, int new_idx) {
    type = new_type;
    side = new_side;
    key = new_key;
    idx = new_idx;
}

std::string Piece::get_type() {
    return type;
}

std::string Piece::get_side() {
    return side;
}

std::string Piece::get_key() {
    return key;
}

std::string Piece::get_piecekey() {
    return "[" + side + "::" + type + "::" + key + "]";
}

int Piece::get_idx() {
    return idx;
}

void Piece::jump(std::string jumpedby_key) {
    jumpedby = jumpedby_key;
    isactive = false;
}

bool Piece::get_isactive() {
    return isactive;
}

std::string Piece::get_jumpedby() {
    return jumpedby;
}

bool Piece::get_hasmoved() {
    return hasmoved;
}

void Piece::set_hasmoved(bool set) {
    hasmoved = set;
}