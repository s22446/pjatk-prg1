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
    // Creating windows for our data
    // Static data window
    WINDOW * static_data = newwin(4, 200, 0, 0);
    wprintw(static_data, Diagnostics::static_data_cpu.c_str());
    wrefresh(static_data);
    // Cpu usage window
    std::string cpu_usage_str = "Zużycie procesora:";
    WINDOW * cpu_usage_text = newwin(1, (cpu_usage_str.length() + 1), 4, 0);
    wprintw(cpu_usage_text, cpu_usage_str.c_str());
    wrefresh(cpu_usage_text);

    WINDOW * cpu_usage_value = newwin(1, 10, 4, (cpu_usage_str.length() + 1));
    wprintw(cpu_usage_value, (Diagnostics::GetCPUData() + "%").c_str());
    wrefresh(cpu_usage_value);
    // Min cpu usage window
    std::string cpu_min_usage_str = "Minimalne zużycie procesora:";
    WINDOW * cpu_min_usage_text = newwin(1, cpu_min_usage_str.length(), 5, 0);
    wprintw(cpu_min_usage_text, cpu_min_usage_str.c_str());
    wrefresh(cpu_min_usage_text);

    WINDOW * cpu_min_usage_value = newwin(1, 10, 5, (cpu_min_usage_str.length() + 1));
    wprintw(cpu_min_usage_value, (std::to_string(dataStorage["cpu_percent_min"]) + "%%").c_str());
    wrefresh(cpu_min_usage_value);
    // Max cpu usage window
    std::string cpu_max_usage_str = "Maksymalne zużycie procesora:";
    WINDOW * cpu_max_usage_text = newwin(1, cpu_max_usage_str.length(), 6, 0);
    wprintw(cpu_max_usage_text, cpu_max_usage_str.c_str());
    wrefresh(cpu_max_usage_text);

    WINDOW * cpu_max_usage_value = newwin(1, 10, 6, (cpu_max_usage_str.length() + 1));
    wprintw(cpu_max_usage_value, (std::to_string(dataStorage["cpu_percent_max"]) + "%%").c_str());
    wrefresh(cpu_max_usage_value);
    // Processes window
    std::string processes_str = "Liczba procesow:";
    WINDOW * processes_text = newwin(1, processes_str.length(), 7, 0);
    wprintw(processes_text, processes_str.c_str());
    wrefresh(processes_text);

    WINDOW * processes_value = newwin(1, 15, 7, (processes_str.length() + 1));
    wprintw(processes_value, Diagnostics::GetCPUProcesses().c_str());
    wrefresh(processes_value);
    // Getting memory data
    Diagnostics::GetMemoryData();
    // Total memory window
    std::string total_ram_str = "Ilosc pamieci RAM:";
    WINDOW * total_ram_text = newwin(1, total_ram_str.length(), 8, 0);
    wprintw(total_ram_text, total_ram_str.c_str());
    wrefresh(total_ram_text);

    WINDOW * total_ram_value = newwin(1, 15, 8, (total_ram_str.length() + 1));
    wprintw(total_ram_value, Diagnostics::total_ram.c_str());
    wrefresh(total_ram_value);
    // Memory used window
    std::string ram_used_str = "Ilosc pamieci RAM w użyciu:";
    WINDOW * ram_used_text = newwin(1, ram_used_str.length(), 9, 0);
    wprintw(ram_used_text, ram_used_str.c_str());
    wrefresh(ram_used_text);

    WINDOW * ram_used_value = newwin(1, 15, 9, (ram_used_str.length() + 1));
    wprintw(ram_used_value, Diagnostics::ram_used.c_str());
    wrefresh(ram_used_value);
    // Percent memory used window
    std::string percent_ram_used_str = "Procent użytej pamieci RAM:";
    WINDOW * percent_ram_used_text = newwin(1, percent_ram_used_str.length(), 10, 0);
    wprintw(percent_ram_used_text, percent_ram_used_str.c_str());
    wrefresh(percent_ram_used_text);

    WINDOW * percent_ram_used_value = newwin(1, 3, 10, (percent_ram_used_str.length() + 1));
    wprintw(percent_ram_used_value, Diagnostics::percent_ram_used.c_str());
    wrefresh(percent_ram_used_value);
    // Percent memory free window
    std::string percent_ram_free_str = "Procent wolnej pamieci RAM:";
    WINDOW * percent_ram_free_text = newwin(1, percent_ram_free_str.length(), 11, 0);
    wprintw(percent_ram_free_text, percent_ram_free_str.c_str());
    wrefresh(percent_ram_free_text);
    
    WINDOW * percent_ram_free_value = newwin(1, 3, 11, (percent_ram_free_str.length() + 1));
    wprintw(percent_ram_free_value, Diagnostics::percent_ram_free.c_str());
    wrefresh(percent_ram_free_value);
    // Back key window
    std::string back_str = "Nacisnij b oraz potwierdź klawiszem enter, aby wrocic.\n";
    WINDOW * back_text = newwin(1, back_str.length(), 12, 0);
    wprintw(back_text, back_str.c_str());
    wrefresh(back_text);
    // Starting timer for checking if refresh needed
    time_t start = time(0);
    // Creating another thread to listen for b key
    pthread_t tId;
    (void) pthread_create(&tId, 0, Diagnostics::userInput_thread, 0);
    // Infinite diagnostics loop
    for (;;) {
        if (double seconds_since_start = difftime(time(0), start) >= Diagnostics::refresh_rate_from_menu) {
            // Cleaning and filling windows with data again

            // Cpu usage
            wclear(cpu_usage_value);
            wprintw(cpu_usage_value, (Diagnostics::GetCPUData() + "%").c_str());
            wrefresh(cpu_usage_value);

            // Min cpu usage
            wclear(cpu_min_usage_value);
            wprintw(cpu_min_usage_value, (std::to_string(dataStorage["cpu_percent_min"]) + "%%").c_str());
            wrefresh(cpu_min_usage_value);

            // Max cpu usage
            wclear(cpu_max_usage_value);
            wprintw(cpu_max_usage_value, (std::to_string(dataStorage["cpu_percent_max"]) + "%%").c_str());
            wrefresh(cpu_max_usage_value);

            // Processes
            wclear(processes_value);
            wprintw(processes_value, Diagnostics::GetCPUProcesses().c_str());
            wrefresh(processes_value);

            // Getting memory data
            Diagnostics::GetMemoryData();

            // Memory used  
            wclear(ram_used_value);
            wprintw(ram_used_value, Diagnostics::ram_used.c_str());
            wrefresh(ram_used_value);

            // Percent memory
            wclear(percent_ram_used_value);
            wprintw(percent_ram_used_value, Diagnostics::percent_ram_used.c_str());
            wrefresh(percent_ram_used_value);

            // Percent memory free
            wclear(percent_ram_free_value);
            wprintw(percent_ram_free_value, Diagnostics::percent_ram_free.c_str());
            wrefresh(percent_ram_free_value);
            // Reseting timer
            start = time(0);
        }

        if (!keep_running) {
            // Suspending timer thread
            (void) pthread_join(tId, NULL);
            keep_running = true;
            // Closing ncurses
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

    std::string cpu_percent = std::to_string(percent_cpu_int) + "%%";

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
void Diagnostics::GetMemoryData() {
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

    Diagnostics::total_ram = totalPhysMemMB_string + " MB";
    Diagnostics::ram_used = physMemUsedMB_string + " MB";
    Diagnostics::percent_ram_used = RAM_percentage_use_string + "%%";
    Diagnostics::percent_ram_free = RAM_percentage_free_string + "%%";
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
