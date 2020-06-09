#include <dirent.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T readValueFromFile(const std::string &path, const std::string &argumentName) {
  T value;
  std::ifstream filestream(path);

  if (filestream.is_open()) {
    std::string line;
    
    while (std::getline(filestream, line)) {
      std::istringstream lineStream(line);
      std::string argument;
      lineStream >> argument;
      
      if (argument == argumentName) {
        lineStream >> value;
        return value;
      }
    }
  }

  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  const std::string path(kProcDirectory + kMeminfoFilename);

  const float totalMemory = readValueFromFile <float> (path, "MemTotal:");
  const float freeMemory = readValueFromFile <float> (path, "MemFree:");
 
  const float usedMemory = totalMemory - freeMemory;
  return totalMemory == 0.0 ? 0.0 : (usedMemory/totalMemory);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptimeInSeconds = 0; 
  
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    filestream >> uptimeInSeconds;
  }

  return uptimeInSeconds;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long totalTime = 0;

  std::stringstream path;
  path << kProcDirectory << pid << kStatFilename;
    
  std::ifstream filestream(path.str());
  if (filestream.is_open()) {

    const int position = 13;
    std::string uselessValue;
    for (int i = 0; i < position; i++) {
      if (!(filestream >> uselessValue)) {

        return 10000;
      }
    }

    long userTimeTicks, kernelTimeTicks = 0, userChildrenTimeTicks = 0, kernelChildrenTimeTicks = 0;
    if (filestream >> userTimeTicks >> kernelTimeTicks >> userChildrenTimeTicks >> kernelChildrenTimeTicks) {
        totalTime = (userTimeTicks + kernelTimeTicks + userChildrenTimeTicks + kernelChildrenTimeTicks);
    }
  }
  
  return totalTime/sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::vector <long> cpuJiffies = CpuUtilization();
  
  return cpuJiffies[kUser_] + cpuJiffies[kNice_] + cpuJiffies[kSystem_] + 
         cpuJiffies[kIRQ_] + cpuJiffies[kSoftIRQ_] + cpuJiffies[kSteal_];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::vector <long> cpuJiffies = CpuUtilization();
  return cpuJiffies[kIdle_] + cpuJiffies[kIOwait_];
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() { 
  std::vector<long> cpuJiffies;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::string cpu;
    filestream >> cpu;
    if (cpu == "cpu") {
      for (int i = 0; i < CPUStates::END; i++) {
        long value;
        filestream >> value;
        cpuJiffies.push_back(value);
      }
    }
  }

  return cpuJiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int numberProcesses = 0; 
  std::string path(kProcDirectory + kStatFilename);

  numberProcesses = readValueFromFile <int> (path, "processes");
  return numberProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int processesRunning = 0; 
  std::string path(kProcDirectory + kStatFilename);

  processesRunning = readValueFromFile <int> (path, "procs_running");
  return processesRunning;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::string command = "No command";

  std::stringstream path;
  path << kProcDirectory << pid << kCmdlineFilename;

  std::ifstream filestream(path.str());
    if (filestream.is_open()) {
    
    std::string line;
      if (std::getline(filestream, line)) {
        command = line;
      }
  }

  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::stringstream path;
  path << kProcDirectory << pid << kStatusFilename;

  int ramInKiloBytes = readValueFromFile <int> (path.str(), "VmSize:");
  std::stringstream ramInMegaBytes;
  ramInMegaBytes << (ramInKiloBytes/1000);

  return ramInMegaBytes.str(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::string userId = "0000";
  std::stringstream path;
  path << kProcDirectory << pid << kStatusFilename;

  userId = readValueFromFile <std::string> (path.str(), "Uid:");
  return userId;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string userId = Uid(pid);

  string userName = "No one";
  std::ifstream filestream(kPasswordPath);
  
  if (filestream.is_open()) {
    std::string line;
    
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      std::string currentId;
      std::string xField;
      
      if (linestream >> userName >> xField >> currentId) {
        if (currentId == userId) {
          return userName;
        }
      }
    }
  }

  return userName;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long upTimeInClockTicks = 0;

  std::stringstream path;
  path << kProcDirectory << pid << kStatFilename;
    
  std::ifstream filestream(path.str());
  if (filestream.is_open()) {

    const int position = 22;
    std::string value;
    for (int i = 0; i < position; i++) {
      if (!(filestream >> value)) {
        return 10000;
      }
    }
    upTimeInClockTicks = std::stol(value);
  }
  
  return upTimeInClockTicks/sysconf(_SC_CLK_TCK);
}