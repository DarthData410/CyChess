#include <iostream>
#include "MovementLog.hpp"

MovementLog::MovementLog(int new_pieceidx, int new_fromtileidx, int new_totileidx)
: pieceidx(new_pieceidx), fromtileidx(new_fromtileidx), totileidx(new_totileidx) {
    
}

int MovementLog::get_pieceidx() {
    return pieceidx;
}
int MovementLog::get_fromtileidx() {
    return fromtileidx;
}
int MovementLog::get_totileidx() {
    return totileidx;
}