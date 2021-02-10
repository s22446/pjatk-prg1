#include "../include/Settings.h"
#include <iostream>
#include <ctype.h>
#include <unistd.h>

// Settings class constructor
Settings::Settings() {
    // Setting default settings

    Settings::refresh_rate = 5;
    Settings::memory_format = 2;
    Settings::decimal_places = 0;
}

// Changing refresh rate method
void Settings::ChangeRefreshRate() {
    Settings::is_first_loop = true;
    // Infinite settings change loop
    for (;;) {
        // Settings change visual content
        std::system("clear");
        std::cout << "Podaj częstotliwość odświeżania danych diagnostycznych.\nAktualna wartość: " << Settings::refresh_rate << " sekund" << std::endl;
        std::cout << "Zakres: 1 - 10 sekund" << std::endl;
        std::cout << "Twoja zmiana:" << std::endl;
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

// Changing memory format
void Settings::ChangeMemoryFormat() {
    // Infinite settings change loop
    for (;;) {
        // Settings change visual content
        std::system("clear");
        std::cout << "Podaj format wyświetlania pamięci RAM.\nAktualny format: ";
        if (Settings::memory_format == 0) {
            std::cout << "B" << std::endl;
        }
        else if (Settings::memory_format == 1) {
            std::cout << "KB" << std::endl;
        }
        else if (Settings::memory_format == 2) {
            std::cout << "MB" << std::endl;
        }
        std::cout << "Możliwe opcje: 0 - bajty, 1 - kilobajty, 2 - megabajty" << std::endl;
        std::cout << "Twoja zmiana:" << std::endl;
        // Getting user's input
        std::cin >> Settings::memory_format_user_input;
        bool success = false;

        switch (Settings::memory_format_user_input) {
            case '0':
                Settings::memory_format = 0;
                success = true;
                break;
            case '1':
                Settings::memory_format = 1;
                success = true;
                break;
            case '2':
                Settings::memory_format = 2;
                success = true;
                break;
            default:
                std::cout << "Proszę wybrać prawidłową opcję! Wznawianie za 2 sekundy..." << std::endl;
                usleep(2 * Settings::sleep_time);
        }
        if (success) {
            std::cout << "Pomyślnie zmieniono! Wznawianie za 2 sekundy..." << std::endl;
            usleep(2 * Settings::sleep_time);
            return;
        }
    }
}

// Changing decimal places
void Settings::ChangeDecimalPlaces() {
    // Infinite settings change loop
    for (;;) {
        // Settings change visual content
        std::system("clear");
        std::cout << "Podaj Dokładność miejsc po przecinku.\nAktualna wartość: " << Settings::decimal_places << " miejsc po przecinku." << std::endl;
        std::cout << "Zakres: 0 - 2 miejsc" << std::endl;
        std::cout << "Twoja zmiana:" << std::endl;
        // Getting user's input
        std::cin >> Settings::decimal_places_user_input;
        bool success = false;

        switch (Settings::decimal_places_user_input) {
            case '0':
                Settings::decimal_places = 0;
                success = true;
                break;
            case '1':
                Settings::decimal_places = 1;
                success = true;
                break;
            case '2':
                Settings::decimal_places = 2;
                success = true;
                break;
            default:
                std::cout << "Proszę wybrać prawidłową opcję! Wznawianie za 2 sekundy..." << std::endl;
                usleep(2 * Settings::sleep_time);
        }
        if (success) {
            std::cout << "Pomyślnie zmieniono! Wznawianie za 2 sekundy..." << std::endl;
            usleep(2 * Settings::sleep_time);
            return;
        }
    }
}