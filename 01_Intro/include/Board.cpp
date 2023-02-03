#include <iostream>
#include <vector>
#include <iterator>
#include <istream>
#include <inttypes.h>
#include "Board.hpp"

// **************************
// RULES SECTION
// **************************

Rules::Rules() {
    active_pieces.clear();
    jumped_pieces.clear();
    tiles.clear();
    boardkeys.clear();
}

void Rules::set_active_pieces(std::vector<Piece> ap) {
    active_pieces = ap;
}
void Rules::set_jumped_pieces(std::vector<Piece> jp) {
    jumped_pieces = jp;
}
void Rules::set_tiles(std::vector<Tile> t) {
    tiles = t;
}
void Rules::set_boardkeys(std::vector<keymap> bks) {
    boardkeys = bks;
}
void Rules::set_eval_piece(Piece p) {
    eval_piece = p;
}
// Returns True if ON SAME SIDE or ERROR.
bool Rules::same_side(int from_tileidx, int to_tileidx) {
    bool ret = false;
    if (eval_piece.get_idx()!=99)
    {
        from_tile = loadtile(from_tileidx);
        to_tile = loadtile(to_tileidx);
        keymap ttkm = loadkeymap('1',to_tile.get_idx());
        if (ttkm.get_ikeytwo()!=99) {
            to_piece = loadpiece(ttkm.get_ikeytwo());
            if (to_piece.get_idx()!=99) {
                if (to_piece.get_side()==eval_piece.get_side()) {
                    ret = true;
                    msg = "from tile "+from_tile.get_colrow()+" piece & to tile "+to_tile.get_colrow()+" piece are on same side.";
                }
            }
        }
    }
    else {
        // Error eval_piece not loaded.
        ret = true;
        msg = "eval_piece not loaded. Currently @ null value.";
    }
    return ret;
}

keymap Rules::loadkeymap(char t,int idx) {
    keymap ret = keymap(99,99);
    if (t=='1') {
        // Tile, ikeyone:
        for (keymap km : boardkeys) {
            if (km.get_ikeyone()==idx) {
                ret = km;
                break;
            }
        }
    }
    else if (t=='2') {
        // Piece, ikeytwo:
        for (keymap km : boardkeys) {
            if (km.get_ikeytwo()==idx) {
                ret = km;
                break;
            }
        }
    }
    return ret;
}

Tile Rules::loadtile(int idx) {
    Tile ret = Tile("","",99);
    for (Tile t : tiles) {
        if (t.get_idx()==idx) {
            ret = t;
            break;
        }
    }
    return ret;
}

Tile Rules::loadtile(int icol, int irow) {
    Tile ret = Tile("","",99);
    for (Tile t : tiles) {
        if (t.get_icol()==icol && t.get_irow()==irow) {
            ret = t;
            break;
        }
    }
    return ret;
}

Piece Rules::loadpiece(int idx) {
    Piece ret = Piece("","","null",99);
    for (Piece p : active_pieces) {
        if (p.get_idx()==idx) {
            ret = p;
            break;
        }
    }
    return ret;
}

bool Rules::get_isvalid(int from_tileidx, int to_tileidx) {
    bool ret = false;

    // Side check:
    if (!same_side(from_tileidx,to_tileidx)) {
        switch (t2c(eval_piece.get_type())) {
            case 'P' : ret = pawn_isvalid(); break;
            case 'R' : ret = rook_isvalid(); break;
            case 'N' : ret = knight_isvalid(); break;
            case 'B' : ret = bishop_isvalid(); break;
            case 'Q' : ret = queen_isvalid(); break;
            case 'K' : ret = king_isvalid(); break;
        }

        if (!ret) {
            msg = "Invalid " + eval_piece.get_type() + " Movement Attempted @ " + from_tile.get_colrow() + " - " + to_tile.get_colrow();
        }
    }

    return ret;
}

char Rules::t2c(std::string t) {
    char ret = '0';

    if(t=="PAWN") {ret='P';}
    else if(t=="ROOK") {ret='R';}
    else if(t=="KNIGHT") {ret='N';}
    else if(t=="BISHOP") {ret='B';}
    else if(t=="QUEEN") {ret='Q';}
    else if(t=="KING") {ret='K';}

    return ret;
}

bool Rules::tile_isvalid(int ccol,int crow) {
    bool ret = true;

    Tile tc = loadtile(ccol,crow);
    if (tc.get_idx()!=99) {
        keymap k = loadkeymap('1',tc.get_idx());
        if (k.get_ikeytwo()!=99) {
            ret = false;
        }        
    }

    return ret;
}

bool Rules::cols_isvalid() {
    bool ret = true;
    int diff = 0;

    if (from_tile.get_irow()>to_tile.get_irow()) {
        // -
        diff = from_tile.get_irow()-to_tile.get_irow();
        for (int i=1; i<diff; i++) {
            if (!tile_isvalid(to_tile.get_icol(),(from_tile.get_irow()-i))) {
                ret = false;
                break;
            }
        }
    }
    else {
        // +
        diff = to_tile.get_irow()-from_tile.get_irow();
        for (int i=1; i<diff; i++) {
            if (!tile_isvalid(to_tile.get_icol(),(from_tile.get_irow()+i))) {
                ret = false;
                break;
            }
        }
    }

    return ret;
}

