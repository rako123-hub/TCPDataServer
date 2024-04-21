/*
 * CConnectSock.h
 *
 *  Created on: 29.02.2024
 *      Author: rako
 */

#ifndef SRC_CCONNECTSOCK_H_
#define SRC_CCONNECTSOCK_H_

#include "CSock.h"
#include "DataProvider.h"

class CConnectSock: public CSock
{
public:
	//static int m_Instance;

	CConnectSock();
	CConnectSock(int socket, DataProvider *pData);	// Constructor called from CServerSock
	virtual ~CConnectSock();

protected:
   std::thread *m_Thread = nullptr;	    // Thread-Handle of the Request Thread
private:
//   int			m_Number;				// Instance Counter of this Object
   bool         m_Running = false;
   DataProvider *pDataProvider = nullptr;
//	Variables to parse and respond a Request:
   BYTE         m_Buffer[BUFFSIZE+1];	// Inputbuffer
   void         doParse();

public:
   void doRequestStub();
   void destroy();
   bool getRunningMode() {return m_Running;}
};

#endif /* SRC_CCONNECTSOCK_H_ */
