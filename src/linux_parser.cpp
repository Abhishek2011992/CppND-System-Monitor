#include "linux_parser.h"

#include <bits/stdc++.h>
#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

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
  string os, kernel;
  string line;
  string version;
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

float LinuxParser::MemoryUtilization() {
  string string_val;
  string line;
  float mem_total_val;
  float mem_free_val;
  bool total_mem_found = false;
  bool free_mem_found = false;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> string_val;
      if (string_val == "MemTotal:") {
        total_mem_found = true;
        linestream >> mem_total_val;
      } else if (string_val == "MemFree:") {
        free_mem_found = true;
        linestream >> mem_free_val;
      }
      if (total_mem_found == true && free_mem_found == true) {
        break;
      }
    }
  }
  return (mem_total_val - mem_free_val) / mem_total_val;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long system_uptime;
  string line;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> system_uptime;
  }

  return system_uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string string_cmp;
  string line;
  vector<string> cpu_data;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> string_cmp;
      if (string_cmp == "cpu") {
        while (linestream) {
          string data;
          linestream >> data;
          cpu_data.push_back(data);
        }
        break;
      }
    }
  }
  return cpu_data;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string process_cmp;
  string line;
  int total_process = 0;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> process_cmp;
      if (process_cmp == "processes") {
        linestream >> total_process;
        break;
      }
    }
  }
  return total_process;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string process_cmp;
  string line;
  int running_process = 0;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> process_cmp;
      if (process_cmp == "procs_running") {
        linestream >> running_process;
        break;
      }
    }
  }
  return running_process;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command_line;
  string line;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> command_line;
    }
  }

  return command_line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string cmp;
  string line;
  string ram_usuage = "0";

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> cmp;
      if (cmp == "VmSize:") {
        linestream >> ram_usuage;
        break;
      }
    }
  }

  ram_usuage = std::to_string(std::stol(ram_usuage) / 1000);
  return ram_usuage;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string pid_cmp;
  string line;
  string uid;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> pid_cmp;
      if (pid_cmp == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
}
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line;
  string user;

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      // std::cout << line << std::endl;
      std::string intermediate;
      std::vector<std::string> tokens;
      while (std::getline(linestream, intermediate, ':')) {
        tokens.push_back(intermediate);
      }
      // std::cout << tokens[2] << std::endl;
      if (tokens[2] == uid) {
        user = tokens[0];
        break;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  vector<string> tokens;
  long int uptime = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      std::string intermediate;
      while (std::getline(linestream, intermediate, ' ')) {
        if (!intermediate.empty()) {
          tokens.push_back(intermediate);
        } else {
          tokens.push_back("0");
        }
      }
      uptime = stol(tokens[21]);
    }
  }

  return uptime / sysconf(_SC_CLK_TCK);
}

float LinuxParser::CpuUtilization(int pid) {
  string string_cmp;
  string line;
  vector<string> tokens;
  float cpu_usage = 0.0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      std::string intermediate;
      while (std::getline(linestream, intermediate, ' ')) {
        if (!intermediate.empty()) {
          tokens.push_back(intermediate);
        } else {
          tokens.push_back("0");
        }
      }
    }

    long int utime = std::stol(tokens[13]);
    long int stime = std::stol(tokens[14]);
    long int cutime = std::stol(tokens[15]);
    long int cstime = std::stol(tokens[16]);
    long int starttime = std::stol(tokens[21]);

    long int uptime = LinuxParser::UpTime();

    float Hertz = (float)sysconf(_SC_CLK_TCK);

    long int total_time = utime + stime + cutime + cstime;

    long int seconds = uptime - (starttime / Hertz);

    cpu_usage = ((total_time / Hertz) / seconds);
  }
  return cpu_usage;
}