bool Rules::rows_isvalid() {
    bool ret = true;
    int diff = 0;

    if (from_tile.get_icol()>to_tile.get_icol()) {
        // - 
        diff = from_tile.get_icol()-to_tile.get_icol();
        for (int i=1; i<diff; i++) {
            if (!tile_isvalid((from_tile.get_icol()-i),to_tile.get_irow())) {
                ret = false;
                break;
            }
        }
    }
    else {
        // +
        diff = to_tile.get_icol()-from_tile.get_icol();
        for (int i=1; i<diff; i++) {
            if (!tile_isvalid((from_tile.get_icol()+i),to_tile.get_irow())) {
                ret = false;
                break;
            }
        }
    }

    return ret;
}

bool Rules::diag_isvalid() {
    bool ret = true;
    bool ranit = false;
    bool touch_totile = false;
    int bounds = 8;

    if (from_tile.get_irow()>to_tile.get_irow() && from_tile.get_icol()>to_tile.get_icol()) {
        // Move toward: top-down, right-left: -/-
            ranit = true;
            for (int c=1;c<=bounds;c++) {
                if ((from_tile.get_icol()-c)==to_tile.get_icol() && (from_tile.get_irow()-c)==to_tile.get_irow()) {
                    touch_totile = true;
                    break;
                }
                ret = tile_isvalid(from_tile.get_icol()-c,from_tile.get_irow()-c);
                if (!ret) { break; }
            }
       }
       else if (from_tile.get_irow()>to_tile.get_irow() && from_tile.get_icol()<to_tile.get_icol()) {
        // Move toward: top-down, left-right: +/-
            ranit = true;
            for (int c=1;c<=bounds;c++) {
                if ((from_tile.get_icol()+c)==to_tile.get_icol() && (from_tile.get_irow()-c)==to_tile.get_irow()) {
                    touch_totile = true;
                    break;
                }
                ret = tile_isvalid(from_tile.get_icol()+c,from_tile.get_irow()-c);
                if (!ret) { break; }
            }
       }
       else if (from_tile.get_irow()<to_tile.get_irow() && from_tile.get_icol()<to_tile.get_icol()) {
        // Move toward: down-top, left-right: +/+
           ranit = true;
           for (int c=1;c<=bounds;c++) {
                if ((from_tile.get_icol()+c)==to_tile.get_icol() && (from_tile.get_irow()+c)==to_tile.get_irow()) {
                    touch_totile = true;
                    break;
                }
                ret = tile_isvalid(from_tile.get_icol()+c,from_tile.get_irow()+c);
                if (!ret) { break; }
            } 
       }
       else if (from_tile.get_irow()<to_tile.get_irow() && from_tile.get_icol()>to_tile.get_icol()) {
        // Move toward: down-top, right-left: -/+
            ranit = true;
            for (int c=1;c<=bounds;c++) {
                if ((from_tile.get_icol()-c)==to_tile.get_icol() && (from_tile.get_irow()+c)==to_tile.get_irow()) {
                    touch_totile = true;
                    break;
                }
                ret = tile_isvalid(from_tile.get_icol()-c,from_tile.get_irow()+c);
                if (!ret) { break; }
            }
       }

    if (ret && ranit && !touch_totile) {
        ret = false;
    }

    return ret;
}

