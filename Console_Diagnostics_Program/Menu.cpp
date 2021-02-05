#include "Menu.h"
#include "Diagnostics.h"
#include <iostream>
#include <string>

Menu::Menu() {

}

void Menu::StartMenu() {
    // Infinite menu loop
    for (;;) {
        // Main menu visual content
        std::cout << "Menu główne" << std::endl;
        std::cout << "Wciśnij klawisz aby przejść do wybranej sekcji:" << std::endl;
        std::cout << "1. Wyświetl dane diagnostyczne" << std::endl;
        std::cout << "2. Ustawienia" << std::endl;
        std::cout << "3. Wyjdź" << std::endl;
    }
}