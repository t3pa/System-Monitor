#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "processor.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process();
  Process(int pid_, const Processor* cpu_);  // constructor
  int Pid() const;                           // process ID accessor
  std::string User() const;                  // User string accessor
  std::string Command() const;               // Command line string accessor
  float CpuUtilization() const;              // CPU utilization accessor
  void UpdateUtilization();                  // Calculate CPU utilization
  std::string Ram();                         // RAM utilization string accessor
  int RamAsInt() const;                      // RAM utilization int accessor
  long int UpTime() const;                   // Uptime accessor
  bool operator<(Process const& a) const;    // Comparator for std::sort()

 private:
  const Processor* cpu;  // pointer to processor for getting CPU stats
  int pid;
  int ram;
  std::string user;
  std::string command;
  struct proc_stats {
    unsigned int cpu_total = 0;   // CPU total jiffies
    unsigned int proc_total = 0;  // Process total jiffies
  };
  proc_stats current, previous;
  float utilization;
};

#endif