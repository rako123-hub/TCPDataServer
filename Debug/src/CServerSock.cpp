/*
 * CServerSock.cpp
 *
 *  Created on: 29.02.2024
 *      Author: rako
 */

#include "CServerSock.h"

CServerSock::CServerSock():CSock()
{
	// TODO Auto-generated constructor stub
	printf("Constructor CServerSock\n");
	init();

}

CServerSock::CServerSock(DataProvider *pData):CSock()
{
	// TODO Auto-generated constructor stub
	pDataProvider = pData;
	printf("Constructor CServerSock\n");
	init();

}

CServerSock::~CServerSock()
{
	// TODO Auto-generated destructor stub
	printf("Destructor CServerSock\n");

	if ( m_acceptThread != nullptr )
	{
		if ( m_acceptThread->joinable() )
		{
			m_acceptThread->join();
		}
		delete m_acceptThread;
	}

	if ( m_connectThread != nullptr )
	{
		if ( m_connectThread->joinable() )
		{
			m_connectThread->join();
		}
		delete m_connectThread;
	}

}

CServerSock::CServerSock(const CServerSock &other)
{
	// TODO Auto-generated constructor stub

}

void CServerSock::init()
{
   if (!create(TCP_PORT)) return;
   if (!doListen())       return;

   // Create Thread for accept Clients
	if (m_acceptThread == nullptr )
	{
		m_acceptThread = new std::thread(&CServerSock::doAcceptStub, this);
	}

	if (m_connectThread == nullptr )
	{
		m_connectThread = new std::thread(&CServerSock::doCreateConnect, this);
	}
}

void CServerSock::doAcceptStub()
{
	if (m_Socket == INVALID_SOCKET) return;
	while(true)		// kommt nie zurück
	{
		socklen_t clen;
		struct sockaddr_in caddr;
		struct sockaddr_in ipAddr;

		if( m_Count < 255 )
		{
			clen = sizeof(caddr);
			m_Sock[m_Write++] = accept(m_Socket, (struct sockaddr *) &caddr, &clen);
			m_Count++;
			printf("Accept, count: %i !\n", m_Count);
//			if(m_Count>255) m_Count=255;
	//		DEBUGMSG(ZONE_SSOCK, (TEXT("Accept: wr: %i, cnt: %i\r\n"),	m_Write++,m_Count));
			m_Write %= 256;

			if (getpeername(m_Sock[m_Read], (struct sockaddr *)&ipAddr, &clen) < 0)
			{
				printf("getpeername failed\n");
				printf("Error Number :%s\n", strerror(errno));
			}
			printf("Client socket's ip : %s\n", inet_ntoa(ipAddr.sin_addr));
			printf("peer is: %s:%hu\n", inet_ntoa(ipAddr.sin_addr), ntohs(ipAddr.sin_port));
		}
	}
}

void CServerSock::doCreateConnect()
{
	int	socket = INVALID_SOCKET;
	while(true)		// kommt nie zurück
	{
//		RETAILMSG(1, (TEXT(".%i"),m_Count));
		if( m_Count > 0 )
		{
//			DEBUGMSG(ZONE_SSOCK, (TEXT("Loop: rd: %i, cnt: %i, Sock: %i\r\n"),	m_Read,m_Count,m_Sock[m_Read]));
//			RETAILMSG(1, (TEXT("Loop: rd: %i, cnt: %i, Sock: %i\r\n"),	m_Read,m_Count,m_Sock[m_Read]));
			m_Count--;
			socket = m_Sock[m_Read];
			m_Sock[m_Read++]=INVALID_SOCKET;
			m_Read %= 256;
			if (socket != INVALID_SOCKET)
			{
//				std::shared_ptr<CConnectSock> pConnSock(new CConnectSock(socket, pDataProvider));
				CConnectSock *sock = new CConnectSock(socket, pDataProvider);		// erzeugt CConnectSock Objecte statt CSock Objecte
				sock->destroy();
				sock = nullptr;
//				CConnectSock *sock = new CConnectSock(socket);		// erzeugt CConnectSock Objecte statt CSock Objecte
//				m_Service->GetNewClient(socket);
//				RETAILMSG(1, (TEXT("New Socket !\r\n")));
			}
//			else
//			DEBUGMSG(ZONE_SSOCK, (TEXT("Invalid Socket: %i\r\n"),	socket));
		}
		else    std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

}

