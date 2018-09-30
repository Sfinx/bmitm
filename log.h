
#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>
#include <sys/time.h>
#include <time.h>
#include <math.h>

enum log_level_t {
  LOG_TRACE_LVL,
  LOG_DEBUG_LVL,
  LOG_INFO_LVL,
  LOG_WARNING_LVL,
  LOG_ERROR_LVL,
  LOG_FATAL_LVL,
  LOG_QUIET_LVL,
  LOG_ALWAYS_LVL
};

#define DEFAULT_DEBUG_LVL       LOG_INFO_LVL

class logger {
  std::ostringstream buf;
  std::mutex log_mutex;
  log_level_t level; // current level
  static bool log_to_file;
  static log_level_t log_level_; // global level
  static std::string log_fname;
  static const char *level2str(log_level_t l) {
    switch (l) {
      case LOG_QUIET_LVL:
        return "quiet";
      case LOG_TRACE_LVL:
        return "trace";
      case LOG_DEBUG_LVL:
        return "debug";
      case LOG_INFO_LVL:
        return "info";
      case LOG_WARNING_LVL:
        return "warning";
      case LOG_ERROR_LVL:
        return "error";
      case LOG_FATAL_LVL:
        return "fatal";
      case LOG_ALWAYS_LVL:
        return "always";
      default:
        return "unknown";
    }
  }
  inline std::string time_str() {
    char b[64];
    struct timeval tv;
    gettimeofday(&tv, 0);
    uint milliseconds = lrint(tv.tv_usec / 1000.0);
    if (milliseconds >= 1000) {
      milliseconds -= 1000;
      tv.tv_sec++;
    }
    struct tm *tm = localtime(&tv.tv_sec);
    strftime(b, 64, "%a %b %d %H:%M:%S", tm);
    char m[16];
    sprintf(m, ".%03d", milliseconds);
    return std::string(b) + std::string(m);
  }
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
  logger(log_level_t l): level(l) { }
  static const char *log_level() { return level2str(log_level_); }
  static void set_log_level(log_level_t l) { log_level_ = l; }
  static void set_log_level(std::string l) {
    log_level_t ll = DEFAULT_DEBUG_LVL;
    if (l == "quiet")
      ll = LOG_QUIET_LVL;
    if (l == "trace")
      ll = LOG_TRACE_LVL;
    if (l == "debug")
      ll = LOG_DEBUG_LVL;
    if (l == "info")
      ll = LOG_INFO_LVL;
    if (l == "warning")
      ll = LOG_WARNING_LVL;
    if (l == "error")
      ll = LOG_ERROR_LVL;
    if (l == "fatal")
      ll = LOG_FATAL_LVL;
    if ((l == "always") || (l == "all"))
      ll = LOG_ALWAYS_LVL;
    set_log_level(ll);
  }
  static void set_log_name(std::string name) {
    log_fname = name;
    if (name.size())
      log_to_file = true;
    else
      log_to_file = false;
  }
};

#define Log(x)	logger(x)
