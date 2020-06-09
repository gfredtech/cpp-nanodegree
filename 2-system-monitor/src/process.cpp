#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int id) :
    _id(id)
{
    _userName = LinuxParser::User(_id);
    _command = LinuxParser::Command(_id);
}

// TODO: Return this process's ID
int Process::Pid() { 
    return _id;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    const int systemUpTimeSeconds = LinuxParser::UpTime();
    const int totalTimeActiveSeconds = LinuxParser::ActiveJiffies(_id);
    const int processUpTimeSeconds = LinuxParser::UpTime(_id);
    
    const int totalTimeSiceStartUp = systemUpTimeSeconds - processUpTimeSeconds;

    _cpuUtilization = (1.0*totalTimeActiveSeconds)/totalTimeSiceStartUp;
    return (_cpuUtilization);
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return _command; 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(_id);
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return _userName; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(_id); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator>(Process const& a) const { 
    return this->_cpuUtilization > a._cpuUtilization; 
}