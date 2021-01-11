#include "processor.h"

#include "linux_parser.h"


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> current_cpu_data = LinuxParser::CpuUtilization();
  float CPU_Percentage;
  if (!old_cpu_data_.empty()) {
    long PrevIdle = std::stol(old_cpu_data_[LinuxParser::kIdle_]) +
                    std::stol(old_cpu_data_[LinuxParser::kIOwait_]);
    long Idle = std::stol(current_cpu_data[LinuxParser::kIdle_]) +
                std::stol(current_cpu_data[LinuxParser::kIOwait_]);

    long PrevNonIdle = std::stol(old_cpu_data_[LinuxParser::kUser_]) +
                       std::stol(old_cpu_data_[LinuxParser::kNice_]) +
                       std::stol(old_cpu_data_[LinuxParser::kIRQ_]) +
                       std::stol(old_cpu_data_[LinuxParser::kSoftIRQ_]) +
                       std::stol(old_cpu_data_[LinuxParser::kSteal_]) +
                       std::stol(old_cpu_data_[LinuxParser::kGuest_]) +
                       std::stol(old_cpu_data_[LinuxParser::kGuestNice_]) ;

    long NonIdle = std::stol(current_cpu_data[LinuxParser::kUser_]) +
                       std::stol(current_cpu_data[LinuxParser::kNice_]) +
                       std::stol(current_cpu_data[LinuxParser::kIRQ_]) +
                       std::stol(current_cpu_data[LinuxParser::kSoftIRQ_]) +
                       std::stol(current_cpu_data[LinuxParser::kSteal_]) +
                       std::stol(current_cpu_data[LinuxParser::kGuest_]) +
                       std::stol(current_cpu_data[LinuxParser::kGuestNice_]) ;


    long PrevTotal = PrevIdle + PrevNonIdle;
    long Total = Idle + NonIdle;

    //differentiate: actual value minus the previous one
    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;

    CPU_Percentage = (totald - idled)/totald;

  } else {
    old_cpu_data_ = current_cpu_data;
    CPU_Percentage = 0;
  }

  return CPU_Percentage;
}