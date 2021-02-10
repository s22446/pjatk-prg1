#include <iostream>
#include <string>
#include <map>

class Diagnostics {
    public: 
        // Class constructor
        Diagnostics(int refresh_rate = 5, int memory_format = 2, int decimal_places = 0);

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
        void GetMemoryData();

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
        int memory_format_from_menu;
        int decimal_places_from_menu;

        // Diagnostics data storage

        std::map <std::string, double> dataStorage;

        std::string static_data_cpu;

        std::string total_ram;

        std::string ram_used;

        double percent_ram_used;

        double percent_ram_free;
};
