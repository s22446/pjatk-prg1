#include "Menu.h"
#include <iostream>

int main() {
    // Starting screen
    std::cout << "Program diagnostyczny" << std::endl;
    std::cout << "Projekt zaliczeniowy z programowania: Michał Siwek - nr indeksu: s22446" << std::endl;
    std::cout << "Wciśnij enter, aby przejść do menu.";
    getchar();
    // Menu object
    Menu menu = Menu();
    // Starting menu
    menu.StartMenu();

    return 0;
}