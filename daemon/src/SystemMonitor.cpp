#include "SystemMonitor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <cctype>
#include <signal.h>
#include <errno.h>
#include <unordered_set>

namespace fs = std::filesystem;


int SystemMonitor::cpuUsage() const{
    return m_cpuUsage;
}
int SystemMonitor::ramUsage() const{
    return m_ramUsage;
}

const std::vector<ProcessData>& SystemMonitor::getProcessList() const {
    return processList;
}

bool SystemMonitor::killProcess(int pid)
{
    if (::kill(pid, SIGTERM) == 0) {
        return true;
    }

    return false;
}

bool isSystemService(const std::string& processName) {
    static const std::unordered_set<std::string> ignored = {
        "plasmashell", "kwin_wayland", "kwin_x11", "kscreenlocker_welcome",
        "org.kde.discovernotifier", "DiscoverNotifier", "kded6", "kded5",
        "kglobalaccel6", "kglobalaccel5", "ksystemstats", "pipewire", "wireplumber",
        "dbus-daemon", "dbus-broker", "systemd", "xwayland", "Xwayland", "agent"
    };

    std::string lowerName = processName;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    for (const auto& item : ignored) {
        std::string lowerItem = item;
        std::transform(lowerItem.begin(), lowerItem.end(), lowerItem.begin(), ::tolower);
        if (lowerName.find(lowerItem) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool SystemMonitor::isGuiApplication(int pid) {

    std::string envPath = "/proc/" + std::to_string(pid) + "/environ";
    std::ifstream envFile(envPath, std::ios::binary);
    if (!envFile.is_open()) return false;

    std::string env;
    while (std::getline(envFile, env, '\0')) {
        if (env.rfind("WAYLAND_DISPLAY=", 0) == 0 || env.rfind("DISPLAY=", 0) == 0) {
            return true;
        }
    }

    return false;
}

void SystemMonitor::readSystemStats(){
    std::ifstream cpuFile(cpuPath);
    std::ifstream ramFile(ramPath);

    if(!cpuFile.is_open() || !ramFile.is_open()){
        std::cerr << "[sysmond] Error: Failed to open system files in /proc\n";
        return ;
    }

    std::string label;
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    if (cpuFile >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal) {
        unsigned long long idleTime = idle + iowait;
        unsigned long long totalTime = idleTime + user + nice + system + irq + softirq + steal;

        unsigned long long totalDiff = totalTime - prevTotalCpu;
        unsigned long long idleDiff  = idleTime - prevIdleCpu;

        if (totalDiff > 0) {
            m_cpuUsage = static_cast<int>((static_cast<double>(totalDiff - idleDiff) / totalDiff) * 100.0);
        }

        prevTotalCpu = totalTime;
        prevIdleCpu  = idleTime;
    }
    cpuFile.close();

    std::string key;
    unsigned long long value;
    unsigned long long memTotal = 0;
    unsigned long long memAvailable = 0;

    while (ramFile >> key >> value) {
        if (key == "MemTotal:") {
            memTotal = value;
        } else if (key == "MemAvailable:") {
            memAvailable = value;
            break;
        }
        std::string unit;
        ramFile >> unit;
    }

    if (memTotal > 0) {
        m_ramUsage = static_cast<int>((static_cast<double>(memTotal - memAvailable) / memTotal) * 100.0);
    }

    ramFile.close();
}


void SystemMonitor::readSystemProcesses() {
    processList.clear();
    for (const auto& entry : fs::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;

        std::string dirName = entry.path().filename().string();

        // 1. Filter out non-numeric directories (like /proc/sys, /proc/net, etc.)
        if (std::all_of(dirName.begin(), dirName.end(), ::isdigit)) {
            int pid = std::stoi(dirName);
            if(isGuiApplication(pid)){
            ProcessData proc;
            proc.pid = pid;

            // 2. Read Process Name from /proc/[pid]/comm
            std::ifstream commFile(entry.path() / "comm");
            if (commFile.is_open()) {
                std::getline(commFile, proc.name);
            }

            // 3. Read Memory Usage (VmRSS) from /proc/[pid]/status
            std::ifstream statusFile(entry.path() / "status");
            std::string line;
            while (std::getline(statusFile, line)) {
                if (line.rfind("VmRSS:", 0) == 0) { // Starts with VmRSS:
                    sscanf(line.c_str(), "VmRSS: %ld kB", &proc.ramKb);
                    break;
                }
            }

            processList.push_back(proc);
            }
        }
    }

    std::cout << "Successfully parsed " << processList.size() << " processes." << std::endl;
}





