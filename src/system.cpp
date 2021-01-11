#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pid = LinuxParser::Pids();
  Process temp_buffer;
  processes_.clear();
  for(unsigned int i=0; i<pid.size();i++){
      int id = pid[i];
      temp_buffer.Pid(id);

      std::string user= LinuxParser::User(id);
      temp_buffer.User(user);

      float cpu_usage = LinuxParser::CpuUtilization(id);
      temp_buffer.CpuUtilization(cpu_usage);

      long int uptime= LinuxParser::UpTime(id);
      temp_buffer.UpTime(uptime);

      std::string ram= LinuxParser::Ram(id);
      temp_buffer.Ram(ram);

      std::string command= LinuxParser::Command(id);
      temp_buffer.Command(command);

      processes_.push_back(temp_buffer);
  }
  
  std::sort(processes_.begin(),processes_.end(),[](Process a,Process b){ return b<a;});

  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }