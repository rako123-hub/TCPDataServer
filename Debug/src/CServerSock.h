/*
 * CServerSock.h
 *
 *  Created on: 29.02.2024
 *      Author: rako
 */

#ifndef SRC_CSERVERSOCK_H_
#define SRC_CSERVERSOCK_H_

#include "CSock.h"
#include "CConnectSock.h"
#include "DataProvider.h"

class CServerSock: public CSock
{
public:
	CServerSock();
	CServerSock(DataProvider *pDataProvider);
	virtual ~CServerSock();
	CServerSock(const CServerSock &other);

protected:
   int	 m_Sock[256] = {-1};
   DWORD m_Read      =   0;
   DWORD m_Write     =   0;
   int	 m_Count     =   0;
private:
    DataProvider *pDataProvider = nullptr;
	void init();
public:
	std::thread *m_acceptThread = nullptr;
	std::thread *m_connectThread = nullptr;
	void doAcceptStub();
	void doCreateConnect();
};

#endif /* SRC_CSERVERSOCK_H_ */
