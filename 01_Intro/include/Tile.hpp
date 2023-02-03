#include <iostream>

class Tile {
private:
    int idx;
    std::string row;
    std::string col;
    int irow = 0;
    int icol = 0;
    void set_col(std::string set_col);
    void set_row(std::string set_row);
    char hightlight = 'n';
    
public:
    Tile(std::string new_col, std::string new_row, int new_idx);
    std::string get_colrow();
    std::string get_row();
    int get_irow();
    std::string get_col();
    int get_icol();
    int get_idx();
    void set_highlight(char set);
    char get_highlight();
};