#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <string>
#include <vector>

struct ProcessData {
    int pid;
    std::string name;
    long ramKb = 0;
};

class SystemMonitor {
public:
    // SystemMonitor();

    int cpuUsage() const;
    int ramUsage() const;
    void readSystemStats();
    void readSystemProcesses();
    const std::vector<ProcessData>& getProcessList() const;
    bool killProcess(int pid);

private:
    std::string cpuPath = "/proc/stat";
    std::string ramPath = "/proc/meminfo";
    std::string processesPath = "/proc";


    int m_cpuUsage = 0;
    int m_ramUsage = 0;
    std::vector<ProcessData> processList;

    unsigned long long prevTotalCpu = 0;
    unsigned long long prevIdleCpu = 0;
};

#endif