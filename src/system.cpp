#include "system.h"

#include <unistd.h>

#include <algorithm>
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

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  process_list.clear();
  vector<int> pids = LinuxParser::Pids();
  for (auto i : pids) {
    if (processes.find(i) == processes.end()) {
      // Process not in map
      Process p(i, &cpu_);
      p.UpdateUtilization();
      processes[i] = p;  // add to map
      process_list.emplace_back(p);
    } else {
      // Process already existing
      Process p = processes[i];
      p.UpdateUtilization();
      process_list.emplace_back(p);
    }
  }
  std::sort(process_list.rbegin(), process_list.rend());
  return process_list;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
