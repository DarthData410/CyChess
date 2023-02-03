#include <iostream>
#include "format.hpp"

format::format() { }

std::string format::gray() {
    return _gray;
}
std::string format::red() {
    return _red;
}
std::string format::green() {
    return _green;
}
std::string format::gold() {
    return _gold;
}
std::string format::blue() {
    return _blue;
}
std::string format::dblue() {
    return _dblue;
}
std::string format::purple() {
    return _purple;
}
std::string format::turq() {
    return _turq;
}
std::string format::white() {
    return _white;
}
std::string format::none() {
    return _none;
}
std::string format::gb(std::string fmt) {
    return gray()+fmt+none();
}