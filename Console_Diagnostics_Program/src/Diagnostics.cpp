#include "../include/Diagnostics.h"
#include "sys/types.h"
#include "sys/sysinfo.h"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <unistd.h>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <pthread.h>
#include <ncurses.h>

static volatile bool keep_running = true;

// Diagnostics class constructor
Diagnostics::Diagnostics(int refresh_rate) {
    Diagnostics::refresh_rate_from_menu = refresh_rate;
    Diagnostics::GetStaticCPUData();
}

// Main Diagnostics method for showing diagnostics data
void Diagnostics::ShowDiagnosticsData() {
    // Creating ncurses screen
    initscr();   
    mvprintw(0, 0, Diagnostics::static_data_cpu.c_str());     
    mvprintw(4, 0, ("Zużycie procesora: " + Diagnostics::GetCPUData() + " | Min: " + std::to_string(dataStorage["cpu_percent_min"]) + "% | Max: " + std::to_string(dataStorage["cpu_percent_max"]) + "%\n").c_str());
    refresh();
    mvprintw(5, 0, ("Liczba procesow: " + Diagnostics::GetCPUProcesses() + "\n").c_str());
    refresh();
    mvprintw(6, 0, (Diagnostics::GetMemoryData() + "\n").c_str());
    refresh();
    mvprintw(10, 0, "Nacisnij b oraz potwierdź klawiszem enter, aby wrocic.\n");
    refresh();
    // Starting timer for checking if refresh needed
    time_t start = time(0);
    // Creating another thread to listen for b key
    pthread_t tId;
    (void) pthread_create(&tId, 0, Diagnostics::userInput_thread, 0);
    // Infinite diagnostics loop
    for (;;) {
        if (double seconds_since_start = difftime(time(0), start) >= Diagnostics::refresh_rate_from_menu) {
            mvprintw(0, 0, Diagnostics::static_data_cpu.c_str());     
            mvprintw(4, 0, ("Zużycie procesora: " + Diagnostics::GetCPUData() + " | Min: " + std::to_string(dataStorage["cpu_percent_min"]) + "% | Max: " + std::to_string(dataStorage["cpu_percent_max"]) + "%\n").c_str());
            refresh();
            mvprintw(5, 0, ("Liczba procesow: " + Diagnostics::GetCPUProcesses() + "\n").c_str());
            refresh();
            mvprintw(6, 0, (Diagnostics::GetMemoryData() + "\n").c_str());
            refresh();
            mvprintw(10, 0, "Nacisnij b oraz potwierdź klawiszem enter, aby wrocic.\n");
            refresh();
            start = time(0);
        }

        if (!keep_running) {
            (void) pthread_join(tId, NULL);
            keep_running = true;
            endwin();
            return;
        }
    }
}

// Method for checking if user pressed b key
void* Diagnostics::userInput_thread(void*)
{
    while(keep_running) {
        if (std::cin.get() == 'b')
        {
            //! desired user input 'q' received
            keep_running = false;
        }
    }
}

// Getting static CPU data method
void Diagnostics::GetStaticCPUData() {
    std::string cpu_name = Diagnostics::GetCommandOutput("cat /proc/cpuinfo | grep -w 'model name' | cut -d' ' -f3-");
    std::string cpu_cores = Diagnostics::GetCommandOutput("cat /proc/cpuinfo | grep -w 'cpu cores' | cut -d' ' -f3-");
    std::string cpu_clock = Diagnostics::GetCommandOutput("cat /proc/cpuinfo | grep -w 'cpu MHz' | cut -d' ' -f3-");
    std::string cpu_l2cache = Diagnostics::GetCommandOutput("cat /proc/cpuinfo | grep -w 'cache size' | cut -d' ' -f3-");

    Diagnostics::static_data_cpu = "Nazwa procesora: " + cpu_name + "Liczba rdzeni: " + cpu_cores + "Taktowanie procesora w MHz: " + cpu_clock + "Cache L2: " + cpu_l2cache;
}

// Getting CPU processes method
std::string Diagnostics::GetCPUProcesses() {
    std::string processes = Diagnostics::GetCommandOutput("cat /proc/stat | grep -w processes | cut -d' ' -f2-");

    return processes;
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
    double percent_cpu = round(work_over_period / total_over_period * 100);
    int percent_cpu_int = (int)percent_cpu;

    std::string cpu_percent = std::to_string(percent_cpu_int) + "%";

    if (dataStorage.find("cpu_percent_max") == dataStorage.end()) {
        dataStorage["cpu_percent_max"] = percent_cpu_int;
    }
    else {
        if (dataStorage["cpu_percent_max"] < percent_cpu_int) {
            dataStorage["cpu_percent_max"] = percent_cpu_int;
        }
    }

    if (dataStorage.find("cpu_percent_min") == dataStorage.end()) {
        dataStorage["cpu_percent_min"] = percent_cpu_int;
    }
    else {
        if (dataStorage["cpu_percent_min"] > percent_cpu_int) {
            dataStorage["cpu_percent_min"] = percent_cpu_int;
        }
    }

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

// Getting memory data method
std::string Diagnostics::GetMemoryData() {
    struct sysinfo memInfo;

    sysinfo (&memInfo);

    // Total RAM
    long long totalPhysMem = memInfo.totalram;
    totalPhysMem *= memInfo.mem_unit;
    double totalPhysMemMB = round(totalPhysMem / 1024 / 1024);
    // Creating string with total RAM
    std::string totalPhysMemMB_string = std::to_string((int)totalPhysMemMB);
    
    // RAM used
    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    physMemUsed *= memInfo.mem_unit;
    double physMemUsedMB = round(physMemUsed / 1024 / 1024);
    // Creating string with RAM used
    std::string physMemUsedMB_string = std::to_string((int)physMemUsedMB);

    // Percentage use
    double RAM_percentage_use = round(physMemUsedMB / totalPhysMemMB * 100);
    // Creating string with RAM used
    std::string RAM_percentage_use_string = std::to_string((int)RAM_percentage_use);

    // Percentage free
    int RAM_percentage_free = 100 - (int)RAM_percentage_use;
    std::string RAM_percentage_free_string = std::to_string(RAM_percentage_free);

    return "Ilosc pamieci RAM: " + totalPhysMemMB_string + " MB\nIlosc pamieci RAM w użyciu: " + physMemUsedMB_string + "MB\nProcent użytej pamieci RAM: " + RAM_percentage_use_string + "%\nProcent wolnej pamieci RAM: " + RAM_percentage_free_string + "%\n";
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
