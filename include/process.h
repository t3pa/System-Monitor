#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process();
  Process(int pid_);                       // constructor
  int Pid() const;                         // process ID accessor
  std::string User() const;                // User string accessor
  std::string Command() const;             // Command line string accessor
  float CpuUtilization();                  // Calculate CPU utilization
  std::string Ram();                       // RAM utilization string accessor
  int RamAsInt() const;                    // RAM utilization int accessor
  long int UpTime() const;                 // Uptime accessor
  bool operator<(Process const& a) const;  // Comparator for std::sort()

 private:
  int pid;
  int ram;
  std::string user;
  std::string command;
  float utilization;
  void SetUtilization();
};

#endif