#include "../include/Diagnostics.h"
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <unistd.h>
#include <math.h>
#include <sstream>
#include <iomanip>

// Diagnostics class constructor
Diagnostics::Diagnostics(int refresh_rate) {
    Diagnostics::refresh_rate_from_menu = refresh_rate;
}

// Main Diagnostics method for showing diagnostics data
void Diagnostics::ShowDiagnosticsData() {
    // Infinite diagnostics loop
    for (;;) {
        // Diagnostics change visual content
        std::system("clear");
        std::cout << "Zużycie procesora: " << Diagnostics::GetCPUData() << std::endl;

        // Sleep for refresh rate from settings
        usleep(Diagnostics::refresh_rate_from_menu * 1000000);
    }
}

// Getting CPU data method
std::string Diagnostics::GetCPUData() {
    std::string result1 = Diagnostics::GetCommandOutput("cat /proc/stat | grep -w cpu | cut -d' ' -f2-");
    int result1_jiffies = Diagnostics::CpuJiffiesSum(result1, 0);
    int result1_work_jiffies = Diagnostics::CpuJiffiesSum(result1, 1);
    // Sleep 0.1 second
    usleep(100000);
    std::string result2 = Diagnostics::GetCommandOutput("cat /proc/stat | grep -w cpu | cut -d' ' -f2-");
    int result2_jiffies = Diagnostics::CpuJiffiesSum(result2, 0);
    int result2_work_jiffies = Diagnostics::CpuJiffiesSum(result2, 1);

    double work_over_period = result2_work_jiffies - result1_work_jiffies;
    double total_over_period = result2_jiffies - result1_jiffies;
    double percent_cpu = round(work_over_period / total_over_period * 100 * 10) / 10;

    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << percent_cpu;

    std::string cpu_percent = stream.str() + "%";

    return cpu_percent;
}

// Sum splitted cpu all jiffies
int Diagnostics::CpuJiffiesSum(std::string str, int mode) {
    // mode == 0: all jiffies, mode == 1: work jiffies
    std::string delimiter = " ";
    std::string token;

    size_t pos = 0;
    
    int sum = 0;
    int iterator = 0;

    str.erase(0, 1);
    str.erase((str.length()-1), 1);

    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        sum += std::stoi(token);
        str.erase(0, pos + delimiter.length());
        if (mode == 1 && iterator == 2) {
            break;
        }
        iterator++;
    }
    if (mode == 0) {
        sum += std::stoi(str);
    }

    return sum;
}

// Getting terminal commands output method
std::string Diagnostics::GetCommandOutput(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

// Spliting string method
std::string Diagnostics::SplitGivenString(std::string str, std::string delimiter) {
    std::string token;
    std::string output_string = "";

    size_t pos = 0;
    
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        output_string += token;
        str.erase(0, pos + delimiter.length());
    }
    
    return output_string;
}