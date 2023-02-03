#include <iostream>

class keymap {
private:
    std::string keyone;
    int ikeyone;
    std::string keytwo;
    int ikeytwo;

public:
    keymap(std::string new_keyone,std::string new_keytwo);
    keymap(int new_ikeyone,int new_ikeytwo);
    keymap(std::string new_keyone,int new_ikeytwo);
    std::string get_keyone();
    int get_ikeyone();
    std::string get_keytwo();
    int get_ikeytwo();
    void update_ikeys(int up_ikeyone, int up_ikeytwo);
    void update_keys(std::string up_keyone, std::string up_keytwo);
    void update_keys(std::string up_keyone, int up_ikeytwo);
};