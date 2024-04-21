/*
 * DataProvider.cpp
 *
 *  Created on: 18.02.2024
 *      Author: rako
 */

#include "DataProvider.h"

DataProvider::DataProvider()
{
	// TODO Auto-generated constructor stub
   init();
}

DataProvider::~DataProvider()
{
	// TODO Auto-generated destructor stub
	printf("default Destructor of DataProvider\n");
	/*
	if (rcvthread != nullptr)
	{
		if (rcvthread->joinable())
		{
			rcvthread->join();
		}
		delete rcvthread;
	}
	*/
	if (rcvthread.joinable())
	{
		rcvthread.join();
	}
}

void DataProvider::init()
{
	for (int x = 0 ; x < 2; x++)
	{
		if (openSmlDevice())
		{
			printf("Open SML Device OK!!!\n");
			createRcvThreadFromSml();
			return;
		}
		else
		{
			printf("Failure:: Can't open SmlDevice --> try again\n");
		}
		std::this_thread::sleep_for(std::chrono::seconds(60));
	}
}

void DataProvider::createRcvThreadFromSml()
{
	/*
	if(rcvthread == nullptr)
	{
		rcvthread = new std::thread(&DataProvider::doRunReceive, this);
	}
	*/
	rcvthread = std::thread(&DataProvider::doRunReceive, this);
}

bool DataProvider::openSmlDevice()
{
	bool result = true;
	int bits;
	struct termios config;
	std::string dev = "/dev/ttyUSB0";
	memset(&config, 0, sizeof(config));

	#ifdef O_NONBLOCK
		fileHndSerial = open(dev.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	#else
		fileHndSerial = open(dev.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	#endif
	if (fileHndSerial < 0)
	{
		result = false;
		fprintf(stderr, "error: open(%s): %s\n", dev.c_str(), strerror(errno));
		return result;
	}
	// set RTS

	ioctl(fileHndSerial, TIOCMGET, &bits);
	bits |= TIOCM_RTS;
	ioctl(fileHndSerial, TIOCMSET, &bits);

	tcgetattr(fileHndSerial, &config);

	// set 8-N-1
	config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR
			| ICRNL | IXON);
	config.c_oflag &= ~OPOST;
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	config.c_cflag &= ~(CSIZE | PARENB | PARODD | CSTOPB);
	config.c_cflag |= CS8;

	// set speed to 9600 baud
	cfsetispeed(&config, B9600);
	cfsetospeed(&config, B9600);

	tcsetattr(fileHndSerial, TCSANOW, &config);

	return result;
}


void DataProvider::doRunReceive()
{

	unsigned char buffer[MC_SML_BUFFER_LEN];
	size_t bytes;

	while ((bytes = sml_transport_read(fileHndSerial, buffer, MC_SML_BUFFER_LEN)) > 0)
	{
			transport_receiver(buffer, bytes);
	}
}

void DataProvider::transport_receiver(unsigned char *buffer, size_t buffer_len)
{
	//bool vflag = false;
	int i;
	char obis_buffer[25];
	char obis_value[25];
	// the buffer contains the whole message, with transport escape sequences.
	// these escape sequences are stripped here.
	sml_file *file = sml_file_parse(buffer + 8, buffer_len - 16);
	// the sml file is parsed now

	// this prints some information about the file
	/*
	if (vflag)
		sml_file_print(file);
		*/

	// read here some values ...
	//if (vflag)
		//printf("OBIS data\n");
	for (i = 0; i < file->messages_len; i++) {
		sml_message *message = file->messages[i];
		if (*message->message_body->tag == SML_MESSAGE_GET_LIST_RESPONSE) {
			sml_list *entry;
			sml_get_list_response *body;
			body = (sml_get_list_response *) message->message_body->data;
			for (entry = body->val_list; entry != NULL; entry = entry->next) {
				if (!entry->value) { // do not crash on null value
					fprintf(stderr, "Error in data stream. entry->value should not be NULL. Skipping this.\n");
					continue;
				}
				if (entry->value->type == SML_TYPE_OCTET_STRING) {
/*					char *str;

					printf("%d-%d:%d.%d.%d*%d   ####%s#\n",
						entry->obj_name->str[0], entry->obj_name->str[1],
						entry->obj_name->str[2], entry->obj_name->str[3],
						entry->obj_name->str[4], entry->obj_name->str[5],
						sml_value_to_strhex(entry->value, &str, true));

					free(str);
*/
				} else if (entry->value->type == SML_TYPE_BOOLEAN) {

					printf("%d-%d:%d.%d.%d*%d >>>>>> ###%s#\n",
						entry->obj_name->str[0], entry->obj_name->str[1],
						entry->obj_name->str[2], entry->obj_name->str[3],
						entry->obj_name->str[4], entry->obj_name->str[5],
						entry->value->data.boolean ? "true" : "false");

				} else if (((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_INTEGER) ||
						((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_UNSIGNED)) {
					double value = sml_value_to_double(entry->value);
					int scaler = (entry->scaler) ? *entry->scaler : 0;
					int prec = -scaler;
					if (prec < 0)
						prec = 0;
					value = value * pow(10, scaler);
					    sprintf(obis_buffer, "%d.%d.%d", entry->obj_name->str[2],
					    		entry->obj_name->str[3], entry->obj_name->str[4]);
					    std::string strObis(obis_buffer);

					    if ((strObis == "1.8.0") || (strObis == "2.8.0")) value = value / 1000.0;
					    sprintf(obis_value, "%f\n", value);
					    std::string strObisValue(obis_value);

					    if (strObis == "1.8.0")
					    {
					    	if (enablePrintF)
					    	{
     					    	printf("Obis Kennzahl %s    value %s", strObis.c_str(), strObisValue.c_str());
					    	}
					    	setEngeryToPowerSupplySys(strObisValue);
					    }
					    else if (strObis == "2.8.0")
					    {
					    	if (enablePrintF)
					    	{
					    	    printf("Obis Kennzahl %s    value %s", strObis.c_str(), strObisValue.c_str());
					    	}
					    	setEngeryfromPowerSupplySys(strObisValue);
					    }
					    else if (strObis == "16.7.0")
					    {
					    	if (enablePrintF)
					    	{
					    	   printf("Obis Kennzahl %s    value %s", strObis.c_str(), strObisValue.c_str());
					    	}
					    	setactualEnergy(strObisValue);
					    }
					    else
					    {
					         //do nothing
					    }
					// flush the stdout puffer, that pipes work without waiting
					fflush(stdout);
				}
			}
		}
	}

	// free the malloc'd memory
	sml_file_free(file);
}



