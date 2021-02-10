#include <iostream>
#include <string>

class Settings {
    public:
        // Class constructor
        Settings();

        // Settings methods declarations

        // Changing refresh_rate
        void ChangeRefreshRate();
        // Changing memory format
        void ChangeMemoryFormat();
        // Changing decimal places
        void ChangeDecimalPlaces();
        // Checking if string is number function
        bool IsInteger(const std::string & text);

        // Public fields

        // Data refresh rate
        int refresh_rate;
        // Memory format (0 - B, 1 - KB, 2 - MB)
        int memory_format;
        // Decimal places (0, 1, 2)
        int decimal_places;

        const unsigned int sleep_time = 1000000;
    private:
        // Private fields

        char memory_format_user_input;
        char decimal_places_user_input;

        // Variable with user's input for changing refresh rate for getline
        std::string change_refresh_rate_input;

        // Variable for checking if method is in first loop
        bool is_first_loop;
};
