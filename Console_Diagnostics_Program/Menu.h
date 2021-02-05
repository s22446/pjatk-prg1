#include "Settings.h"
#include <iostream>

class  Menu {
    public: 
        // Class constructor
        Menu();

        // Menu methods declarations

        // Starting main menu method
        void StartMenu();
        // Setting menu method
        void SettingsMenu();

    private:
        // Private fields
        char user_input;
        char user_settings_input;

        Settings settings;
};