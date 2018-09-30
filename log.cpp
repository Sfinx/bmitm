
#include "log.h"

bool logger::log_to_file = false;
log_level_t logger::log_level_ = DEFAULT_DEBUG_LVL;
std::string logger::log_fname;

logger::~logger()
{
 if ((log_level_ != LOG_ALWAYS_LVL) && (level < log_level_))
   return;
 std::lock_guard<std::mutex> guard(log_mutex);
 std::stringstream m;
 m << "[" + time_str() + "] [" << level2str(level) << "] " << buf.str() << std::endl;
 if (log_to_file) {
   std::ofstream flog;
   flog.open(log_fname, std::ios::app);
   flog << m.str();
   flog.close();
 } else
     std::clog << m.str();
}
