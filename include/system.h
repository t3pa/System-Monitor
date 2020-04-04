#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <unordered_map>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   // Getter for CPU member
  std::vector<Process>& Processes();  // Getter for processes member
  float MemoryUtilization();          // Memory utilization accessor
  long UpTime();                      // Uptime accessor
  int TotalProcesses();               // Number of processes accessor
  int RunningProcesses();             // Running processes accessor
  std::string Kernel();               // Kernel version accessor
  std::string OperatingSystem();      // Operating system accessor

 private:
  Processor cpu_ = {};                         // aggregate CPU statistics
  std::unordered_map<int, Process> processes;  // running processes hash table
  std::vector<Process> process_list = {};  // running processes (output list)
};

#endif