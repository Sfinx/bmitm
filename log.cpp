
#include "log.h"

inline std::string time_str()
{
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

bool log_to_file = false;
int log_level = DEFAULT_DEBUG_LVL;
std::string log_fname;

void set_log_level(int l) { log_level = l; }

const char *level2str(int l)
{
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
   default:
     return "unknown";
 }
}

logger::~logger()
{
 std::lock_guard<std::mutex> guard(log_mutex);
 if (level < log_level)
   return;
 std::stringstream m;
 m << "[" + time_str() + "] [" << level2str(level) << "] " << buf.str();
 if (log_to_file) {
   std::ofstream flog;
   flog.open(log_fname, std::ios::app);
   flog << m.str() << std::endl;
   flog.close();
 } else {
   std::cerr << m.str() << std::endl;
   std::cerr.flush();
 }
}
