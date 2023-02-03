#include <iostream>
#include <vector>
#include "Tile.cpp"
#include "Piece.cpp"
#include "keymap.cpp"
#include "MovementLog.cpp"
#include "format.cpp"

class Rules{
private:
    std::string msg;
    std::vector<Piece> active_pieces;
    std::vector<Piece> jumped_pieces;
    std::vector<Tile> tiles;
    std::vector<keymap> boardkeys;
    keymap loadkeymap(char t,int idx);
    Piece eval_piece = Piece("","","null",99);
    Piece to_piece = Piece("","","null",99);
    Piece loadpiece(int idx);
    Tile from_tile = Tile("","",99);
    Tile to_tile = Tile("","",99);
    Tile loadtile(int idx);
    Tile loadtile(int icol, int irow);
    char t2c(std::string t);
    bool same_side(int from_tileidx, int to_tileidx);
    bool tile_isvalid(int ccol,int crow);
    bool cols_isvalid();
    bool rows_isvalid();
    bool diag_isvalid();
    bool pawn_isvalid();
    bool rook_isvalid();
    bool knight_isvalid();
    bool bishop_isvalid();
    bool queen_isvalid();
    bool king_isvalid();

public:
    Rules();
    void set_active_pieces(std::vector<Piece> ap);
    void set_jumped_pieces(std::vector<Piece> jp);
    void set_tiles(std::vector<Tile> t);
    void set_boardkeys(std::vector<keymap> bks);
    void set_eval_piece(Piece p);
    bool get_isvalid(int from_tileidx, int to_tileidx);
    std::string get_msg();
};

class Board {
private:
  std::string board_name;
  std::string board_msg;
  void init_board();
  std::vector<Tile> tiles;
  std::vector<Piece> pieces;
  std::vector<Piece> jumped_pieces;
  void add_tile(Tile new_tile);
  void add_piece(Piece new_piece);
  std::vector<keymap> boardkeys;
  void add_key(keymap new_keymap);
  std::string get_royal(int col);
  void init_pieces();
  void init_key(Tile key_tile, Piece key_piece);
  void init_key(int key_tile_index, int key_piece_index);
  void update_key(int tile_fromidx, int tile_toidx, int up_pieceidx, int key_index);
  keymap get_key(char lookup, int idx);
  Piece get_piece(std::string get_piecekey);
  Piece get_piece(int get_pieceidx);
  Tile get_tile(std::string get_tilecolrow);
  Tile get_tile(int get_tileidx);
  std::vector<MovementLog> mlogs;
  void add_movementlog(MovementLog new_log);
  std::string make_tilecolrow(std::string col,std::string row);
  std::string get_debug();
  std::string draw_board();
  format form;
  std::string initial_board();
  std::string get_charpiece(int pieceidx);
  std::string get_charpiecebase(std::string type,std::string side);
  std::string get_activepieces(std::string side);
  std::string get_jumpedpieces(std::string side);
  std::string get_ajpiecesbase(std::string side, std::string aj, std::vector<keymap> kmc);
  std::string get_pieces_base(std::string type, std::string side, std::vector<keymap> tkm);
  std::string active_board();
  Tile aft = Tile("","",99);
  Tile att = Tile("","",99);
  keymap aftkm = keymap("null","null");
  Piece afp = Piece("","","null",99);
  keymap atfkm = keymap("null","null");
  Piece atp = Piece("","","null",99);
  MovementLog lastlog = MovementLog(99,99,99);
  std::string draw_tile(Tile draw_tile);
  std::string draw_active_board();
  bool update_tile(Tile up_tile);
  bool check_tile(int tileidx);
  bool jump_piece(Piece to_jump, Piece jumped_by);

public:
  Board();
  std::string get_board(bool restart);
  std::string get_colrowstr(int row);
  void set_board_name(std::string new_boardname);
  bool move_piece(int pieceidx,int fromtileidx, int totileidx);
  bool move_piece(std::string piecekey, std::string fromtile_col, std::string fromtile_row, std::string totile_col, std::string totile_row);
  std::string get_boardname();
  bool valid_tile(std::string tile_check,char ft);
  Tile get_active_fromtile();
  Piece get_active_frompiece();
  Tile get_active_totile();
  Piece get_active_topiece();
  MovementLog get_lastlog();
  std::string get_safetile(std::string tile);
  void cleartile_highlights();
  std::string get_boardmsg();
  
};
