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

Process::Process(int pid_)
    : pid(pid_),
      user(LinuxParser::User(pid_)),
      command(LinuxParser::Command(pid_)) {
  SetUtilization();
  Ram();
}

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
float Process::CpuUtilization() {
  SetUtilization();
  return utilization;
}

void Process::SetUtilization() {
  long Uptime = LinuxParser::UpTime();  // get system uptime
  auto cpuinfo = LinuxParser::CpuUtilization(pid);
  if (cpuinfo.size() < 23)
    utilization = 0;
  else {
    // convert strings to integer
    long utime = std::stoi(cpuinfo[13]);
    long stime = std::stoi(cpuinfo[14]);
    long starttime = std::stoi(cpuinfo[21]);

    // calculate aggegrate stats
    long Hertz = sysconf(_SC_CLK_TCK);
    starttime /= Hertz;
    long Total = utime + stime;
    Total /= Hertz;
    long Seconds = Uptime - starttime;
    if (Seconds != 0)
      utilization = 100.0 * (Total / Seconds);
    else
      utilization = 0;
  }
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return (this->RamAsInt() < a.RamAsInt());
}