bool Rules::pawn_isvalid() {
    bool ret = false;
    
    // test jump (daig):
    if (((from_tile.get_icol()+1)==to_tile.get_icol()) || ((from_tile.get_icol()-1)==to_tile.get_icol())) {
        if (to_piece.get_idx()!=99) {
            ret = true;
        }
    }
    
    if (!ret) {
        if (!eval_piece.get_hasmoved()) {
            // can move twice:
            if (eval_piece.get_side()=="BLACK") {
                // -
                if ((from_tile.get_irow()-1)==to_tile.get_irow() && from_tile.get_icol()==to_tile.get_icol()) {
                    if (to_piece.get_idx()==99) {
                        ret = true;
                    }
                }
                else if ((from_tile.get_irow()-2)==to_tile.get_irow() && from_tile.get_icol()==to_tile.get_icol()) { 
                    if (to_piece.get_idx()==99) {
                        ret = true;
                    }

                    if (ret) {
                        int gtidx = 0;
                        for (Tile t : tiles) {
                            if (t.get_icol()==to_tile.get_icol() && (t.get_irow()-1)==to_tile.get_irow()) {
                                gtidx = t.get_idx();
                                break;
                            }    
                        }

                        if (gtidx!=0) {
                            keymap kmlu = loadkeymap('1',gtidx);
                            if (kmlu.get_ikeytwo()!=99) {
                                ret = false;
                            }
                        }
                    }
                }
            }
            else {
                // +
                if ((from_tile.get_irow()+1)==to_tile.get_irow() && from_tile.get_icol()==to_tile.get_icol()) {
                    if (to_piece.get_idx()==99) {
                        ret = true;
                    }
                }
                else if ((from_tile.get_irow()+2)==to_tile.get_irow() && from_tile.get_icol()==to_tile.get_icol()) { 
                    if (to_piece.get_idx()==99) {
                        ret = true;
                    }

                    if (ret) {
                        int gtidx = 0;
                        for (Tile t : tiles) {
                            if (t.get_icol()==to_tile.get_icol() && (t.get_irow()+1)==to_tile.get_irow()) {
                                gtidx = t.get_idx();
                                break;
                            }    
                        }

                        if (gtidx!=0) {
                            keymap kmlu = loadkeymap('1',gtidx);
                            if (kmlu.get_ikeytwo()!=99) {
                                ret = false;
                            }
                        }
                    }
                }

            }
        }
        else {
            // can move once:
            if (eval_piece.get_side()=="BLACK") {
                // -
                if ((from_tile.get_irow()-1)==to_tile.get_irow() && from_tile.get_icol()==to_tile.get_icol()) {
                    if (to_piece.get_idx()==99) {
                        ret = true;
                    }
                }
            }
            else {
                // +
                if ((from_tile.get_irow()+1)==to_tile.get_irow() && from_tile.get_icol()==to_tile.get_icol()) {
                    if (to_piece.get_idx()==99) {
                        ret = true;
                    }
                }
            }
        }
    }

    return ret;
}

bool Rules::rook_isvalid() {
    bool ret = true;
    
    // Column Check:
    if (from_tile.get_icol()==to_tile.get_icol() && from_tile.get_irow()!=to_tile.get_irow()) {
        ret = cols_isvalid();
    }
    // Row Check:
    else if (from_tile.get_irow()==to_tile.get_irow() && from_tile.get_icol()!=to_tile.get_icol()) {
        ret = rows_isvalid();
    }
    
    // same column or row, final check:
    if (ret) {
        if (from_tile.get_icol()!=to_tile.get_icol() && from_tile.get_irow()!=to_tile.get_irow()) {
            ret = false;
        }
    }
    
    return ret;
}

bool Rules::knight_isvalid() {
    bool ret = false;

    if ((from_tile.get_icol()+1)==to_tile.get_icol() || (from_tile.get_icol()-1)==to_tile.get_icol()) {
        if ((from_tile.get_irow()+2)==to_tile.get_irow() || (from_tile.get_irow()-2)==to_tile.get_irow()) {
            ret = true;
        }
    }
    else if ((from_tile.get_icol()+2)==to_tile.get_icol() || (from_tile.get_icol()-2)==to_tile.get_icol()) {
        if ((from_tile.get_irow()+1)==to_tile.get_irow() || (from_tile.get_irow()-1)==to_tile.get_irow()) {
            ret = true;
        }
    }

    return ret;
}

bool Rules::bishop_isvalid() {
    bool ret = true;
    int bounds = 8;

    // Force diag movements.:
    if (from_tile.get_icol()==to_tile.get_icol() || from_tile.get_irow()==to_tile.get_irow()) {
        ret = false;
    }

    // Valid diag checks:
    if (ret) {
       ret = diag_isvalid();
    }

    return ret;
}

bool Rules::queen_isvalid() {
    bool ret = true;

    ret = diag_isvalid();
    if(ret) {
        ret = cols_isvalid();
    }
    if(ret) {
        ret = rows_isvalid();
    }

    return ret;
}

bool Rules::king_isvalid() {
    bool ret = false;

    if (from_tile.get_icol()==to_tile.get_icol() && from_tile.get_irow()!=to_tile.get_irow()) {
        // same column.:
        if ((to_tile.get_idx()==loadtile(to_tile.get_icol(),from_tile.get_irow()+1).get_idx()) || (to_tile.get_idx()==loadtile(to_tile.get_icol(),from_tile.get_irow()-1).get_idx())) {
            ret = true;
        } 
    }
    else if (from_tile.get_icol()!=to_tile.get_icol() && from_tile.get_irow()==to_tile.get_irow()) {
        // same row:
        if ((to_tile.get_idx()==loadtile(from_tile.get_icol()+1,to_tile.get_irow()).get_idx()) || (to_tile.get_idx()==loadtile(from_tile.get_icol()-1,to_tile.get_irow()).get_idx())) {
            ret = true;
        }
    }

    // diags check:
    if(!ret) {
        if (from_tile.get_icol()!=to_tile.get_icol() && from_tile.get_irow()!=to_tile.get_icol()) {
            
            if ((from_tile.get_icol()+1)==to_tile.get_icol() && (from_tile.get_irow()+1)==to_tile.get_irow()) {
                ret = true;
            }
            else if ((from_tile.get_icol()+1)==to_tile.get_icol() && (from_tile.get_irow()-1)==to_tile.get_irow()) {
                ret = true;
            }
            else if ((from_tile.get_icol()-1)==to_tile.get_icol() && (from_tile.get_irow()-1)==to_tile.get_irow()) {
                ret = true;
            }
            else if ((from_tile.get_icol()-1)==to_tile.get_icol() && (from_tile.get_irow()-1)==to_tile.get_irow()) {
                ret = true;
            }
        }
    }

    return ret;
}

std::string Rules::get_msg() {
    return msg;
}
// ***************************************************
// END RULES
// ***************************************************



// ***************************
// BOARD SECTION:
// ***************************

Board::Board() {
    init_board();
    init_pieces();
}

std::string Board::make_tilecolrow(std::string col, std::string row) {
    return "[" + col + "::" + row + "]";
}

void Board::set_board_name(std::string new_boardname) {
    board_name = new_boardname;
}

void Board::add_tile(Tile new_tile) {
    tiles.push_back(new_tile);
}

void Board::add_piece(Piece new_piece) {
    pieces.push_back(new_piece);
}

void Board::add_movementlog(MovementLog new_log) {
    mlogs.push_back(new_log);
    lastlog = new_log;
}

MovementLog Board::get_lastlog() {
    return lastlog;
}

std::string Board::get_safetile(std::string tile) {
    
    int i = 0;
    bool passon = false;
    std::string bft = "";
    for(char c : tile) {
        // Operate on values sent in.:
        if(i==0) {
            if(c=='[') {
                passon = true;
                bft = tile;
                break;
            }
            else {
                bft = "[";
                switch(c) {
                    case 'a' : bft+= 'A'; break;
                    case 'b' : bft+= 'B'; break;
                    case 'c' : bft+= 'C'; break;
                    case 'd' : bft+= 'D'; break;
                    case 'e' : bft+= 'E'; break;
                    case 'f' : bft+= 'F'; break;
                    case 'g' : bft+= 'G'; break;
                    case 'h' : bft+= 'H'; break;
                    default: bft+= c; break;
                }
                bft += "::";
            }
        }
        else if (i==1) {
            bft += c;
            bft += "]";
            break;
        }
        i++;
    }

    return bft;
}

std::string Board::get_colrowstr(int rowcol) {
    
    std::string ret;
    switch (rowcol) {
        case 1 :
            ret = "1";
            break;
        
        case 2 :
            ret = "2";
            break;

        case 3 :
            ret = "3";
            break;
        
        case 4:
            ret = "4";
            break;
        
        case 5:
            ret = "5";
            break;
        
        case 6:
            ret = "6";
            break;
        
        case 7:
            ret = "7";
            break;
        
        case 8:
            ret = "8";
            break;
        
        default:
            ret = "0";
            break;
    }

    return ret;
}

void Board::init_board() {

    // Initalize Board, with adding tiles that represent each tile by col & row:
    int idx = 0;
    for (int i = 1; i <= 8; i++) {
        for (int x = 1; x <= 8; x++) {
            
            Tile t(get_colrowstr(i),get_colrowstr(x),idx); 
            add_tile(t);
            idx++;
        }
    }
}

std::string Board::get_royal(int col) {
    std::string ret;
    
    switch(col) {
        case 1 : // A 
            ret = "ROOK";
            break;
        case 2 : // B 
            ret = "KNIGHT";
            break;
        case 3 : // C 
            ret = "BISHOP";
            break;
        case 4 : // D
            ret = "QUEEN";
            break;
        case 5 : // E
            ret = "KING";
            break;
        case 8 : // A 
            ret = "ROOK";
            break;
        case 7 : // B 
            ret = "KNIGHT";
            break;
        case 6 : // C 
            ret = "BISHOP";
            break;
        default:
            ret = "NONE";
            break;
    }

    return ret;
}

void Board::init_pieces() {

    std::string pt;
    std::string ps;
    std::string cr;
    int ip = 0;
    // Initialize Board Pieces, based on tiles:
    for (Tile t : tiles) {
        pt = "";
        ps = "";
        cr = "";
        cr = t.get_colrow();
        switch(t.get_irow()) {
            case 1 :
                ps = "WHITE";
                pt = get_royal(t.get_icol());
                break;
            case 2 : 
                ps = "WHITE";
                pt = "PAWN";
                break;
            case 7 :
                ps = "BLACK";
                pt = "PAWN";
                break;
            case 8 :
                ps = "BLACK";
                pt = get_royal(t.get_icol());
                break;
            default:
                ps = "";
                pt = "";
                break;
        }
        
        if (ps != "" && pt != "") {
            Piece p(pt,ps,cr,ip);
            add_piece(p);
            init_key(t.get_idx(),p.get_idx());
            ip++;
        }
    }
}

void Board::add_key(keymap new_keymap) {
    boardkeys.push_back(new_keymap);
}

void Board::init_key(Tile key_tile, Piece key_piece) {
    keymap km(key_tile.get_colrow(),key_piece.get_piecekey());
    add_key(km);
}

