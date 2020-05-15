#include <dirent.h>
#include <unistd.h>
#include <sys/time.h>
#include <string>
#include <vector>
#include <map>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string name;  // name of the memeory info
  float size;     // size of the memory info
  string unit;  // unit of memory in kB
  std::map <string, float> memoryUtilMap;
  string line;
  std::ifstream inFile(kProcDirectory + kMeminfoFilename); // open /proc/meminfo
  if (inFile.is_open()) {
    while(std::getline(inFile, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> size >> unit;
      memoryUtilMap[name] = size;
    }
  }
  
  return (memoryUtilMap["MemTotal"] - (memoryUtilMap["MemFree"] + 
          memoryUtilMap["Buffers"]+memoryUtilMap["Cached"])) / memoryUtilMap["MemTotal"]; 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  long upTime;
  long idle; 
  string line;
  std::ifstream inFile(kProcDirectory + kUptimeFilename);
  if (inFile.is_open()) {
    while(std::getline(inFile, line)) {
      std::istringstream linestream(line);
      linestream >> upTime >> idle;
    }
  }
  return upTime; 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> utilization = LinuxParser::CpuUtilization();
  return stol(utilization[kUser_]) + stol(utilization[kNice_]) + stol(utilization[kSystem_]) +
      stol(utilization[kIRQ_]) + stol(utilization[kSoftIRQ_]) + stol(utilization[kSteal_]) + 
      stol(utilization[kGuest_]) + stol(utilization[kGuestNice_]); 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> utilization = LinuxParser::CpuUtilization();
  return stol(utilization[kIdle_]) + stol(utilization[kIOwait_]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string cpu;
  string value;
  vector<string> utilization = {}; 
  std::ifstream inFile(kProcDirectory + kStatFilename);
  string line;
  if (inFile.is_open()) {
    std::getline(inFile, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while (linestream >> value) {
      utilization.emplace_back(value);
    }
  }

  return utilization;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key;
  int value;
  string line;
  std::ifstream inFile(kProcDirectory + kStatFilename);
  if (inFile.is_open()) {
    while (std::getline(inFile, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key;
  int value;
  string line;
  std::ifstream inFile(kProcDirectory + kStatFilename);
  if (inFile.is_open()) {
    while (std::getline(inFile, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }