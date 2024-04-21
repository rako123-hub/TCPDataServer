/*
 * DataProvider.h
 *
 *  Created on: 18.02.2024
 *      Author: rako
 */

#ifndef SRC_DATAPROVIDER_H_
#define SRC_DATAPROVIDER_H_

#define MC_SML_BUFFER_LEN 8096

#include <thread>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <mutex>

#include <sys/ioctl.h>

#include <sml/sml_file.h>
#include <sml/sml_transport.h>
#include <sml/sml_value.h>
//#include "unit.h"


class DataProvider
{
public:
	DataProvider();
	virtual ~DataProvider();

private:
	void init();
	void createRcvThreadFromSml();
	void createSndThreadToServer();
	bool openSmlDevice();
	void getDataFromSmlDevice();


	std::string energyGet;
	std::string energySet;
	std::string actualEnergy;
	int fileHndSerial = -1;

	std::mutex m;

public:
	void setEngeryfromPowerSupplySys(std::string value)
	{
		std::lock_guard<std::mutex> lockFromPower(m);
		energyGet    = value;
	}
	void setEngeryToPowerSupplySys(std::string value)
	{
		std::lock_guard<std::mutex> lockToPower(m);
		energySet    = value;
	}
	void setactualEnergy(std::string value)
	{
		std::lock_guard<std::mutex> lockactualEnergy(m);
		actualEnergy = value;
	};

	std::string getEngeryfromPowerSupplySys()
	{
		std::lock_guard<std::mutex> lockFromPower(m);
		return energyGet;
	}
	std::string getEngeryToPowerSupplySys()
	{
		std::lock_guard<std::mutex> lockToPower(m);
		return energySet;
	}
	std::string getactualEnergy()
	{
		std::lock_guard<std::mutex> lockactualEnergy(m);
		return actualEnergy;
	}

	std::thread *rcvthread = nullptr;
	std::thread *sndthread = nullptr;

	void doRunReceive();
	void doRunSend();
	void transport_receiver(unsigned char *buffer, size_t buffer_len);

	bool enablePrintF = false;

};

#endif /* SRC_DATAPROVIDER_H_ */