void Board::init_key(int key_tile_index, int key_piece_index) {
    keymap km(key_tile_index,key_piece_index);
    add_key(km);
}

keymap Board::get_key(char lookup, int idx) {
    keymap ret("null","null");
    
    switch (lookup) {
        case 'T' : 
            for (keymap km : boardkeys) {
                if (km.get_ikeyone()==idx) {
                    ret = km;
                    break;
                }
            }
            break;
        case 'P' :
            for (keymap km : boardkeys) {
                if (km.get_ikeytwo()==idx) {
                    ret = km;
                    break;
                }
            }
        default:
            ret = ret;
    }

    return ret;
}

Piece Board::get_piece(std::string get_piecekey) {

    Piece ret("","","",99);
    
    for (Piece p : pieces) {
        if (p.get_piecekey() == get_piecekey) {
            ret = p;
            break;
        }
    }

    return ret;
}

Piece Board::get_piece(int get_pieceidx) {

    Piece ret = pieces.at(get_pieceidx);
    return ret;
}

Piece Board::get_active_frompiece() {
    return afp;
}
Tile Board::get_active_fromtile() {
    return aft;
}
Piece Board::get_active_topiece() {
    return atp;
}
Tile Board::get_active_totile() {
    return att;
}

bool Board::valid_tile(std::string tile_check,char ft) {
    bool ret = false;

    Piece pc = Piece("","","null",99);
    keymap km = keymap("null","null");
    Tile tc = get_tile(tile_check);
    bool hasp = false;
    char th = 'n';

    if (tc.get_idx() != 99) {

        // Load keymap and Piece: 
        km = get_key('T',tc.get_idx());
        if (km.get_keyone()!="null") {
            pc = get_piece(km.get_ikeytwo());
            hasp = true;
        }

        if (ft == 'F') {
            aft = tc;
            aftkm = km;
            afp = pc;
            if (hasp) {
                th = 'g';
            }
            else {
                th = 'r';
            }
        }
        else if (ft == 'T') {
            att = tc;
            atfkm = km;
            atp = pc;
            th = 'g';
        }
        tc.set_highlight(th);
        ret = update_tile(tc);
    }

    return ret;
}

void Board::cleartile_highlights() {
    char th = 'n';

    for (Tile t : tiles) {
        t.set_highlight(th);
        update_tile(t);
    }
}

bool Board::update_tile(Tile up_tile) {
    bool ret = false;
    int i = 0;
    
    for(Tile t : tiles) {
        if (t.get_idx()==up_tile.get_idx()) {
            ret = true;
            break;
        }
        i++;
    }

    Tile &tu = tiles[i];
    tu.set_highlight(up_tile.get_highlight());
    return ret;
}

Tile Board::get_tile(std::string get_tilecolrow) {

    Tile ret("","",99);
    for (Tile t : tiles) {
        if (t.get_colrow() == get_tilecolrow) {
            ret = t;
            break;
        }
    }
    return ret;
}

Tile Board::get_tile(int get_tileidx) {

    Tile ret = tiles.at(get_tileidx);
    return ret;
}

void Board::update_key(int tile_fromidx, int tile_toidx, int up_pieceidx, int key_index) {

    keymap &ktu = boardkeys[key_index];
    if (ktu.get_ikeyone()==tile_fromidx) {
        ktu.update_ikeys(tile_toidx,up_pieceidx);
    }
}

// Returns true if has a peice on it.
bool Board::check_tile(int tileidx) {
    bool ret = false;

    for (int i = 0; i < boardkeys.size(); i++) {
        keymap km = boardkeys[i];

        if (km.get_ikeyone()==tileidx) {
            ret = true;
        }
    }

    return ret;
}

bool Board::jump_piece(Piece to_jump, Piece jumped_by) {
    bool ret = false;

    int i = 0;
    for (Piece pc : pieces) {
        if (pc.get_idx()==to_jump.get_idx()) {
            ret = true;
            break;
        }
        i++;
    }

    Piece &jumped_piece = pieces[i];
    jumped_piece.jump(jumped_by.get_piecekey());
    Piece njp = Piece(jumped_piece.get_type(),jumped_piece.get_side(),jumped_piece.get_key(),jumped_piece.get_idx());
    njp.jump(jumped_piece.get_jumpedby());
    jumped_pieces.push_back(njp);

    return ret;
}

std::string Board::get_boardmsg() {
    return board_msg;
}

