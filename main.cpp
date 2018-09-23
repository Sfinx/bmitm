
#include "mitm.h"

using namespace std;

bool app_tx_cb(uint cid, string &d)
{
 Log(LOG_INFO_LVL) << "conn N" << cid << " app_tx: sz: " << d.size();
 return true;
}

bool app_rx_cb(uint cid, string &d)
{
 Log(LOG_INFO_LVL) << "conn N" << cid << " app_rx: sz: " << d.size();
 return true;
}

int main(int argc, char **argv)
{
 if (argc < 2) {
   cerr << "Usage: " << argv[0] << " <SSLproxy port> [log_fname]";
   ::exit(0);
 }
 if (argc > 2)
   logger::set_log_name(argv[2]);
 Log(LOG_INFO_LVL) << argv[0] << " started";
 try {
   mitm_t tl(atoi(argv[1]), app_tx_cb, app_rx_cb);
   tl.start();
   while(1)
     sleep(1);
 } catch (ost::Socket* s) {
   int err = s->getErrorNumber() ? s->getErrorNumber() : s->getSystemError();
   cerr << "main: Socket exception: " << strerror(err) << endl;
   return -1;
 } catch (...) {
   cerr << "main: general exception" << endl;
   return -1;
 }
 return 0;
}
