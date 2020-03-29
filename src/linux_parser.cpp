#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// searches file for key string followed by delim, and returns following value
// string in same line
string LinuxParser::ParseValueFromFile(const string filepath, string& key,
                                       const char delim) {
  string line, value;
  std::ifstream filestream(filepath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      auto pos = line.find(key);
      if (pos != string::npos)  // if key was found in line
      {
        string substring = line.substr(
            pos, string::npos);  // cut out anything BEFORE the key string
        std::istringstream linestream(substring);
        std::getline(linestream, key, delim);  // extract key and delimiter
        linestream >> value;                   // extract value
        break;
      }
    }
  }
  return value;
}

// Returns the operating system "pretty name"
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Returns the Linux kernel version
string LinuxParser::Kernel() {
  string key{"version"};
  return ParseValueFromFile(kProcDirectory + kVersionFilename, key);
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key1{"MemTotal"}, key2{"MemFree"};
  auto memtotal = ParseValueFromFile(kProcDirectory + kMeminfoFilename, key1);
  auto memfree = ParseValueFromFile(kProcDirectory + kMeminfoFilename, key2);

  int memused = std::stoi(memtotal) - std::stoi(memfree);
  float utilization = memused / std::stof(memtotal);
  return utilization;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, uptime, idletime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return std::stoi(uptime);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> result = {};
  string line, column;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);  // aggregate CPU info is in the first line.
    std::istringstream linestream(line);
    while (linestream) {
      linestream >> column;
      result.push_back(column);
    }
  }
  return result;
}

vector<string> LinuxParser::CpuUtilization(int pid) {
  vector<string> result = {};
  string line, column;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);  // aggregate CPU info is in the first line.
    std::istringstream linestream(line);
    while (linestream) {
      linestream >> column;
      result.push_back(column);
    }
  }
  return result;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key{"processes"};
  return std::stoi(ParseValueFromFile(kProcDirectory + kStatFilename, key));
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key{"procs_running"};
  return std::stoi(ParseValueFromFile(kProcDirectory + kStatFilename, key));
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string key{""};
  auto result = ParseValueFromFile(
      kProcDirectory + std::to_string(pid) + kCmdlineFilename, key);
  return key;  // because this is just line of information, all is parsed into
               // the key string.
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  // Linux stores data about individual processes in files within subdirectories
  // of /proc
  string key{"VmSize"};
  return ParseValueFromFile(
      kProcDirectory + std::to_string(pid) + kStatusFilename, key);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key{"Uid"};
  return ParseValueFromFile(
      kProcDirectory + std::to_string(pid) + kStatusFilename, key, '\t');
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  auto uid = Uid(pid);
  string line, username;

  // look-up the User name
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      auto found = line.find(uid);
      if (found != string::npos) {
        std::istringstream linestream(line);
        std::getline(linestream, username,
                     ':');  // only get the string before ":"
        break;
      }
    }
  }
  return username;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long uptime = 0;
  vector<string> result = {};
  string line, column;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);  // aggregate CPU info is in the first line.
    std::istringstream linestream(line);
    while (linestream) {
      linestream >> column;
      result.push_back(column);
    }
    uptime = std::stol(result[21]) / sysconf(_SC_CLK_TCK);
  } else
    uptime = 0;
  return uptime;
}