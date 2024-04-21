#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include "CServerSock.h"
#include "CReboot.h"



std::string version("1.1.0");



int main(int argc, char *argv[])
{
   printf("TCPDataServer %s is starting \n" ,version.c_str());


   DataProvider *pData  = new DataProvider();

   CServerSock *pServer = new CServerSock(pData);

   CReboot *preboot     = new CReboot();



//   std::shared_ptr<CServerSock> pServer(new CServerSock(pData));


   /*
   std::shared_ptr<CServerSock> p2(new CServerSock());
   std::shared_ptr<CServerSock> p3 = std::make_shared<CServerSock>();
   //the same
   std::shared_ptr<CServerSock> p4(new CServerSock());
   */

   while(true)
   {
	   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   }
}


/*
#include <unistd.h>
#include <sys/reboot.h>

int main () {
  sync();
  setuid(0);
  reboot(RB_AUTOBOOT);
  return(0);
}
*/
