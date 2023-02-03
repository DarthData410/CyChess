#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include "include/pfdintros.hpp"
#include "Game.cpp"

int main() {

    system("clear");
    // Get introduction:
    srand (time(NULL));
    int	it = rand() % 2;
  
    if (it == 0) { 
        introone(); 
    } 
    else { 
        introtwo(); 
    }

    std::string proceed;
    std::cout << "\n Welcome to CyChess, ready to play? ([Y|y]es/*no): ";\
    std::cin >> proceed;

    Game g;
    if (proceed == "Y" || proceed == "y") {
        system("clear");
        std::string user_action;
        while (g.run()) {
            
            if (g.get_clearscreen()) {
                system("clear");    
            }

            std::cout << g.get_screen();
            if (g.get_iserror()) {
                std::cout << "\n ... Try again: ";\
            }
            std::cin >> user_action;
            g.set_useraction(user_action);
        }
    }

    system("clear");
    introtwo();
    std::cout << g.get_screen() + "\n goodbye! \n";\

    // Finalized return of main:
    return 0;
}