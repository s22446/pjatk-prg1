#include <iostream>
#include <string>
#include <map>

class Diagnostics {
    public: 
        // Class constructor
        Diagnostics(int refresh_rate = 5);

        // Diagnostics methods declarations

        // Main Diagnostics method for showing diagnostics data
        void ShowDiagnosticsData();

    private:
        // Printing diagnostics data method
        void PrintDiagnosticsData();

        // Getting static CPU data method
        void GetStaticCPUData();

        // Getting CPU data method
        std::string GetCPUData();

        // Getting CPU processes method
        std::string GetCPUProcesses();

        // Getting memory data method
        std::string GetMemoryData();

        // Getting terminal commands output method
        std::string GetCommandOutput(const char* cmd);

        // Spliting string method
        std::string SplitGivenString(std::string str, std::string delimiter);

        // Sum splitted cpu jiffies
        int CpuJiffiesSum(std::string str, int mode);

        // Method for checking if user pressed b key
        static void* userInput_thread(void*);

        // Private fields

        int refresh_rate_from_menu;

        // Diagnostics data storage

        std::map <std::string, double> dataStorage;

        std::string static_data_cpu;
};