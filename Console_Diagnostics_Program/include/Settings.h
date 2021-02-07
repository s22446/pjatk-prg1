#include <iostream>
#include <string>

class Settings {
    public:
        // Class constructor
        Settings();

        // Settings methods declarations

        // Changing refresh_rate
        void ChangeRefreshRate();
        // Checking if string is number function
        bool IsInteger(const std::string & text);

        // Public fields
        int refresh_rate;

        const unsigned int sleep_time = 1000000;
    private:
        // Private fields
        std::string change_refresh_rate_input;

        // Variable for checking if method is in first loop
        bool is_first_loop;
};
