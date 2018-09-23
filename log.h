
#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>
#include <sys/time.h>
#include <time.h>
#include <math.h>

enum log_t {
  LOG_QUIET_LVL = -1,
  LOG_TRACE_LVL = 0,
  LOG_DEBUG_LVL = 1,
  LOG_INFO_LVL = 2,
  LOG_WARNING_LVL = 3,
  LOG_ERROR_LVL = 4,
  LOG_FATAL_LVL = 5
};

class logger {
  std::ostringstream buf;
  std::mutex log_mutex;
  int level;
  static bool log_to_file;
  static int log_level;
  static std::string log_fname;
 public:
  typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
  typedef CoutType& (*StandardEndLine)(CoutType&);
  logger& operator<<(StandardEndLine manip) {
    manip(buf);  
    return *this;
  }
  template <typename T> logger& operator<< (const T &value) {
    buf << value;
    return *this;
  }
  ~logger();
  logger(int l): level(l) { }
  static void set_log_level(int l) { log_level = l; }
  static void set_log_name(std::string name) {
    log_fname = name;
    if (name.size())
      log_to_file = true;
    else
      log_to_file = false;
  }
};

#define Log(x)	logger(x)

#define DEFAULT_DEBUG_LVL       LOG_DEBUG_LVL
