#include <iostream>
#include <string>
#include "keymap.hpp"

keymap::keymap(std::string new_keyone, std::string new_keytwo)
    : keyone(new_keyone), keytwo(new_keytwo) { }
keymap::keymap(int new_ikeyone, int new_ikeytwo)
    : ikeyone(new_ikeyone), ikeytwo(new_ikeytwo) { }
keymap::keymap(std::string new_keyone,int new_ikeytwo) 
    : keyone(new_keyone), ikeytwo(new_ikeytwo) { }

std::string keymap::get_keyone() {
    return keyone;
}
int keymap::get_ikeyone() {
    return ikeyone;
}
std::string keymap::get_keytwo() {
    return keytwo;
}
int keymap::get_ikeytwo() {
    return ikeytwo;
}
void keymap::update_ikeys(int up_ikeyone,int up_ikeytwo) {
    ikeyone = up_ikeyone;
    if(up_ikeyone==99) {
        keyone = "null";
    }
    ikeytwo = up_ikeytwo;
    if(up_ikeytwo==99) {
        keytwo = "null";
    }
}

void keymap::update_keys(std::string up_keyone, std::string up_keytwo) {
    keyone = up_keyone;
    keytwo = up_keytwo;
}
void keymap::update_keys(std::string up_keyone, int up_ikeytwo) {
    keyone = up_keyone;
    ikeytwo = up_ikeytwo;
}
