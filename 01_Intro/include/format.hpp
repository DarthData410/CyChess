#include <iostream>

class format{
private:
    std::string _gray = "\e[0;100m";
    std::string _red =  "\e[41m"; //"\e[0;101m";
    std::string _green = "\e[0;102m";
    std::string _gold = "\e[0;103m";
    std::string _blue = "\e[0;104m";
    std::string _dblue = "\e[44m";
    std::string _purple = "\e[0;105m";
    std::string _turq = "\e[0;106m";
    std::string _white = "\e[0;107m";
    std::string _none = "\e[0;0m";

public:
    format();
    std::string gray();
    std::string red();
    std::string green();
    std::string gold();
    std::string blue();
    std::string dblue();
    std::string purple();
    std::string turq();
    std::string white();
    std::string none();
    std::string gb(std::string fmt);

};