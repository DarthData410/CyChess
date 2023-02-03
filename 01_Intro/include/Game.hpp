#include <iostream>
#include <vector>
#include "Board.cpp"

class Game {
private:
    bool debug_mode = false;
    bool is_running = true;
    std::string screen_return;
    Board chessboard;
    std::string user_action;
    std::string last_action;
    std::string piece_to_move;
    std::string from_tile;
    std::string to_tile;
    std::string from_col;
    std::string from_row;
    std::string to_col;
    std::string to_row;
    format form;
    std::string get_board();
    bool clearscreen = false;
    bool iserror = false;
    void clearvars();
    bool wait_nextturn = false;
    bool restart_game = false;

public:
    Game();
    bool run();
    std::string get_screen();
    void set_useraction(std::string new_action);
    bool get_clearscreen();
    bool get_iserror();
};