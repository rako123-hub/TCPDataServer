/*
 * CConnectSock.cpp
 *
 *  Created on: 29.02.2024
 *      Author: rako
 */

#include "CConnectSock.h"

// Statische Datenelemente initialisieren:
//int	CConnectSock::m_Instance = 0;

CConnectSock::CConnectSock()
{
	// TODO Auto-generated constructor stub
}

CConnectSock::CConnectSock(int socket, DataProvider *pData) : CSock(socket)
{
//	m_Number = ++m_Instance;
	printf("socket/First Constructor of CConnectSock !\n");
//	printf("%i Instance of CConnectSock, Sock: %i !\n",m_Number,socket);

	pDataProvider = pData;
    if (m_Thread == nullptr )
    {
    	m_Thread = new std::thread(&CConnectSock::doRequestStub, this);
    }
}

CConnectSock::~CConnectSock() {
	// TODO Auto-generated destructor stub
	printf("default destructor of CConnectSock\n");

	if ( m_Thread != nullptr)
	{
		if (m_Thread->joinable())
		{
			m_Thread->join();
		}
		delete m_Thread;
	}
}
/*
CConnectSock::CConnectSock(const CConnectSock &other)
{
	// TODO Auto-generated constructor stub

}
*/

void CConnectSock::doRequestStub()
{
	char text[BUFFSIZE];
	while(true)
	{
		m_Running = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
		int bytesRecv = doReceive(m_Buffer, BUFFSIZE);
		if (bytesRecv > 0)
		{
			m_Buffer[bytesRecv] = '\0';
			doParse();
		}
		else
		{
			m_Running = false;
			break;
		}
	}

	//delete this;
}

void CConnectSock::doParse()
{
	std::string str_snd = "Hallo";
	printf(">>>>>%s<<<<<<< \n", m_Buffer);

	std::string strRequest(reinterpret_cast<char*>(m_Buffer));
	if (strRequest == "get 1.8.0")
	{
	   str_snd = pDataProvider->getEngeryToPowerSupplySys();
	}
	else if (strRequest == "get 2.8.0")
	{
	   str_snd = pDataProvider->getEngeryfromPowerSupplySys();
	}
	else if (strRequest == "get 16.7.0")
	{
	   str_snd = pDataProvider->getactualEnergy();
	}
	printf("#send *%s*\n", str_snd.c_str());

	strcpy( (char*)m_Buffer, str_snd.c_str());
	m_Buffer[str_snd.length()] = '\0';
	int len = strlen((char*)m_Buffer);

	doSend(m_Buffer, len);
}

void CConnectSock::destroy()
{
	delete this;
}