bool Board::move_piece(int pieceidx,int fromtileidx, int totileidx) {

    bool ret = false;
    bool tile_haspiece = false;
    bool pass_rules = false;
    std::string rules_msg = "";
    Rules rules = Rules();
    rules.set_boardkeys(boardkeys);
    rules.set_active_pieces(pieces);
    rules.set_jumped_pieces(jumped_pieces);
    rules.set_tiles(tiles);
    rules.set_eval_piece(get_piece(pieceidx));
    Piece piece_tojump = Piece("","","null",99);
    Piece piece_jumping = Piece("","","null",99);

    for (int i = 0; i < boardkeys.size(); i++) {
        keymap km = boardkeys[i];

        if (km.get_ikeytwo()==pieceidx) {
            if (rules.get_isvalid(fromtileidx,totileidx)) {
                tile_haspiece = check_tile(totileidx);
                if (tile_haspiece) {
                    piece_tojump = get_piece(get_key('T',totileidx).get_ikeytwo());
                    piece_jumping = get_piece(pieceidx);
                    jump_piece(piece_tojump,piece_jumping);
                }
                MovementLog ml(pieceidx,fromtileidx,totileidx);
                add_movementlog(ml);
                update_key(fromtileidx,totileidx,pieceidx,i);
                ret = true;
                break;
            }
            else {
                board_msg = rules.get_msg();
                break;
            }
        }
    }

    if (tile_haspiece) {
        for (int x = 0; x < boardkeys.size(); x++) {
            keymap kmu = boardkeys[x];

            if (kmu.get_ikeytwo()==piece_tojump.get_idx()) {
                MovementLog ml(piece_tojump.get_idx(),totileidx,99);
                add_movementlog(ml);
                update_key(totileidx,99,piece_tojump.get_idx(),x);
                break;
            }
        }
    }

    if (ret) {
        Piece &up_piece = pieces[pieceidx];
        up_piece.set_hasmoved(true);
    }

    return ret;
}

bool Board::move_piece(std::string piecekey, std::string fromtile_col, std::string fromtile_row, std::string totile_col, std::string totile_row) {

    bool ret = false;
    Piece ptm = get_piece(piecekey);
    std::string ftcr = make_tilecolrow(fromtile_col,fromtile_row);
    Tile ft = get_tile(ftcr);
    std::string ttcr = make_tilecolrow(totile_col,totile_row);
    Tile tt = get_tile(ttcr);
    ret = move_piece(ptm.get_idx(),ft.get_idx(),tt.get_idx());
    return ret;
}

std::string Board::get_boardname() {
    return board_name;
}

std::string Board::get_charpiecebase(std::string type,std::string side) {
    std::string ret = " ";

    if (side=="WHITE") {
        if(type=="PAWN") { ret = "♟"; }
        else if(type=="ROOK") { ret = "♜"; }
        else if(type=="KNIGHT") { ret = "♞"; }
        else if(type=="BISHOP") { ret = "♝"; }
        else if(type=="QUEEN") { ret = "♛"; }
        else if(type=="KING") { ret = "♚"; }
    }
    else if (side=="BLACK") {
        if(type=="PAWN") { ret = "♙"; }
        else if(type=="ROOK") { ret = "♖"; }
        else if(type=="KNIGHT") { ret = "♘"; }
        else if(type=="BISHOP") { ret = "♗"; }
        else if(type=="QUEEN") { ret = "♕"; }
        else if(type=="KING") { ret = "♔"; }
    }

    return ret;
}

std::string Board::get_charpiece(int pieceidx) {
    std::string ret = " ";
    Piece pc = pieces.at(pieceidx);

    ret = get_charpiecebase(pc.get_type(),pc.get_side());

    return ret;
}

std::string Board::get_ajpiecesbase(std::string side, std::string aj, std::vector<keymap> kmc) {
    std::string ret = "";
    ret += ""+side+" "+aj+" Pieces: \n";
    Tile t = Tile("","",99);
    Piece p = Piece("","","null",99);
    std::vector<keymap> ptots;
    bool found_key = false;

    for (keymap km : kmc) {
        if (km.get_ikeyone()!=99) {
            t = tiles.at(km.get_ikeyone());
        }
        if (km.get_ikeytwo()!=99) {
            p = pieces.at(km.get_ikeytwo());
        }
        if (p.get_side()==side && p.get_idx() != 99) {
            
            if (aj=="Active" && p.get_isactive()==false) {
                continue;
            }
            else if (aj=="Jumped" && p.get_isactive()==true) {
                continue;
            }

            int iu = 0;
            int iuc = 0;
            for (keymap kpc : ptots) {
                if (kpc.get_keyone()==p.get_type()) {
                    iuc = kpc.get_ikeytwo();
                    iuc++;
                    keymap &kpcu = ptots[iu];
                    kpcu.update_keys(kpc.get_keyone(),iuc);
                    found_key = true;
                    break;
                }
                iu++;
            }

            if (found_key == false) {
                keymap nkpc(p.get_type(),1);
                ptots.push_back(nkpc);
            }
            else {
                found_key = false;
            }
        }
    }

    ret += get_pieces_base("PAWN",side,ptots);
    ret += get_pieces_base("ROOK",side,ptots);
    ret += get_pieces_base("KNIGHT",side,ptots);
    ret += get_pieces_base("BISHOP",side,ptots);
    ret += get_pieces_base("QUEEN",side,ptots);
    ret += get_pieces_base("KING",side,ptots);

    int rs = ret.size() - 1;
    ret.resize(rs);

    ret += "\n";

    return ret;
}

