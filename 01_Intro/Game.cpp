#include <iostream>
#include "include/Game.hpp"

Game::Game() {
    screen_return = "";
    chessboard.set_board_name(form.blue()+"CyChess v 1.0 @PyFryDay"+form.none());
}

bool Game::run() {
    // Main game loop:

    screen_return = "\n";
    if (clearscreen) {
        clearscreen = false;
    }
    if (iserror) {
        iserror = false;
    }
    if (restart_game) {
        restart_game = false;
    }

    if (user_action == "Q" || user_action == "q" || user_action == "QUIT" || user_action == "quit") {
        is_running = false;
        screen_return += "\n";
        screen_return += chessboard.get_boardname();
        screen_return += "... game quit detected. Game Shut Down.\n";\
    }
    else if (user_action == "R" || user_action == "REDRAW" || user_action == "r" || user_action == "redraw") {
        restart_game = true;
        screen_return += get_board();
        screen_return += "What piece to move? (ie: A7/a7): ";\
        clearscreen = true;
        clearvars();
    }
    else if (wait_nextturn) {
        wait_nextturn = false;
        if (user_action == "YES" || user_action == "Y" || user_action == "y" || user_action == "yes") {
            screen_return += get_board();
            clearscreen = true;
        }
        else {
            wait_nextturn = true;
            screen_return = "\n";
            screen_return += "  ...well, enter [Y/y]es when ready. ;-): ";\
        }
    }
    else {
        if (user_action != "" && from_tile == "") {
            // Need logic to check for valid move format, testing for now.:
            from_tile = user_action;
            from_tile = chessboard.get_safetile(from_tile);

            if (chessboard.valid_tile(from_tile,'F')) {
                if (chessboard.get_active_frompiece().get_piecekey()!="[::::null]") {
                    
                    screen_return = get_board();
                    screen_return += "\nWill attempt to move {"+chessboard.get_active_frompiece().get_side()+"-"+chessboard.get_active_frompiece().get_type()+"} @: "+chessboard.get_active_fromtile().get_colrow()+" \n";
                    screen_return += "Next, move to what tile? (ie: a6/A6): ";\
                    clearscreen = true;
                }
                else {
                    screen_return = get_board();
                    screen_return += form.red()+"ERROR: No active piece @ Tile: "+from_tile+form.none()+"\n";
                    iserror = true;
                    from_tile = "";
                    clearscreen = true;
                }
            }
            else{
                screen_return = form.red()+"ERROR: Invalid Choice @: "+from_tile+form.none()+"\n";
                iserror = true;
                from_tile = "";
            }
        }
        else if (user_action != "" && from_tile != "" && to_tile == "") {
            to_tile = user_action;
            to_tile = chessboard.get_safetile(to_tile);

            if (chessboard.valid_tile(to_tile,'T')) {
                screen_return = get_board();
                screen_return += "Great! ready to move {"+chessboard.get_active_frompiece().get_side()+"-"+chessboard.get_active_frompiece().get_type()+"} from: " + from_tile + ", to: " + to_tile + "\n";
                screen_return += "Proceed?: [Y/y]es/no*: ";\
                clearscreen = true;
            }
            else{
                screen_return = form.red()+"ERROR: Invalid Choice @: "+to_tile+form.none()+"\n";
                iserror = true;
                to_tile = "";
            }
        }
        else if (user_action != "" && from_tile != "" && to_tile != "") {
            if (user_action == "YES" || user_action == "Y" || user_action == "y") {
               bool moved = chessboard.move_piece(chessboard.get_active_frompiece().get_idx(),chessboard.get_active_fromtile().get_idx(),chessboard.get_active_totile().get_idx());
               if (moved) {
                    screen_return += form.green()+"Piece Moved!"+form.none()+"\n";
                    screen_return += form.gray()+"--------------------------"+form.none()+"\n";
                    screen_return += form.green()+"Last move was: {"+chessboard.get_active_frompiece().get_side()+"-"+chessboard.get_active_frompiece().get_type()+"} @: "+chessboard.get_active_fromtile().get_colrow()+" to: "+chessboard.get_active_totile().get_colrow()+"."+form.none()+"";
               } 
               else {
                    screen_return = form.red()+"ERROR: "+chessboard.get_boardmsg()+""+form.none()+"";
                    iserror = true;
               }
            }
            
            clearvars();
            screen_return += get_board();
            screen_return += "What piece to move? (ie: A7/a7): ";\
            clearscreen = true;
        }
        else {
            screen_return += get_board();
            screen_return += "What piece to move? (ie: A7/a7): ";\
            clearvars();
        }
    }
    
    user_action = "";
    return is_running;
}

void Game::clearvars() {
    from_tile = "";
    to_tile = "";
    chessboard.cleartile_highlights();
}

std::string Game::get_board() {
    std::string ret = "\n";
    
    ret += chessboard.get_board(restart_game);
    ret += "\n";
    return ret;
}

std::string Game::get_screen() {
    return screen_return;
}
bool Game::get_clearscreen() {
    return clearscreen;
}
bool Game::get_iserror() {
    return iserror;
}

void Game::set_useraction(std::string new_action) {
    last_action = user_action;
    user_action = new_action;
}