
#include "log.h"

bool logger::log_to_file = false;
log_level_t logger::log_level = DEFAULT_DEBUG_LVL;
std::string logger::log_fname;

logger::~logger()
{
 if (level < log_level)
   return;
 std::lock_guard<std::mutex> guard(log_mutex);
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
