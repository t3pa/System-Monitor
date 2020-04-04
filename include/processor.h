#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // returns instantaneous utilization (against previous
                        // call stats)
  unsigned int Total() const;  // returns total jiffies

 private:
  struct cpu_stats {
    // raw numbers
    unsigned int user;
    unsigned int nice;
    unsigned int system;
    unsigned int idle;
    unsigned int iowait;
    unsigned int irq;
    unsigned int softirq;
    unsigned int steal;
    // aggregate numbers
    unsigned int Idle;
    unsigned int NonIdle;
    unsigned int Total;
  };

  cpu_stats current, previous;  // utilization at current and previous timestep
};

#endif