std::string Board::get_activepieces(std::string side) {
    std::string ret = "";
    ret = get_ajpiecesbase(side,"Active",boardkeys);
    return ret;
}

std::string Board::get_jumpedpieces(std::string side) {
    std::string ret = "";
    std::vector<keymap> jumpedkeys;

    for (Piece jp : jumped_pieces) {
        keymap jpkm(0,jp.get_idx());
        jumpedkeys.push_back(jpkm);
    }

    ret = get_ajpiecesbase(side,"Jumped",jumpedkeys);
    return ret;
}

std::string Board::get_pieces_base(std::string type, std::string side, std::vector<keymap> tkm) {
    std::string ret = "";

    for (keymap pk : tkm) {
        if (pk.get_keyone()==type) {
            ret += get_charpiecebase(pk.get_keyone(),side) + "x" + std::to_string(pk.get_ikeytwo()) + ",";
        }
    }

    return ret;
}

std::string Board::draw_tile(Tile draw_tile) {
    std::string ret = "";
    std::string blank = "|     ";
    ret = blank;
    std::string bl = "|";
    std::string fp = "  ";
    std::string ep = "  ";
    std::string pieceval = " ";
    std::string fm = "";

    keymap km = get_key('T',draw_tile.get_idx());
    if (km.get_keyone()!="null") {
        Piece p = get_piece(km.get_ikeytwo());
        pieceval = get_charpiece(p.get_idx());
    }

    if (draw_tile.get_highlight()=='n') {
        fm = form.none();
    }
    else if (draw_tile.get_highlight()=='g') {
        fm = form.green();
    }
    else if (draw_tile.get_highlight()=='r') {
        fm = form.red();
    }

    ret = bl+ fm + fp + pieceval + ep + form.none();
    return ret;
}

std::string Board::active_board() {
    std::string ret = "\n";

    ret += get_activepieces("WHITE");
    ret += " - - - - - - \n";  
    ret += get_activepieces("BLACK");
    
    ret += draw_active_board();

    if (jumped_pieces.size()>0) {
        ret += get_jumpedpieces("WHITE");
        ret += " - - - - - - \n";  
        ret += get_jumpedpieces("BLACK");
    }
    

    return ret;
}

std::string Board::draw_active_board() {
    std::string ret = "\n";
    std::string row_break = form.gb("  -")+"|-----|-----|-----|-----|-----|-----|-----|-----|"+form.gb("-  ")+"";
    std::string row8 = "";
    std::string row7 = "";
    std::string row6 = "";
    std::string row5 = "";
    std::string row4 = "";
    std::string row3 = "";
    std::string row2 = "";
    std::string row1 = "";

    ret += form.gb("   |  A  |  B  |  C  |  D  |  E  |  F  |  G  |  H  |")+""+form.gb("   ")+"\n";
    ret += form.gb("  -")+"= = = = = = = = = = = = = = = = = = = = = = = = ="+form.gb("-  ")+"\n";

    for (Tile t : tiles) {
        if (t.get_row()=="8") {
            row8 += draw_tile(t);
        }
        else if (t.get_row()=="7") {
            row7 += draw_tile(t);
        }
        else if (t.get_row()=="6") {
            row6 += draw_tile(t);
        }
        else if (t.get_row()=="5") {
            row5 += draw_tile(t);
        }
        else if (t.get_row()=="4") {
            row4 += draw_tile(t);
        }
        else if (t.get_row()=="3") {
            row3 += draw_tile(t);
        }
        else if (t.get_row()=="2") {
            row2 += draw_tile(t);
        }
        else if (t.get_row()=="1") {
            row1 += draw_tile(t);
        }
    }

    ret += form.gb(" 8 ")+row8+"|"+form.gb(" 8 ")+"\n";
    ret += row_break+" Game Help: \n";
    ret += form.gb(" 7 ")+row7+"|"+form.gb(" 7 ")+" - - - - \n";
    ret += row_break+" [Q/q & Enter to 'Quit']\n";
    ret += form.gb(" 6 ")+row6+"|"+form.gb(" 6 ")+" [R/r & Enter to 'Restart']\n";
    ret += row_break+" - - - - \n";
    ret += form.gb(" 5 ")+row5+"|"+form.gb(" 5 ")+" Move Pieces by entering \n";
    ret += row_break+" tile column|row. \n";
    ret += form.gb(" 4 ")+row4+"|"+form.gb(" 4 ")+" (ie: d2 to d4) \n";
    ret += row_break+" - - - - \n";
    ret += form.gb(" 3 ")+row3+"|"+form.gb(" 3 ")+" "+form.green()+"   "+form.none()+" = good move(s). \n";
    ret += row_break+"\n";
    ret += form.gb(" 2 ")+row2+"|"+form.gb(" 2 ")+" "+form.red()+"   "+form.none()+" = bad move(s). \n";
    ret += row_break+"\n";
    ret += form.gb(" 1 ")+row1+"|"+form.gb(" 1 ")+"\n";
    
    ret += form.gb("  -")+"= = = = = = = = = = = = = = = = = = = = = = = = ="+form.gb("-  ")+"\n";
    ret += form.gb("   |  A  |  B  |  C  |  D  |  E  |  F  |  G  |  H  |")+""+form.gb("   ")+"\n";

    return ret;
}

