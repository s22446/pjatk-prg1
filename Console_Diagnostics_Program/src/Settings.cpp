#include "../include/Settings.h"
#include <iostream>
#include <ctype.h>
#include <unistd.h>

// Settings class constructor
Settings::Settings() {
    // Setting default settings
    Settings::refresh_rate = 5;
}

// Changing refresh rate method
void Settings::ChangeRefreshRate() {
    Settings::is_first_loop = true;
    // Infinite settings change loop
    for (;;) {
        // Settings change visual content
        std::system("clear");
        std::cout << "Podaj częstotliwość odświeżania danych diagnostycznych." << std::endl;
        std::cout << "Zakres: 1 - 10 sekund" << std::endl;
        std::cout << "Twoja zmiana: " << std::endl;
        // Getting user's input
        if (Settings::is_first_loop) {
            std::cin.ignore();
            Settings::is_first_loop = false;
        }
        std::getline(std::cin, Settings::change_refresh_rate_input);

        // Checking if value entered is digit
        if (Settings::IsInteger(Settings::change_refresh_rate_input)) {
            // Checking if value is in proper range
            int refresh_rate = std::stoi(Settings::change_refresh_rate_input);
            if (refresh_rate >= 1 && refresh_rate <= 10) {
                // Setting new refresh rate
                Settings::refresh_rate = refresh_rate;
                std::cout << "Pomyślnie zmieniono! Wznawianie za 2 sekundy..." << std::endl;
                usleep(2 * Settings::sleep_time);
                return;
            }
            else {
                std::cout << "Podano liczbę spoza zakresu! Wznawianie za 2 sekundy..." << std::endl;
                usleep(2 * Settings::sleep_time);
            }
        }
        else {
            std::cout << "Proszę podać liczbę! Wznawianie za 2 sekundy..." << std::endl;
            usleep(2 * Settings::sleep_time);
        }
    }
}

// Checking if string is number function method
bool Settings::IsInteger(const std::string & text) {
    if(text.empty() || ((!isdigit(text[0])) && (text[0] != '-') && (text[0] != '+'))){
        return false;
    } 

    char * pointer;
    strtol(text.c_str(), &pointer, 10);

    return (*pointer == 0);
}