#include "processor.h"

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  auto cpuinfo = LinuxParser::CpuUtilization();
  if (cpuinfo.size() < 9)
    return 0.0;
  else {
    // convert strings to integer
    current.user = std::stoi(cpuinfo[1]);
    current.nice = std::stoi(cpuinfo[2]);
    current.system = std::stoi(cpuinfo[3]);
    current.idle = std::stoi(cpuinfo[4]);
    current.iowait = std::stoi(cpuinfo[5]);
    current.irq = std::stoi(cpuinfo[6]);
    current.softirq = std::stoi(cpuinfo[7]);
    current.steal = std::stoi(cpuinfo[8]);

    // calculate aggegrate stats
    current.Idle = current.idle + current.iowait;
    current.NonIdle = current.user + current.nice + current.system +
                      current.irq + current.softirq + current.steal;
    current.Total = current.Idle + current.NonIdle;

    // delta against previous stats
    float deltaTotal = current.Total - previous.Total;
    float deltaIdle = current.Idle - previous.Idle;

    float utilization = (deltaTotal - deltaIdle) / deltaTotal;
    previous = current;  // shift register
    return utilization;
  }
}