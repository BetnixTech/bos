
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    std::cout << "CPU Info:\n";
    while (std::getline(cpuinfo, line)) {
        if (line.find("model name") != std::string::npos) {
            std::cout << line << std::endl;
        }
    }

    std::ifstream meminfo("/proc/meminfo");
    std::cout << "\nMemory Info:\n";
    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal") != std::string::npos ||
            line.find("MemAvailable") != std::string::npos)
            std::cout << line << std::endl;
    }

    return 0;
}
