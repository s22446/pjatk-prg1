#include "../include/Menu.h"
#include <iostream>
#include <string>
#include <unistd.h>

// Menu class constructor
Menu::Menu() {

}

// Main menu method
void Menu::StartMenu() {
    // Infinite menu loop
    for (;;) {
        // Main menu visual content
        std::system("clear");
        std::cout << "Menu główne" << std::endl;
        std::cout << "Wciśnij klawisz aby przejść do wybranej sekcji:" << std::endl;
        std::cout << "1. Wyświetl dane diagnostyczne" << std::endl;
        std::cout << "2. Ustawienia" << std::endl;
        std::cout << "3. Wyjdź" << std::endl;
        // Getting user's menu choice
        std::cin >> Menu::user_input;
        // Creating diagnostics object
        Menu::diagnostics = Diagnostics(Menu::settings.refresh_rate);
        switch (Menu::user_input) {
            case '1':
                Menu::diagnostics.ShowDiagnosticsData();
                break;
            
            case '2':
                Menu::SettingsMenu();
                break;

            case '3':
                exit(1);
                break;

            default:
                std::cout << "Proszę wybrać prawidłową opcję! Wznawianie za 2 sekundy..." << std::endl;
                usleep(2 * Menu::settings.sleep_time);
        }
    }
}

// Settings method
void Menu::SettingsMenu() {
    // Infinite settings loop
    for (;;) {
        // Settings visual content
        std::system("clear");
        std::cout << "Ustawienia:" << std::endl;
        std::cout << "Wciśnij klawisz aby zmienić ustawienie przy danej cyfrze:" << std::endl;
        std::cout << "1. Częstotliwość odświeżania danych diagnostycznych" << std::endl;
        std::cout << "Naciśnij b, aby wrócić." << std::endl;
        // Getting user's settings choice
        std::cin >> Menu::user_settings_input;
        
        switch (Menu::user_settings_input)
        {
            case '1':
                Menu::settings.ChangeRefreshRate();
                break;

            case 'B':
            case 'b':
                return;
            
            default:
                std::cout << "Proszę wybrać prawidłową opcję! Wznawianie za 2 sekundy..." << std::endl;
                usleep(2 * Menu::settings.sleep_time);
        }
    }
}