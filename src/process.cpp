#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process() : pid(0), user("none"), command("none") {}

Process::Process(int pid_, const Processor* cpu_)
    : cpu(cpu_),
      pid(pid_),
      user(LinuxParser::User(pid_)),
      command(LinuxParser::Command(pid_)) {}

// Return this process's ID
int Process::Pid() const { return pid; }

// Return the user (name) that generated this process
string Process::User() const { return user; }

// Return the command that generated this process
string Process::Command() const { return command; }

// Return this process's memory utilization
string Process::Ram() {
  auto ramstring = LinuxParser::Ram(pid);
  if (ramstring == "")
    ram = 0;
  else {
    ram = std::stoi(ramstring);
    ram /= 1000;  // Ram in MByte
  }
  return std::to_string(ram);
}

int Process::RamAsInt() const { return ram; }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid); }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return utilization; }

void Process::UpdateUtilization() {
  current.cpu_total = cpu->Total();
  auto procinfo = LinuxParser::CpuUtilization(pid);
  if (procinfo.size() < 23)
    utilization = 0;
  else {
    // convert strings to integer
    long utime = std::stoi(procinfo[13]);
    long stime = std::stoi(procinfo[14]);
    current.proc_total = utime + stime;
    if (current.cpu_total - previous.cpu_total != 0)
      utilization = (current.proc_total - previous.proc_total) /
                    float(current.cpu_total - previous.cpu_total);
    else
      utilization = 0;

    previous = current;  // shift register
  }
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  // return (this->RamAsInt() < a.RamAsInt());
  return (this->CpuUtilization() < a.CpuUtilization());
}