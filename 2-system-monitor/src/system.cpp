#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() : 
    operatingSystem_(LinuxParser::OperatingSystem()),
    kernelVersion_(LinuxParser::Kernel())
{ 
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    set<int> uniqueIds;
    for (Process process : processes_) {
        uniqueIds.insert(process.Pid());
    }

    std::vector<int> ids = LinuxParser::Pids();
    for (int id: ids) {
        if (uniqueIds.find(id) == uniqueIds.end()) {
            processes_.emplace_back(Process(id));
        }
    }

    for (Process& process: processes_) {
        process.CpuUtilization();
    }

    std::sort(processes_.begin(), processes_.end(), std::greater<Process>());

    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return kernelVersion_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return operatingSystem_;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}