#include "format.h"

#include <memory>
#include <stdexcept>
#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  unsigned int hours = 0, minutes = 0;
  if (seconds >= (60 * 60)) {
    hours = seconds / (60 * 60);
    seconds -= hours * 60 * 60;

    minutes = seconds / 60;
    seconds -= minutes * 60;
  } else if (seconds >= 60) {
    minutes = seconds / 60;
    seconds -= minutes * 60;
  }
  string format = "%02d:%02d:%02d";
  size_t size = snprintf(nullptr, 0, format.c_str(), hours, minutes, seconds) +
                1;  // Extra space for '\0'
  if (size <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), hours, minutes, seconds);
  return std::string(buf.get(), buf.get() + size - 1);
}