std::string Board::initial_board() {
    
    std::string ret = "\n";
    
    ret += form.gb("   |  A  |  B  |  C  |  D  |  E  |  F  |  G  |  H  |")+""+form.gb("   ")+"\n";
    ret += form.gb("  -")+"= = = = = = = = = = = = = = = = = = = = = = = = ="+form.gb("-  ")+"\n";
    ret += form.gb(" 8 ")+"|  ♖  |  ♘  |  ♗  |  ♕  |  ♔  |  ♗  |  ♘  |  ♖  |"+form.gb(" 8 ")+"\n";
    ret += form.gb("  -")+"|-----|-----|-----|-----|-----|-----|-----|-----|"+form.gb("-  ")+"\n";
    ret += form.gb(" 7 ")+"|  ♙  |  ♙  |  ♙  |  ♙  |  ♙  |  ♙  |  ♙  |  ♙  |"+form.gb(" 7 ")+"\n";
    ret += form.gb("  -")+"|-----|-----|-----|-----|-----|-----|-----|-----|"+form.gb("-  ")+"\n";
    ret += form.gb(" 6 ")+"|     |     |     |     |     |     |     |     |"+form.gb(" 6 ")+"\n";
    ret += form.gb("  -")+"|-----|-----|-----|-----|-----|-----|-----|-----|"+form.gb("-  ")+"\n";
    ret += form.gb(" 5 ")+"|     |     |     |     |     |     |     |     |"+form.gb(" 5 ")+"\n";
    ret += form.gb("  -")+"|-----|-----|-----|-----|-----|-----|-----|-----|"+form.gb("-  ")+"\n";
    ret += form.gb(" 4 ")+"|     |     |     |     |     |     |     |     |"+form.gb(" 4 ")+"\n";
    ret += form.gb("  -")+"|-----|-----|-----|-----|-----|-----|-----|-----|"+form.gb("-  ")+"\n";
    ret += form.gb(" 3 ")+"|     |     |     |     |     |     |     |     |"+form.gb(" 3 ")+"\n";
    ret += form.gb("  -")+"|-----|-----|-----|-----|-----|-----|-----|-----|"+form.gb("-  ")+"\n";
    ret += form.gb(" 2 ")+"|  ♟  |  ♟  |  ♟  |  ♟  |  ♟  |  ♟  |  ♟  |  ♟  |"+form.gb(" 2 ")+"\n";
    ret += form.gb("  -")+"|-----|-----|-----|-----|-----|-----|-----|-----|"+form.gb("-  ")+"\n";
    ret += form.gb(" 1 ")+"|  ♜  |  ♞  |  ♝  |  ♛  |  ♚  |  ♝  |  ♞  |  ♜  |"+form.gb(" 1 ")+"\n";
    ret += form.gb("  -")+"= = = = = = = = = = = = = = = = = = = = = = = = ="+form.gb("-  ")+"\n";
    ret += form.gb("   |  A  |  B  |  C  |  D  |  E  |  F  |  G  |  H  |")+""+form.gb("   ")+"\n";
    
    return ret;
}

std::string Board::draw_board() {
    
    std::string ret = "";
    ret += active_board();
    //ret += initial_board();
    return ret;
}

std::string Board::get_board(bool restart) {

    std::string ret = board_name + "\n";
    
    if(restart) {
        tiles.clear();
        pieces.clear();
        jumped_pieces.clear();
        boardkeys.clear();

        init_board();
        init_pieces();
    }

    ret += draw_board();

    return ret;
}

std::string Board::get_debug() {
    std::string ret = "\n";

    ret += "Chess Board Tiles & Pieces: \n";
    ret += "Tiles: \n";

    for (Tile t : tiles) {
        ret += "Tile: " + t.get_colrow() + "\n";
    }
    ret += "\nPieces: \n";

    for (Piece p : pieces) {
        ret += "Piece: {" + p.get_side() + "::" + p.get_type() + "::" + p.get_piecekey() + "}\n"; 
    }

    ret += "\nBoard keymap: \n";

    int i = 0;
    for (keymap km : boardkeys) {
            
        Tile t = get_tile(km.get_ikeyone());
        Piece p = get_piece(km.get_ikeytwo());

        ret += "Key: ("; 
        ret += t.get_colrow();  
        ret += " - "; 
        ret += p.get_piecekey();  
        ret += ")\n";
        ret += "-------------------\n";
        ret += " * Piece Details * \n";
        ret += " - Side: ";
        ret += p.get_side();
        ret += "\n";
        ret += " - Type: ";
        ret += p.get_type();
        ret += "\n";
        ret += " - Key: ";
        ret += p.get_piecekey();
        ret += "\n";
        ret += "-------------------\n";
        i++;
    }

    return ret;
}
