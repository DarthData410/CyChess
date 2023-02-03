#include <iostream>

class MovementLog {
private:
    int pieceidx;
    int fromtileidx;
    int totileidx;

public:
    MovementLog(int new_pieceidx,int new_fromtileidx, int new_totileidx);
    int get_pieceidx();
    int get_fromtileidx();
    int get_totileidx();
};