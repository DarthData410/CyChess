#include <iostream>

class Piece {
private:
    int idx;
    std::string type;
    std::string side;
    std::string key;
    bool isactive = true;
    bool hasmoved = false;
    std::string jumpedby;

public:
    Piece(std::string new_type, std::string new_side, std::string new_key, int new_idx);
    std::string get_type();
    std::string get_side();
    std::string get_piecekey();
    std::string get_key();
    int get_idx();
    void jump(std::string jumpedby_key);
    bool get_isactive();
    std::string get_jumpedby();
    bool get_hasmoved();
    void set_hasmoved(bool set);
};