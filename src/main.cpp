#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string
#include "CServerSock.h"



std::string version("3.0.3");

std::string return_current_time_and_date()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}


int main(int argc, char *argv[])
{
   printf("TCPDataServer %s is starting \n" ,version.c_str());

   DataProvider *pData  = new DataProvider();
   std::this_thread::sleep_for(std::chrono::seconds(5000));
   CServerSock *pServer = new CServerSock(pData);

   std::fstream my_file;
   my_file.open("/var/log/TCPDataServer.log", std::ios::out);
   if (!my_file)
   {
   		printf( "File not created!\n");
   }
   	else
   	{
   		my_file << "Server reboot at:: ";
   		my_file << return_current_time_and_date();
   		my_file << "----------------------\n";
   		my_file.close();
   	}

   while(true)
   {
	   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   }
}
