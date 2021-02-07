#include <iostream>
#include <string>

class Diagnostics {
    public: 
        // Class constructor
        Diagnostics(int refresh_rate = 5);

        // Diagnostics methods declarations

        // Main Diagnostics method for showing diagnostics data
        void ShowDiagnosticsData();

    private:
        // Getting CPU data method
        std::string GetCPUData();

        // Getting terminal commands output method
        std::string GetCommandOutput(const char* cmd);

        // Spliting string method
        std::string SplitGivenString(std::string str, std::string delimiter);

        // Sum splitted cpu jiffies
        int CpuJiffiesSum(std::string str, int mode);

        // Private fields

        int refresh_rate_from_menu;
};