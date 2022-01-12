#include "stdafx.h"

#include "ExternalIcd.h"

#include "CGRCommObj.h"
#include "../ICAAMngr.h"
//#include "../ASAE_DEMONDlg.h"

void CALLBACK GRCommObjTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired);
CRITICAL_SECTION        cs_GRCommLinkSend;     // critical section
CRITICAL_SECTION				cs_GRCommLinkRecvQueue;
/**
* @brief CGRCommObj ������
*/
CGRCommObj::CGRCommObj() :
	m_uiRegisteredOpcodeCount(0),
	m_uiLinkFrameSize(LINK_FRAME_SIZE)
{
	InitializeCriticalSection( &cs_GRCommLinkRecvQueue);

	m_hEnqueueSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
	m_hDequeueSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	m_hClientSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

	m_iSocketMode = SOCKET_CLIENT;	
	
	m_hDataLinkServerSocket = NULL;
	m_hRadarLink1ClientSocket = NULL;
	m_hInterCommClientSocket = NULL;

	memset(m_aucSendBuf, NULL, MAX_BUF_SIZE);
	memset(m_aucTcpRecvBuf, NULL, MAX_BUF_SIZE);
	memset(m_aucUdpRecvBuf, NULL, MAX_BUF_SIZE);
	memset(m_aucCompRecvBuf, NULL, MAX_BUF_SIZE);
	memset(m_aucNFrameBuf, NULL, MAX_BUF_SIZE);

	m_bDisplayCommLog = false;
	m_hUdpSocket = NULL;
	m_iServerID = NULL;

	m_bDataLinkConnection = false;
	m_bServerConnection = false;

	memset(&m_stMissionPacketForResend, NULL, sizeof(StMissionPacket));
	m_uiMissionPacketSizeForResend = NULL;

	m_uiLinkSendSeqNo = START_LINK_SEQ_INDEX;
	m_uiLinkRecvSeqNo = NULL;
	m_uiCurrentTimerNo = NULL;
	m_hTimer = NULL;
	m_hTimerQueue = NULL;

	m_hEncEvent = NULL;


	m_iRetryCnt = NULL;

	m_uiTotalRecvMsgCnt = NULL;
	m_uiUnRecvMsgCnt = NULL;
	m_uiTotalSendMsgCnt = NULL;
	m_uiUnSendMsgCnt = NULL;

	// Heartbeat ��뿩�� Ȯ��
	LoadHeartbeatInfo();
	
	m_bPacketProcessingThreadEndFlag = false;
	m_bHeartbeatConnection_1 = false;
	m_bHeartbeatConnection_2 = false; //PDW�߻��� �������
	m_bHeartbeatConnection_3 = false;
	m_bHeartbeatConnection_4 = false;
	m_hEnqueueSemaphoreForSend = CreateSemaphore(NULL, 1, 1, NULL);
	m_hDequeueSemaphoreForSend = CreateSemaphore(NULL, 0, 1, NULL);
	StartTimer();
	
	QueryPerformanceFrequency(&liFrequency);  // retrieves the frequency of the high-resolution performance counter
	InitializeCriticalSection( &cs_GRCommLinkSend);
}

/**
* @brief CGRCommObj �Ҹ���
*/
CGRCommObj::~CGRCommObj()
{
	CloseHandle(m_hEnqueueSemaphore);
	CloseHandle(m_hDequeueSemaphore);
	CloseHandle(m_hClientSemaphore);

	CloseHandle(m_hEnqueueSemaphoreForSend);
	CloseHandle(m_hDequeueSemaphoreForSend);

	m_bPacketProcessingThreadEndFlag = true;

	if ( m_hTimerQueue != NULL)
	{
		DeleteTimerQueue(m_hTimerQueue);
	}

	DeleteCriticalSection( &cs_GRCommLinkSend);
	DeleteCriticalSection( &cs_GRCommLinkRecvQueue);
}

/**
* @brief CGRCommObj�� singleton pattern �Լ�
* @return Process���� ������ CGRCommObj�� ��ü
*/
CGRCommObj& CGRCommObj::GetInstance()
{
	static CGRCommObj cGRCommObj;
	return cGRCommObj;
}

bool CGRCommObj::StartTimer()
{
	bool bRtn = false;

	// �̹� ���������� �޽����� �����Ѵٸ�
	if ( m_uiCurrentTimerNo != NULL)
	{
		bRtn = false;
		//TRACE("TImer Start Fail!(�� �������� �޽��� ����)\n");
	}
	else
	{
		if(m_hTimerQueue == NULL)
		{	
			m_hTimerQueue = CreateTimerQueue();
			CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, (WAITORTIMERCALLBACK)GRCommObjTimer, this, RETRY_INTERVAL_TIME, RETRY_INTERVAL_TIME, WT_EXECUTEINTIMERTHREAD);			
			bRtn = true;		
			//m_uiCurrentTimerNo = i_uiLinkSendSeqNo;
			//TRACE("Ÿ�̸Ӱ� Ȱ��ȭ �Ǿ����ϴ�.\n");
		}
		else
		{
			bRtn = false;
			//TRACE("Ÿ�̸Ӱ� �̹� �����Ǿ� �ֽ��ϴ�.");
		}
	}

	return bRtn;
}

void CGRCommObj::StopTimer()
{
	if ( m_hTimerQueue == NULL )
		return;

	BOOL bRtn = DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, NULL);

	if(bRtn)
	{
		m_hTimerQueue = NULL;
		m_iRetryCnt = NULL;
		m_uiCurrentTimerNo = NULL;
	}
	else
	{
		if ( ERROR_IO_PENDING == GetLastError() || m_hTimerQueue == NULL || m_hTimer == NULL )
		{
			m_hTimerQueue = NULL;
			m_iRetryCnt = NULL;
			m_uiCurrentTimerNo = NULL;
		}
		else
		{
			while(DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, NULL) == 0)
			{
				TRACE("DeleteTimerQueue Error %d\n", GetLastError());

				if ( ERROR_IO_PENDING == GetLastError() || m_hTimerQueue == NULL || m_hTimer == NULL )
				{
					m_hTimerQueue = NULL;
					m_iRetryCnt = NULL;
					m_uiCurrentTimerNo = NULL;
					break;
				}
			}			
		}
	}

	m_uiLinkSendSeqNo++;
}

void CGRCommObj::SetDisplayCommLog(bool i_bDisplayCommLog)
{
	m_bDisplayCommLog = i_bDisplayCommLog;
}


bool CGRCommObj::GetDisplayCommLog()
{
	return m_bDisplayCommLog;
}

CCommonMngr* CGRCommObj::GetMngr(unsigned int i_uiOpcode)
{
	// TODO : �� ���̽��ϰ� ������
	for(unsigned int i=0; i<m_uiRegisteredOpcodeCount; i++)
	{
		if(m_auiOpcode[i] == i_uiOpcode)
		{
			return m_apcMngr[i];
		}
	}

	// ����� �ȵǾ��ִ� ��� NULL�� ��ȯ
	return NULL;
}

void CGRCommObj::SetDataLinkServerSocket(SOCKET i_hSocket)
{
	m_hDataLinkServerSocket = i_hSocket;
}

void CGRCommObj::SetRadarLink1ClientSocket(SOCKET i_hSocket)
{
	m_hRadarLink1ClientSocket = i_hSocket;
}

void CGRCommObj::SetInterCommClientSocket(SOCKET i_hSocket)
{
	m_hInterCommClientSocket = i_hSocket;	
}

SOCKET CGRCommObj::GetInterCommClientSocket()
{
	return m_hInterCommClientSocket;
}

SOCKET CGRCommObj::GetDataLinkServerSocket()
{
	return m_hDataLinkServerSocket;
}

SOCKET CGRCommObj::GetRadarLinkClientSocket()
{
	SOCKET hRst = NULL;

	if ( m_iServerID == SERVER_ID_LINK_1 )
	{
		hRst = m_hRadarLink1ClientSocket;
	}
	else
	{
		//hRst = m_hRadarLink2ClientSocket;
	}

	return m_hRadarLink1ClientSocket;
}

bool CGRCommObj::RegisterOpcode(unsigned int i_uiOpcode, CCommonMngr *i_pcMngr)
{
	bool bResult = true;

/*	for(unsigned int i=0; i<m_uiRegisteredOpcodeCount; i++)
	{
		// �� ��ϵ� Opcode �ߺ� �˻� => 2�� �̻��� �Ŵ������� ���ÿ� �����͸� ó���� �ʿ䰡 �ֱ� ������ �ߺ��˻� ������ �ּ� ó��
		if(m_auiOpcode[i] == i_uiOpcode)
		{
			return false;
		}
	}
*/
//	m_auiOpcode[m_uiRegisteredOpcodeCount] = i_uiOpcode;
//	m_apcMngr[m_uiRegisteredOpcodeCount] = i_pcMngr;

	m_auiOpcode.push_back( i_uiOpcode );
	m_apcMngr.push_back( i_pcMngr );

	m_uiRegisteredOpcodeCount++;

	return bResult;
}

bool CGRCommObj::UnregisterOpcode(unsigned int i_uiOpcode, CCommonMngr *i_pcMngr)
{
	bool bResult = true;
	unsigned int unCnt = 0;

	vector <CCommonMngr*>::iterator itMngr;
	vector <unsigned int>::iterator itOpcode;

	for( itMngr = m_apcMngr.begin(), itOpcode = m_auiOpcode.begin() ; itMngr != m_apcMngr.end(), itOpcode != m_auiOpcode.end() ; itMngr++, itOpcode++ )
	{
		if( i_pcMngr == (*itMngr) )
		{
			m_apcMngr.erase( itMngr );
			m_auiOpcode.erase( itOpcode );

			break;
		}
	}

	m_uiRegisteredOpcodeCount--;

	return bResult;
}

bool CGRCommObj::EnqueuePacket(void* i_pvPacket, int i_iLength)
{
	bool bRst = false;
	DWORD dRet = NULL;
	dRet = WaitForSingleObject(m_hEnqueueSemaphore, COMM_TIMEOUT);
	if (WAIT_OBJECT_0 == dRet)
	{
		StMissionPacketContainer *stPacketContainer = new StMissionPacketContainer;
		stPacketContainer->pvPacket = i_pvPacket;
		stPacketContainer->iLength = i_iLength;

		m_pstPacketQueue.push(stPacketContainer);

		if( ReleaseSemaphore( m_hDequeueSemaphore, 1, NULL ) == TRUE )
		{ 
			bRst = true;
		}
		else
		{ //EX_DTEC_TryCatchException
			TRACE("EnqueuePacket DequeueSemaphore Release Error");
			bRst = false;
		}
	}
	else
	{ //EX_DTEC_TryCatchException
		//TRACE("EnqueuePacket FAILED\n");
		bRst = false;
	}	

	return bRst;
}

bool CGRCommObj::DequeuePacket(void** i_pvPacket, int *i_piLength)
{
	DWORD dRet;
	dRet = WaitForSingleObject(m_hDequeueSemaphore, COMM_TIMEOUT);
	if (WAIT_OBJECT_0 == dRet)
	{
		StMissionPacketContainer *stPacketContainer = m_pstPacketQueue.front();

		*i_pvPacket = stPacketContainer->pvPacket;
		*i_piLength = stPacketContainer->iLength;

		delete stPacketContainer;
		m_pstPacketQueue.pop();

		if (false == ReleaseSemaphore(m_hEnqueueSemaphore, 1, NULL))
		{
			TRACE("DequeuePacket EnqueueSemaphore Release Error");
			return false;
		}
	}
	else if (WAIT_TIMEOUT == dRet)
	{
		//TRACE("DequeuePacket TIMEOUT\n");
		return false;
	}
	else if (WAIT_FAILED == dRet)
	{
		TRACE("DequeuePacket FAILED\n");
		return false;
	}
	else
	{
		TRACE("DequeuePacket ABANDONED\n");
		return false;
	}

	return true;
}

bool CGRCommObj::AddClientInfo(SOCKET i_hClientSocket, unsigned char i_ucLastIP, unsigned short i_usOperatorID, bool i_bDataLink)
{
	DWORD dRet;
	dRet = WaitForSingleObject(m_hClientSemaphore, INFINITE);
	if (WAIT_OBJECT_0 == dRet)
	{
		StClientSocket *stClientSocket = new struct StClientSocket;
		stClientSocket->hSocket = i_hClientSocket;
		stClientSocket->us4thIP = i_ucLastIP;
		stClientSocket->usOperatorID = i_usOperatorID;		
		stClientSocket->bDataLink = i_bDataLink;		

		this->m_pstClientList.push_back(stClientSocket);

		if (false == ReleaseSemaphore(m_hClientSemaphore, 1, NULL))
		{
			TRACE("AddClientInfo ClientSemaphore Release Error");
			return false;
		}
	}
	else if (WAIT_TIMEOUT == dRet)
	{
		TRACE("AddClientInfo TIMEOUT\n");
		return false;
	}
	else if (WAIT_FAILED == dRet)
	{
		TRACE("AddClientInfo FAILED\n");
		return false;
	}
	else
	{
		TRACE("AddClientInfo ABANDONED\n");
		return false;
	}

	return true;
}

bool CGRCommObj::DeleteClientInfo(SOCKET i_hClientSocket)
{
	DWORD dRet;
	dRet = WaitForSingleObject(m_hClientSemaphore, INFINITE);
	if (WAIT_OBJECT_0 == dRet)
	{
		list<StClientSocket*>::iterator iter;
		for(iter = m_pstClientList.begin(); iter != m_pstClientList.end(); ++iter)
		{
			StClientSocket *pTempClient = *iter;
			if(pTempClient->hSocket == i_hClientSocket)
			{
				TRACE("[### TCP PROC CLIENT CLOSE IP %d ####]\n", pTempClient->us4thIP);
				m_pstClientList.erase(iter);				
				delete pTempClient;
				break;
			}
		}

		if (false == ReleaseSemaphore(m_hClientSemaphore, 1, NULL))
		{
			TRACE("DeleteClientInfo ClientSemaphore Release Error");
			return false;
		}
	}
	else if (WAIT_TIMEOUT == dRet)
	{
		TRACE("DeleteClientInfo TIMEOUT\n");
		return false;
	}
	else if (WAIT_FAILED == dRet)
	{
		TRACE("DeleteClientInfo FAILED\n");
		return false;
	}
	else
	{
		TRACE("DeleteClientInfo ABANDONED\n");
		return false;
	}

	return true;
}

StClientSocket* CGRCommObj::GetClientInfo(unsigned char i_ucLastIP)
{
	DWORD dRet;
	dRet = WaitForSingleObject(m_hClientSemaphore, INFINITE);
	if (WAIT_OBJECT_0 == dRet)
	{
		list<StClientSocket*>::iterator iter;
		for(iter = m_pstClientList.begin(); iter != m_pstClientList.end(); ++iter)
		{
			StClientSocket *pTempClient = *iter;
			if(pTempClient->us4thIP == i_ucLastIP)
			{
				if (false == ReleaseSemaphore(m_hClientSemaphore, 1, NULL))
				{
					TRACE("DeleteClientInfo ClientSemaphore Release Error");
					return NULL;
				}

				return pTempClient;
			}
		}
	}
	else if (WAIT_TIMEOUT == dRet)
	{
		TRACE("DeleteClientInfo TIMEOUT\n");
		return NULL;
	}
	else if (WAIT_FAILED == dRet)
	{
		TRACE("DeleteClientInfo FAILED\n");
		return NULL;
	}
	else
	{
		TRACE("DeleteClientInfo ABANDONED\n");
		return NULL;
	}

	return NULL;
}

void CGRCommObj::SetSocketMode(int i_iSocketMode)
{
	m_iSocketMode = i_iSocketMode;
}

int CGRCommObj::GetSocketMode()
{
	return m_iSocketMode;
}

void CGRCommObj::StartTcpServerThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpServerThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpClientThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpClientThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpClientADSBThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpClientADSBThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpClientDF_ThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpClientDF_1ThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpClientPDW_ThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpClientPDW_2ThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpClientRDUnit_ThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpClientRDUnit_3ThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpClientVUHFUnit_ThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpClientVUHFUnit_4ThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartUdpClientThreadFunc(LPVOID arg)
{
	::AfxBeginThread(UdpClientThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);	
}

void CGRCommObj::StartTcpPacketProcessingThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpPacketProcessingThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpPacketProcessingADSBThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpPacketProcessingADSBThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpPacketProcessingThreadFuncForSend(LPVOID arg)
{
	::AfxBeginThread(TcpPacketProcessingThreadFuncForSend, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartRadarLinkPacketProcessingThreadFunc(LPVOID arg)
{
	m_hEncEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	::AfxBeginThread(RadarLinkPacketProcessingThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpRadarLink_1ClientThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpRadarLinkClientThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);
}

void CGRCommObj::StartTcpInterCommClientThreadFunc(LPVOID arg)
{
	::AfxBeginThread(TcpInterCommClientThreadFunc, LPVOID(arg), THREAD_PRIORITY_NORMAL);	
}

/**
* @brief TCP Server Thread �Լ�
* @return ���� ���
*/
UINT CGRCommObj::TcpServerThreadFunc(LPVOID arg)
{
	TRACE("[### TCP SERVER THREAD START ####]\n"); 

	StMissionOperPacket* pstClientPacket = NULL;

	unsigned int uiClientReceivingOffset[MAX_SERVER_CONNECTION_CNT] = {0,};
	unsigned int uiPacketOffset[MAX_SERVER_CONNECTION_CNT] = {0,};

	unsigned int uiPacketStartOffset[MAX_SERVER_CONNECTION_CNT] = {0,};
	bool bReceiveStart[MAX_SERVER_CONNECTION_CNT] = {0,};
	unsigned int uiConnectionIndex = NULL;

	unsigned int uiLoopCnt = NULL;
	for ( uiLoopCnt = NULL; uiLoopCnt < MAX_SERVER_CONNECTION_CNT; uiLoopCnt++ )
	{
		uiClientReceivingOffset[uiLoopCnt] = NULL;
		uiPacketOffset[uiLoopCnt] = NULL;
		uiPacketStartOffset[uiLoopCnt] = NULL;
		bReceiveStart[uiLoopCnt] = false;
	}

	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hServerSocket = *pSocket;

	SOCKET hClientSocket = NULL;
	SOCKADDR_IN stClientAddress = SOCKADDR_IN();
	memset(&stClientAddress, NULL, sizeof(SOCKADDR_IN));
	fd_set stReads = fd_set();
	FD_ZERO(&stReads);
	fd_set stCopyReads = fd_set();
	FD_ZERO(&stCopyReads);
	FD_ZERO(&stReads);

	FD_SET(hServerSocket, &stReads);

	int iFdNum = NULL;
	int iAddressSize = NULL;
	int iLength = NULL;

	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();

	memset( cGRCommObj->m_aucTcpServerRecvBuf, NULL, MAX_SERVER_CONNECTION_CNT * MAX_BUF_SIZE_FOR_CHILD );
	StMissionOperHeader stMissionHeader = StMissionOperHeader();
	memset(&stMissionHeader, NULL, sizeof(StMissionOperHeader));

	TIMEVAL stTimeout = TIMEVAL();
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second
	int iTime = 1;

	while( cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if( NULL != iFdNum && SOCKET_ERROR != iFdNum) 
		{
			for( unsigned int i=0; i < stReads.fd_count; i++)
			{
				if( FD_ISSET(stReads.fd_array[i], &stCopyReads) != NULL )
				{
					if ( i > MAX_SERVER_CONNECTION_CNT )
					{
						uiConnectionIndex = (MAX_SERVER_CONNECTION_CNT-1);
					}
					else
					{
						uiConnectionIndex = i;
					}

					// client ����
					if(stReads.fd_array[i] == hServerSocket)
					{
						iAddressSize = sizeof(stClientAddress);

						hClientSocket = accept(hServerSocket, (SOCKADDR*)&stClientAddress, &iAddressSize);

						if ( hClientSocket != INVALID_SOCKET )
						{
							FD_SET(hClientSocket, &stReads);
							TRACE("[### TCP PROC CLIENT ACCEPT ####]\n");

							setsockopt(hClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTime, sizeof(iTime));

							// Client Handle�� client ��Ͽ� IP �� �� �ڸ��� �Բ� ���� (Operator ID�� �α��� �޽����� ���� ����)														
							if ( hServerSocket == cGRCommObj->GetDataLinkServerSocket() )
							{
								cGRCommObj->AddClientInfo(hClientSocket, stClientAddress.sin_addr.S_un.S_un_b.s_b4, stClientAddress.sin_addr.S_un.S_un_b.s_b4, true);
								TRACE("============== AddClientInfo IP %d\n ", stClientAddress.sin_addr.S_un.S_un_b.s_b4);							
							}
							else
							{
								cGRCommObj->AddClientInfo(hClientSocket, stClientAddress.sin_addr.S_un.S_un_b.s_b4, stClientAddress.sin_addr.S_un.S_un_b.s_b4, false);
								TRACE("AddClientInfo IP %d============== \n", stClientAddress.sin_addr.S_un.S_un_b.s_b4);
							}
						}
					}
					// ������ ����
					else
					{
						iLength = recv(stReads.fd_array[i], (char *)cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex]+uiClientReceivingOffset[uiConnectionIndex], MAX_BUF_SIZE -1 , 0);

						//TRACE("[==Server Recv. Socket[%d], size[%d]\n", uiConnectionIndex, iLength);

						if( NULL >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							hClientSocket = stReads.fd_array[i];

							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							TRACE("[### TCP PROC CLIENT CLOSE ####]\n"); 

							// Client Handle�� client ��Ͽ��� ����
							cGRCommObj->DeleteClientInfo(hClientSocket);

							uiClientReceivingOffset[uiConnectionIndex] = 0;
						}
						else	// ������ ���� ����
						{
							// COMM CONVERT
							//uiClientReceivingOffset[uiConnectionIndex] = CConvertMng::ConvertIntToUINT(iLength) + uiClientReceivingOffset[uiConnectionIndex];
							uiClientReceivingOffset[uiConnectionIndex] = iLength + uiClientReceivingOffset[uiConnectionIndex];

							if ( uiClientReceivingOffset[uiConnectionIndex] >= (sizeof(StMissionOperHeader)) )
							{
								if ( bReceiveStart[uiConnectionIndex] == false)
								{
									unsigned char result[8];
									memcpy(&result, cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex], sizeof(StMissionOperHeader));
									memcpy(&stMissionHeader, cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex], sizeof(StMissionOperHeader));
									//ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
									bReceiveStart[uiConnectionIndex] = true;
								}
								
								if ( uiClientReceivingOffset[uiConnectionIndex] == (stMissionHeader.usSize+sizeof(StMissionOperHeader)) )
								{
									//TRACE("[����ϰ� �� ���]\n");

									pstClientPacket = new struct StMissionOperPacket;
									memcpy(pstClientPacket, cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex], (stMissionHeader.usSize+sizeof(StMissionOperHeader)));
									//ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

									if ( cGRCommObj->EnqueuePacketForSend(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionOperHeader)) == false )
									{
										delete pstClientPacket;
									}

									uiClientReceivingOffset[uiConnectionIndex] = NULL;
									bReceiveStart[uiConnectionIndex] = false;									
								}
								else if ( uiClientReceivingOffset[uiConnectionIndex] > (stMissionHeader.usSize+sizeof(StMissionOperHeader)) )
								{
									//TRACE("[���ļ� �� ���]\n");

									uiPacketOffset[uiConnectionIndex] = uiClientReceivingOffset[uiConnectionIndex];
									uiPacketStartOffset[uiConnectionIndex] = 0;

									//TRACE("[Start uiPacketOffset = %d, uiPacketStartOffset = %d]\n", uiPacketOffset, uiPacketStartOffset);

									while ( uiPacketOffset[uiConnectionIndex] >= (stMissionHeader.usSize+sizeof(StMissionOperHeader)) )
									{
										pstClientPacket = new struct StMissionOperPacket;
										memcpy(pstClientPacket, cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex]+uiPacketStartOffset[uiConnectionIndex], (stMissionHeader.usSize+sizeof(StMissionHeader)));
										//ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

										if ( !cGRCommObj->EnqueuePacketForSend(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionOperHeader) ) )
										{
											delete pstClientPacket;
										}

										uiPacketOffset[uiConnectionIndex] = uiPacketOffset[uiConnectionIndex] - (stMissionHeader.usSize+sizeof(StMissionOperHeader));
										uiPacketStartOffset[uiConnectionIndex] = uiPacketStartOffset[uiConnectionIndex] + (stMissionHeader.usSize+sizeof(StMissionOperHeader));
										//TRACE("[Update uiPacketOffset = %d, uiPacketStartOffset = %d]\n", uiPacketOffset, uiPacketStartOffset);

										// �ű��߰�
										if ( uiPacketOffset[uiConnectionIndex] >= sizeof(StMissionOperHeader) )
										{
											memcpy(&stMissionHeader, cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex]+uiPacketStartOffset[uiConnectionIndex], sizeof(StMissionOperHeader));
											//ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
										}										
									}

									uiClientReceivingOffset[uiConnectionIndex] = uiPacketOffset[uiConnectionIndex];

									if ( uiClientReceivingOffset[uiConnectionIndex] < (int)sizeof(StMissionOperHeader) )
									{
										bReceiveStart[uiConnectionIndex] = false;

										if ( uiClientReceivingOffset[uiConnectionIndex] != NULL )
										{
											// ���� �̵�										
											memcpy(cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex], cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex]+uiPacketStartOffset[uiConnectionIndex], uiClientReceivingOffset[uiConnectionIndex]);
											//TRACE("[�����̵� uiPacketStartOffset = %d, iSize = %d]\n", uiPacketStartOffset, uiClientReceivingOffset);
										}
									}
									else
									{
										bReceiveStart[uiConnectionIndex] = true;

										// ���� �̵�										
										memcpy(cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex], cGRCommObj->m_aucTcpServerRecvBuf[uiConnectionIndex]+uiPacketStartOffset[uiConnectionIndex], uiClientReceivingOffset[uiConnectionIndex]);
										//TRACE("[�����̵� uiPacketStartOffset = %d, iSize = %d]\n", uiPacketStartOffset, uiClientReceivingOffset);
									}
								}
								else
								{ //EX_DTEC_Else
									//DoNothing();
								}
							}							
						}
					}
				}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
			}// for
		}// if(fdNum != 0) 
	}

	//CLog::printf_log(LOG_INFO, "### Tcp Server ThreadFunc end (%d)", entry->getID()); 
	TRACE("[### TCP SERVER THREAD END ####]\n"); 

	return NULL;
}

/**
* @brief TCP Client Thread �Լ�
* @return ���� ���
*/
UINT CGRCommObj::TcpClientThreadFunc(LPVOID arg) // TODO : thread �Լ� classȭ �ʿ�
{	
	TRACE("[### TCP CLIENT THREAD START[For Nexan] ####]\n");

	// TODO : ���� ������ ���� ������ static�� �ʿ�� ����
	static int iClientReceivingOffset = 0;
	static StMissionPacket* pstClientPacket = NULL;

	int iPacketOffset = NULL;
	int iPacketStartOffset = NULL;
	bool bReceiveStart = false;

	fd_set stReads, stCopyReads;
	FD_ZERO(&stReads);
	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hClientSocket = *pSocket;
	FD_SET(hClientSocket, &stReads);

	int iFdNum, iLength;
	bool bConnected = true;

	BYTE RecvBuf[MAX_BUF_SIZE];
	memset(RecvBuf, NULL, MAX_BUF_SIZE);
	StMissionHeader stMissionHeader;
	memset(&stMissionHeader, NULL, sizeof(StMissionHeader));

	SNEXSANPacket* stNEXSANData = new SNEXSANPacket();
	memset(stNEXSANData, NULL, sizeof(SNEXSANPacket));

	TIMEVAL stTimeout;
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second

	while(true == bConnected)
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if(SOCKET_ERROR == iFdNum)  // -1 
		{
			TRACE("TcpClientThreadFunc select Error\n");
			break; // ����
		}
		else if(0 != iFdNum) // select timeout �� �ƴϸ� 
		{
			// ������ ���� �κ� 
			for(unsigned int i=0; i<stReads.fd_count; i++)
			{
				if(false == bConnected)
				{
					break;
				}

				if(FD_ISSET(stReads.fd_array[i], &stCopyReads))
				{
					if(stReads.fd_array[i] == hClientSocket)
					{
						// packet�� ó������ ���� ��� iReceivingOffset�� 0�̱� ������, bReceiving�� ������� ������ �������� ó�� ����
						iLength = recv(stReads.fd_array[i], (char *)RecvBuf, MAX_BUF_SIZE -1 , 0);

						TRACE("TcpClientThreadFunc iLenght = %d, ioffser = %d\n", iLength, iClientReceivingOffset);

						if(0 >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							CGRCommObj& cGRCommObj = CGRCommObj::GetInstance();
							cGRCommObj.SetConnectionInfo(PING_TIMER_SERVER, false);

							TRACE("[### TCP CLIENT THREAD CLOSE ####]\n");
							bConnected = false;

						}
						else
						{
							stNEXSANData->ucID = 100;
							stNEXSANData->Opcode = 100;
							memcpy(stNEXSANData->aucData, RecvBuf, iLength);

							CGRCommObj& cGRCommObj = CGRCommObj::GetInstance();
							cGRCommObj.EnqueuePacket(stNEXSANData, iLength+2);
						}

					}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
				}
			}// for
		}//if(fdNum != 0) 
	}

	//CLog::printf_log(LOG_INFO, "@@@ TcpClientThreadFunc end (%d)", entry->getID());
	TRACE("[### TCP CLIENT THREAD END ####]\n");

	return 0;
}

/**
* @brief TCP Client Thread �Լ�
* @return ���� ���
*/
UINT CGRCommObj::TcpClientADSBThreadFunc(LPVOID arg) // TODO : thread �Լ� classȭ �ʿ�
{	
	TRACE("[### TCP CLIENT THREAD START[For Nexan] ####]\n");

	// TODO : ���� ������ ���� ������ static�� �ʿ�� ����
	static int iClientReceivingOffset = 0;
	static StMissionPacket* pstClientPacket = NULL;

	int iPacketOffset = NULL;
	int iPacketStartOffset = NULL;
	bool bReceiveStart = false;

	fd_set stReads, stCopyReads;
	FD_ZERO(&stReads);
	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hClientSocket = *pSocket;
	FD_SET(hClientSocket, &stReads);

	int iFdNum, iLength;
	bool bConnected = true;

	BYTE RecvBuf[MAX_BUF_SIZE];
	memset(RecvBuf, NULL, MAX_BUF_SIZE);
	StMissionHeader stMissionHeader;
	memset(&stMissionHeader, NULL, sizeof(StMissionHeader));

	SADSBPacket* stLocalADSBData = new SADSBPacket();
	memset(stLocalADSBData, NULL, sizeof(SADSBPacket));

	TIMEVAL stTimeout;
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second

	while(true == bConnected)
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if(SOCKET_ERROR == iFdNum)  // -1 
		{
			TRACE("TcpClientThreadFunc select Error\n");
			break; // ����
		}
		else if(0 != iFdNum) // select timeout �� �ƴϸ� 
		{
			// ������ ���� �κ� 
			for(unsigned int i=0; i<stReads.fd_count; i++)
			{
				if(false == bConnected)
				{
					break;
				}

				if(FD_ISSET(stReads.fd_array[i], &stCopyReads))
				{
					if(stReads.fd_array[i] == hClientSocket)
					{
						// packet�� ó������ ���� ��� iReceivingOffset�� 0�̱� ������, bReceiving�� ������� ������ �������� ó�� ����
						iLength = recv(stReads.fd_array[i], (char *)RecvBuf, MAX_BUF_SIZE -1 , 0);

						TRACE("TcpClientThreadFunc iLenght = %d, ioffser = %d\n", iLength, iClientReceivingOffset);

						if(0 >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							CGRCommObj& cGRCommObj = CGRCommObj::GetInstance();
							cGRCommObj.SetConnectionInfo(PING_TIMER_SERVER, false);

							TRACE("[### TCP CLIENT THREAD CLOSE ####]\n");
							bConnected = false;

						}
						else if(iLength == 23)
						{
							memcpy(stLocalADSBData, RecvBuf, iLength);

							CGRCommObj& cGRCommObj = CGRCommObj::GetInstance();
							cGRCommObj.EnqueuePacket(stLocalADSBData, sizeof(stLocalADSBData));

							TRACE("[EnQueue] ADSB Packet!\n");
						}
						else
						{

						}

					}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
				}
			}// for
		}//if(fdNum != 0) 
	}

	//CLog::printf_log(LOG_INFO, "@@@ TcpClientThreadFunc end (%d)", entry->getID());
	TRACE("[### TCP CLIENT THREAD END ####]\n");

	return 0;
}

UINT CGRCommObj::UdpClientThreadFunc(LPVOID arg)
{
	TRACE("[### UDP CLIENT THREAD START ####]\n");

	static int iClientReceivingOffset = 0;
	StMissionPacket pstClientPacket;	
	int iPacketOffset = NULL;
	int iPacketStartOffset = NULL;
	bool bReceiveStart = false;

	fd_set stReads, stCopyReads;
	FD_ZERO(&stReads);
	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hClientSocket = *pSocket;
	FD_SET(hClientSocket, &stReads);

	SOCKADDR_IN cIntAdr;

	int iFdNum, adrSz, iLength;
	bool bConnected = true;

	BYTE RecvBuf[MAX_BUF_SIZE];
	memset(RecvBuf, NULL, MAX_BUF_SIZE);
	StMissionHeader stMissionHeader;
	memset(&stMissionHeader, NULL, sizeof(StMissionHeader));

	stMissionHeader.stOpcode.ucSource = 0x12;
	stMissionHeader.stOpcode.ucDestination = 0x40;
	stMissionHeader.stOpcode.ucCommandCode = 0x00;
	stMissionHeader.stOpcode.ucEncrytion = 0x00;
	stMissionHeader.stOpcode.ucClass = 0x00;
	stMissionHeader.ucLinkID = 1;
	stMissionHeader.ucRevOprID = 36;	
	stMissionHeader.iOperID = 45;

	
	TIMEVAL stTimeout;
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second
	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();

	while(true == bConnected && cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if(SOCKET_ERROR == iFdNum)  // -1 
		{
			TRACE("[### UDP CLIENT THREAD SELECT ERROR ####]\n");
			break; // ����
		}
		else if(0 != iFdNum) // select timeout �� �ƴϸ� 
		{
			// ������ ���� �κ� 
			for(unsigned int i=0; i<stReads.fd_count; i++)
			{
				if(false == bConnected)
				{
					break;
				}

				if(FD_ISSET(stReads.fd_array[i], &stCopyReads))
				{
					if(stReads.fd_array[i] == hClientSocket)
					{
						adrSz = sizeof(cIntAdr);						
						iLength = recvfrom(hClientSocket, (char *)RecvBuf, MAX_BUF_SIZE -1, 0, (SOCKADDR*)&cIntAdr, &adrSz);

						//AfxMessageBox("�Դ�");

						if(0 >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							TRACE("[### UDP CLIENT THREAD CLOSE ####]\n");
							bConnected = false;
						}
						else
						{
							stMissionHeader.usSize = iLength;
							//CGRCommObj& cGRCommObj = CGRCommObj::GetInstance();
							//TRACE("[����ϰ� �� ���]\n");
							//pstClientPacket = new struct StMissionPacket;
							memcpy(&pstClientPacket.stMissionHeader, &stMissionHeader,sizeof(StMissionHeader));
							//memcpy(pstClientPacket, &stMissionHeader, sizeof(StMissionHeader));
							//void* pTracke = pstClientPacket;
							memcpy(&pstClientPacket.pcData, RecvBuf, stMissionHeader.usSize);
							
							//memcpy(pstClientPacket+sizeof(StMissionHeader), RecvBuf, stMissionHeader.usSize);
							//ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

							if ( cGRCommObj->EnqueuePacket(&pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader) ) == false )
							{
							/*	delete pstClientPacket;
								pstClientPacket = NULL;*/
							}

							
							/*stMissionHeader.stOpcode = 0x12400000;;
							stMissionHeader.ucLinkID = 1;
							stMissionHeader.ucRevOprID = 100;
							stMissionHeader.usSize = iLength;
							stMissionHeader.iOperID = 45;

							cGRCommObj.EnqueuePacket(RecvBuf, 0);*/
						}

					}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
				}
			}// for
		}//if(fdNum != 0) 
	}

	//CLog::printf_log(LOG_INFO, "@@@ TcpClientThreadFunc end (%d)", entry->getID());
	TRACE("[### UDP CLIENT THREAD END ####]\n");

	return 0;
}

/**
* @brief TCP Client Thread �Լ�
* @return ���� ���
*/
UINT CGRCommObj::TcpClientDF_1ThreadFunc(LPVOID arg) // TODO : thread �Լ� classȭ �ʿ�
{	
	TRACE("[### TCP CLIENT RADARDF_1 THREAD START ####]\n");

	int iClientReceivingOffset = NULL;
	StMissionPacket* pstClientPacket = NULL;

	unsigned int uiPacketOffset = NULL;
	unsigned int uiPacketStartOffset = NULL;
	bool bReceiveStart = false;

	fd_set stReads = fd_set();
	FD_ZERO(&stReads);
	fd_set stCopyReads = fd_set();
	FD_ZERO(&stCopyReads);
	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hClientSocket = *pSocket;
	FD_SET(hClientSocket, &stReads);

	int iFdNum = NULL;
	int iLength = NULL;
	bool bConnected = true;

	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();

	memset(cGRCommObj->m_aucTcpClientRecvBuf_1, NULL, MAX_BUF_SIZE);
	StMissionHeader stMissionHeader = StMissionHeader();
	memset(&stMissionHeader, NULL, sizeof(StMissionHeader));

	TIMEVAL stTimeout = TIMEVAL();
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second

	while( bConnected == true && cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if( NULL != iFdNum && SOCKET_ERROR != iFdNum ) // select timeout �� �ƴϸ� 
		{
			// ������ ���� �κ� 
			for( unsigned int i=0; i< stReads.fd_count; i++ )
			{
				if( bConnected == false )
				{
					break;
				}

				if( FD_ISSET(stReads.fd_array[i], &stCopyReads) != NULL )
				{
					if( stReads.fd_array[i] == hClientSocket )
					{
						iLength = recv(stReads.fd_array[i], (char *)cGRCommObj->m_aucTcpClientRecvBuf_1+iClientReceivingOffset, MAX_BUF_SIZE -1 , 0);

						//TRACE( "�ߡ� TcpClientDF_1ThreadFunc iLenght = %d, ioffser = %d\n", iLength, iClientReceivingOffset);

						if( NULL >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							cGRCommObj->SetConnectionInfo(CLIENT_NO_1, false);

							TRACE("[### TCP CLIENT RADAR_DF1 THREAD CLOSE ####]\n");
							TRACE("���̴� �м� #1���� ���������ϴ�." );						
							bConnected = false;
						}
						else
						{
							iClientReceivingOffset = iLength + iClientReceivingOffset;

							// for debug
							if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
							{
								TRACE("\r\n[TcpClientDF_1ThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
							}

							if ( iClientReceivingOffset >= (int)sizeof(StMissionHeader) )
							{
								if ( bReceiveStart == false )
								{
									memcpy(&stMissionHeader, cGRCommObj->m_aucTcpClientRecvBuf_1, sizeof(StMissionHeader));
									ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
									bReceiveStart = true;
								}

								if ( iClientReceivingOffset == (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
								{
									//TRACE("[TcpClientDF_1ThreadFunc ����ϰ� �� ���]\n");

									pstClientPacket = new struct StMissionPacket;
									memcpy(pstClientPacket, cGRCommObj->m_aucTcpClientRecvBuf_1, (stMissionHeader.usSize+sizeof(StMissionHeader)));
									ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

									if ( cGRCommObj->EnqueuePacket(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
									{
										delete pstClientPacket;
										pstClientPacket = NULL;
									}

									iClientReceivingOffset = 0;
									bReceiveStart = false;
								}
								else if ( iClientReceivingOffset > (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
								{
									//TRACE("[TcpClientDF_1ThreadFunc ���ļ� �� ���]\n");

									// COMM CONVERT
									//uiPacketOffset = CConvertMng::ConvertIntToUINT(iClientReceivingOffset);
									uiPacketOffset = iClientReceivingOffset;
									uiPacketStartOffset = 0;

									//TRACE("[Start uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

									while ( uiPacketOffset >= (stMissionHeader.usSize+sizeof(StMissionHeader)) )
									{
										pstClientPacket = new struct StMissionPacket;
										memcpy(pstClientPacket, cGRCommObj->m_aucTcpClientRecvBuf_1+uiPacketStartOffset, (stMissionHeader.usSize+sizeof(StMissionHeader)));
										ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

										if ( cGRCommObj->EnqueuePacket(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
										{
											delete pstClientPacket;
											pstClientPacket = NULL;
										}

										uiPacketOffset = uiPacketOffset - (stMissionHeader.usSize+sizeof(StMissionHeader));
										uiPacketStartOffset = uiPacketStartOffset + (stMissionHeader.usSize+sizeof(StMissionHeader));
										//TRACE("[Update uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

										// �ű��߰�
										if ( uiPacketOffset >= sizeof(StMissionHeader) )
										{
											memcpy(&stMissionHeader, cGRCommObj->m_aucTcpClientRecvBuf_1+uiPacketStartOffset, sizeof(StMissionHeader));
											ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
										}										
									}

									// COMM CONVERT
									//iClientReceivingOffset = CConvertMng::ConvertUINTToInt(uiPacketOffset);
									iClientReceivingOffset = uiPacketOffset;

									// for debug
									if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
									{
										TRACE("\r\n[TcpClientDF_1ThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
									}

									if ( iClientReceivingOffset < (int)sizeof(StMissionHeader) )
									{
										bReceiveStart = false;

										if ( iClientReceivingOffset != NULL )
										{
											// ���� �̵�
											// COMM CONVERT
											//memcpy(cGRCommObj->m_aucTcpClientRecvBuf_1, cGRCommObj->m_aucTcpClientRecvBuf_1+uiPacketStartOffset, CConvertMng::ConvertIntToUINT(iClientReceivingOffset) );
											memcpy(cGRCommObj->m_aucTcpClientRecvBuf_1, cGRCommObj->m_aucTcpClientRecvBuf_1+uiPacketStartOffset, iClientReceivingOffset );
											//TRACE("[TcpClientDF_1ThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
										}
									}
									else
									{
										bReceiveStart = true;

										// ���� �̵�
										// COMM CONVERT
										//memcpy(cGRCommObj->m_aucTcpClientRecvBuf_1, cGRCommObj->m_aucTcpClientRecvBuf_1+uiPacketStartOffset, CConvertMng::ConvertIntToUINT(iClientReceivingOffset) );
										memcpy(cGRCommObj->m_aucTcpClientRecvBuf_1, cGRCommObj->m_aucTcpClientRecvBuf_1+uiPacketStartOffset, iClientReceivingOffset );
										//TRACE("[TcpClientDF_1ThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
									}
								}
								else
								{ //EX_DTEC_Else
									//DoNothing();
								}
							}												
						}

					}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
				}
			}// for
		}//if(fdNum != 0) 
	}

	TRACE("[### TCP CLIENT RADARDF_1 THREAD END ####]\n");

	return NULL;
}


	UINT CGRCommObj::TcpClientPDW_2ThreadFunc(LPVOID arg)
{	
	TRACE("[### TCP CLIENT PDW_2 THREAD START ####]\n");

	int iClientReceivingOffset = NULL;
	StMissionPacket* pstClientPacket = NULL;

	unsigned int uiPacketOffset = NULL;
	unsigned int uiPacketStartOffset = NULL;
	bool bReceiveStart = false;

	fd_set stReads = fd_set();
	FD_ZERO(&stReads);
	fd_set stCopyReads = fd_set();
	FD_ZERO(&stCopyReads);
	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hClientSocket = *pSocket;
	FD_SET(hClientSocket, &stReads);

	int iFdNum = NULL;
	int iLength = NULL;
	bool bConnected = true;

	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();

	memset(cGRCommObj->m_aucTcpClientRecvBuf_2, NULL, MAX_BUF_SIZE);
	StMissionHeader stMissionHeader = StMissionHeader();
	memset(&stMissionHeader, NULL, sizeof(StMissionHeader));

	TIMEVAL stTimeout = TIMEVAL();
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second

	while( bConnected == true && cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if( NULL != iFdNum && SOCKET_ERROR != iFdNum ) // select timeout �� �ƴϸ� 
		{
			// ������ ���� �κ� 
			for( unsigned int i=0; i< stReads.fd_count; i++ )
			{
				if( bConnected == false )
				{
					break;
				}

				if( FD_ISSET(stReads.fd_array[i], &stCopyReads) != NULL )
				{
					if( stReads.fd_array[i] == hClientSocket )
					{
						iLength = recv(stReads.fd_array[i], (char *)cGRCommObj->m_aucTcpClientRecvBuf_2+iClientReceivingOffset, MAX_BUF_SIZE -1 , 0);

						//TRACE( "�ߡ� TcpClientGMI_1ThreadFunc iLenght = %d, ioffser = %d\n", iLength, iClientReceivingOffset);

						if( NULL >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							cGRCommObj->SetConnectionInfo(CLIENT_NO_2, false);

							TRACE("[### TCP CLIENT PDW_2 THREAD CLOSE ####]\n");
							TRACE("PDW�߻��� CLIENT #2���� ���������ϴ�.\n");
							bConnected = false;
						}
						else
						{
							iClientReceivingOffset = iLength + iClientReceivingOffset;

							// for debug
							if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
							{
								TRACE("\r\n[TcpClientGMI_2ThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
							}

							if ( iClientReceivingOffset >= (int)(sizeof(StMissionHeader)) )
							{
								if ( bReceiveStart == false )
								{
									memcpy(&stMissionHeader, cGRCommObj->m_aucTcpClientRecvBuf_2, sizeof(StMissionHeader));
									ChangeEndiannessForMissionHeadeer(&stMissionHeader, sizeof(StMissionHeader));
									bReceiveStart = true;
								}

								if ( iClientReceivingOffset == (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
								{
									//TRACE("[TcpClientPDW_2ThreadFunc ����ϰ� �� ���]\n");

									pstClientPacket = new struct StMissionPacket;
									memcpy(pstClientPacket, cGRCommObj->m_aucTcpClientRecvBuf_2, (stMissionHeader.usSize+sizeof(StMissionHeader)));
									ChangeEndiannessForMissionHeadeer(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

									if ( cGRCommObj->EnqueuePacket(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
									{
										delete pstClientPacket;
										pstClientPacket = NULL;
									}

									iClientReceivingOffset = 0;
									bReceiveStart = false;
								}
								else if ( iClientReceivingOffset > (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
								{
									//TRACE("[TcpClientPDW_2ThreadFunc ���ļ� �� ���]\n");

									// COMM CONVERT
									//uiPacketOffset = CConvertMng::ConvertIntToUINT(iClientReceivingOffset);
									uiPacketOffset = iClientReceivingOffset;
									uiPacketStartOffset = 0;

									//TRACE("[Start uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

									while ( uiPacketOffset >= (stMissionHeader.usSize+sizeof(StMissionHeader)) )
									{
										pstClientPacket = new struct StMissionPacket;
										memcpy(pstClientPacket, cGRCommObj->m_aucTcpClientRecvBuf_2+uiPacketStartOffset, (stMissionHeader.usSize+sizeof(StMissionHeader)));
										ChangeEndiannessForMissionHeadeer(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

										if ( cGRCommObj->EnqueuePacket(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
										{
											delete pstClientPacket;
											pstClientPacket = NULL;
										}

										uiPacketOffset = uiPacketOffset - (stMissionHeader.usSize+sizeof(StMissionHeader));
										uiPacketStartOffset = uiPacketStartOffset + (stMissionHeader.usSize+sizeof(StMissionHeader));
										//TRACE("[Update uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

										// �ű��߰�
										if ( uiPacketOffset >= sizeof(StMissionHeader) )
										{
											memcpy(&stMissionHeader, cGRCommObj->m_aucTcpClientRecvBuf_2+uiPacketStartOffset, sizeof(StMissionHeader));
											ChangeEndiannessForMissionHeadeer(&stMissionHeader, sizeof(StMissionHeader));
										}										
									}

									// COMM CONVERT
									//iClientReceivingOffset = CConvertMng::ConvertUINTToInt(uiPacketOffset);
									iClientReceivingOffset = uiPacketOffset;

									// for debug
									if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
									{
										TRACE("\r\n[TcpClientPDW_2ThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
									}

									if ( iClientReceivingOffset < (int)sizeof(StMissionHeader) )
									{
										bReceiveStart = false;

										if ( iClientReceivingOffset != NULL )
										{
											// ���� �̵�
											// COMMM CONVERT
											//memcpy(cGRCommObj->m_aucTcpClientRecvBuf_2, cGRCommObj->m_aucTcpClientRecvBuf_2+uiPacketStartOffset, CConvertMng::ConvertIntToUINT(iClientReceivingOffset) );
											memcpy(cGRCommObj->m_aucTcpClientRecvBuf_2, cGRCommObj->m_aucTcpClientRecvBuf_2+uiPacketStartOffset, iClientReceivingOffset );
											//TRACE("[TcpClientPDW_2ThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
										}
									}
									else
									{
										bReceiveStart = true;

										// ���� �̵�	
										// COMM CONVERT
										//memcpy(cGRCommObj->m_aucTcpClientRecvBuf_2, cGRCommObj->m_aucTcpClientRecvBuf_2+uiPacketStartOffset, CConvertMng::ConvertIntToUINT(iClientReceivingOffset) );
										memcpy(cGRCommObj->m_aucTcpClientRecvBuf_2, cGRCommObj->m_aucTcpClientRecvBuf_2+uiPacketStartOffset, iClientReceivingOffset );
										//TRACE("[TcpClientPDW_2ThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
									}
								}
								else
								{ //EX_DTEC_Else
									//DoNothing();
								}
							}												
						}

					}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
				}
			}// for
		}//if(fdNum != 0) 
	}

	TRACE("[### TCP CLIENT PDW_2 THREAD END ####]\n");

	return NULL;
}

UINT CGRCommObj::TcpClientRDUnit_3ThreadFunc(LPVOID arg)
{	
	TRACE("[### TCP CLIENT RadarUNIT_3 THREAD START ####]\n");

	int iClientReceivingOffset = NULL;
	StMissionPacket* pstClientPacket = NULL;

	unsigned int uiPacketOffset = NULL;
	unsigned int uiPacketStartOffset = NULL;
	bool bReceiveStart = false;

	fd_set stReads = fd_set();
	FD_ZERO(&stReads);
	fd_set stCopyReads = fd_set();
	FD_ZERO(&stCopyReads);
	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hClientSocket = *pSocket;
	FD_SET(hClientSocket, &stReads);

	int iFdNum = NULL;
	int iLength = NULL;
	bool bConnected = true;

	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();

	memset(cGRCommObj->m_aucTcpClientRecvBuf_3, NULL, MAX_BUF_SIZE);
	StMissionHeader stMissionHeader = StMissionHeader();
	memset(&stMissionHeader, NULL, sizeof(StMissionHeader));

	TIMEVAL stTimeout = TIMEVAL();
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second

	while( bConnected == true && cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if( NULL != iFdNum && SOCKET_ERROR != iFdNum ) // select timeout �� �ƴϸ� 
		{
			// ������ ���� �κ� 
			for( unsigned int i=0; i< stReads.fd_count; i++ )
			{
				if( bConnected == false )
				{
					break;
				}

				if( FD_ISSET(stReads.fd_array[i], &stCopyReads) != NULL )
				{
					if( stReads.fd_array[i] == hClientSocket )
					{
						iLength = recv(stReads.fd_array[i], (char *)cGRCommObj->m_aucTcpClientRecvBuf_3+iClientReceivingOffset, MAX_BUF_SIZE -1 , 0);

						//TRACE( "�ߡ� TcpClientRDUnit_3ThreadFunc iLenght = %d, ioffser = %d\n", iLength, iClientReceivingOffset);

						if( NULL >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							cGRCommObj->SetConnectionInfo(CLIENT_NO_3, false);

							TRACE("[### TCP CLIENT RadarUnit_3 THREAD CLOSE ####]\n");
							TRACE("RadarUnit CLIENT #3���� ���������ϴ�.\n");
							bConnected = false;
						}
						else
						{
							iClientReceivingOffset = iLength + iClientReceivingOffset;

							// for debug
							if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
							{
								TRACE("\r\n[TcpClientRDUnit_3ThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
							}

							if ( iClientReceivingOffset >= (int)(sizeof(StMissionHeader)) )
							{
								if ( bReceiveStart == false )
								{
									memcpy(&stMissionHeader, cGRCommObj->m_aucTcpClientRecvBuf_3, sizeof(StMissionHeader));
									ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
									bReceiveStart = true;
								}

								if ( iClientReceivingOffset == (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
								{
									//TRACE("[TcpClientRDUnit_3ThreadFunc ����ϰ� �� ���]\n");

									pstClientPacket = new struct StMissionPacket;
									memcpy(pstClientPacket, cGRCommObj->m_aucTcpClientRecvBuf_3, (stMissionHeader.usSize+sizeof(StMissionHeader)));
									ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

									if ( cGRCommObj->EnqueuePacket(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
									{
										delete pstClientPacket;
										pstClientPacket = NULL;
									}

									iClientReceivingOffset = 0;
									bReceiveStart = false;
								}
								else if ( iClientReceivingOffset > (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
								{
									//TRACE("[TcpClientRDUnit_3ThreadFunc ���ļ� �� ���]\n");

									// COMM CONVERT
									//uiPacketOffset = CConvertMng::ConvertIntToUINT(iClientReceivingOffset);
									uiPacketOffset = iClientReceivingOffset;
									uiPacketStartOffset = 0;

									//TRACE("[Start uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

									while ( uiPacketOffset >= (stMissionHeader.usSize+sizeof(StMissionHeader)) )
									{
										pstClientPacket = new struct StMissionPacket;
										memcpy(pstClientPacket, cGRCommObj->m_aucTcpClientRecvBuf_3+uiPacketStartOffset, (stMissionHeader.usSize+sizeof(StMissionHeader)));
										ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

										if ( cGRCommObj->EnqueuePacket(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
										{
											delete pstClientPacket;
											pstClientPacket = NULL;
										}

										uiPacketOffset = uiPacketOffset - (stMissionHeader.usSize+sizeof(StMissionHeader));
										uiPacketStartOffset = uiPacketStartOffset + (stMissionHeader.usSize+sizeof(StMissionHeader));
										//TRACE("[Update uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

										// �ű��߰�
										if ( uiPacketOffset >= sizeof(StMissionHeader) )
										{
											memcpy(&stMissionHeader, cGRCommObj->m_aucTcpClientRecvBuf_3+uiPacketStartOffset, sizeof(StMissionHeader));
											ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
										}										
									}

									// COMM CONVERT
									//iClientReceivingOffset = CConvertMng::ConvertUINTToInt(uiPacketOffset);
									iClientReceivingOffset = uiPacketOffset;

									// for debug
									if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
									{
										TRACE("\r\n[TcpClientRDUnit_3ThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
									}

									if ( iClientReceivingOffset < (int)sizeof(StMissionHeader) )
									{
										bReceiveStart = false;

										if ( iClientReceivingOffset != NULL )
										{
											// ���� �̵�
											// COMMM CONVERT
											//memcpy(cGRCommObj->m_aucTcpClientRecvBuf_2, cGRCommObj->m_aucTcpClientRecvBuf_2+uiPacketStartOffset, CConvertMng::ConvertIntToUINT(iClientReceivingOffset) );
											memcpy(cGRCommObj->m_aucTcpClientRecvBuf_3, cGRCommObj->m_aucTcpClientRecvBuf_3+uiPacketStartOffset, iClientReceivingOffset );
											//TRACE("[TcpClientRDUnit_3ThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
										}
									}
									else
									{
										bReceiveStart = true;

										// ���� �̵�	
										// COMM CONVERT
										memcpy(cGRCommObj->m_aucTcpClientRecvBuf_3, cGRCommObj->m_aucTcpClientRecvBuf_3+uiPacketStartOffset, iClientReceivingOffset );
										//TRACE("[TcpClientRDUnit_3ThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
									}
								}
								else
								{ //EX_DTEC_Else
									//DoNothing();
								}
							}												
						}

					}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
				}
			}// for
		}//if(fdNum != 0) 
	}

	TRACE("[### TCP CLIENT RadarUnit_3 THREAD END ####]\n");

	return NULL;
}

UINT CGRCommObj::TcpClientVUHFUnit_4ThreadFunc(LPVOID arg)
{	
	TRACE("[### TCP CLIENT SptrVUHF_4 THREAD START ####]\n");

	int iClientReceivingOffset = NULL;
	StMissionPacket* pstClientPacket = NULL;

	unsigned int uiPacketOffset = NULL;
	unsigned int uiPacketStartOffset = NULL;
	bool bReceiveStart = false;

	fd_set stReads = fd_set();
	FD_ZERO(&stReads);
	fd_set stCopyReads = fd_set();
	FD_ZERO(&stCopyReads);
	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hClientSocket = *pSocket;
	FD_SET(hClientSocket, &stReads);

	int iFdNum = NULL;
	int iLength = NULL;
	bool bConnected = true;

	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();

	memset(cGRCommObj->m_aucTcpClientRecvBuf_4, NULL, MAX_BUF_SIZE);
	StMissionHeader stMissionHeader = StMissionHeader();
	memset(&stMissionHeader, NULL, sizeof(StMissionHeader));

	TIMEVAL stTimeout = TIMEVAL();
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second

	while( bConnected == true && cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if( NULL != iFdNum && SOCKET_ERROR != iFdNum ) // select timeout �� �ƴϸ� 
		{
			// ������ ���� �κ� 
			for( unsigned int i=0; i< stReads.fd_count; i++ )
			{
				if( bConnected == false )
				{
					break;
				}

				if( FD_ISSET(stReads.fd_array[i], &stCopyReads) != NULL )
				{
					if( stReads.fd_array[i] == hClientSocket )
					{
						iLength = recv(stReads.fd_array[i], (char *)cGRCommObj->m_aucTcpClientRecvBuf_4+iClientReceivingOffset, MAX_BUF_SIZE -1 , 0);

						//TRACE( "�ߡ� TcpClientVUHFUnit_4ThreadFunc iLenght = %d, ioffser = %d\n", iLength, iClientReceivingOffset);

						if( NULL >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							cGRCommObj->SetConnectionInfo(CLIENT_NO_4, false);

							TRACE("[### TCP CLIENT SptrOper_4 THREAD CLOSE ####]\n");
							TRACE("SptrOper CLIENT #4���� ���������ϴ�.\n");
							bConnected = false;
						}
						else
						{
							iClientReceivingOffset = iLength + iClientReceivingOffset;

							// for debug
							if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
							{
								TRACE("\r\n[TcpClientVUHFUnit_4ThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
							}

							if ( iClientReceivingOffset >= (int)(sizeof(StMissionHeader)) )
							{
								if ( bReceiveStart == false )
								{
									memcpy(&stMissionHeader, cGRCommObj->m_aucTcpClientRecvBuf_4, sizeof(StMissionHeader));
									ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
									bReceiveStart = true;
								}

								if ( iClientReceivingOffset == (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
								{
									//TRACE("[TcpClientVUHFUnit_4ThreadFunc ����ϰ� �� ���]\n");

									pstClientPacket = new struct StMissionPacket;
									memcpy(pstClientPacket, cGRCommObj->m_aucTcpClientRecvBuf_4, (stMissionHeader.usSize+sizeof(StMissionHeader)));
									ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

									if ( cGRCommObj->EnqueuePacket(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
									{
										delete pstClientPacket;
										pstClientPacket = NULL;
									}

									iClientReceivingOffset = 0;
									bReceiveStart = false;
								}
								else if ( iClientReceivingOffset > (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
								{
									//TRACE("[TcpClientVUHFUnit_4ThreadFunc ���ļ� �� ���]\n");

									// COMM CONVERT
									//uiPacketOffset = CConvertMng::ConvertIntToUINT(iClientReceivingOffset);
									uiPacketOffset = iClientReceivingOffset;
									uiPacketStartOffset = 0;

									//TRACE("[Start uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

									while ( uiPacketOffset >= (stMissionHeader.usSize+sizeof(StMissionHeader)) )
									{
										pstClientPacket = new struct StMissionPacket;
										memcpy(pstClientPacket, cGRCommObj->m_aucTcpClientRecvBuf_4+uiPacketStartOffset, (stMissionHeader.usSize+sizeof(StMissionHeader)));
										ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

										if ( cGRCommObj->EnqueuePacket(pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
										{
											delete pstClientPacket;
											pstClientPacket = NULL;
										}

										uiPacketOffset = uiPacketOffset - (stMissionHeader.usSize+sizeof(StMissionHeader));
										uiPacketStartOffset = uiPacketStartOffset + (stMissionHeader.usSize+sizeof(StMissionHeader));
										//TRACE("[Update uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

										// �ű��߰�
										if ( uiPacketOffset >= sizeof(StMissionHeader) )
										{
											memcpy(&stMissionHeader, cGRCommObj->m_aucTcpClientRecvBuf_4+uiPacketStartOffset, sizeof(StMissionHeader));
											ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
										}										
									}

									// COMM CONVERT
									//iClientReceivingOffset = CConvertMng::ConvertUINTToInt(uiPacketOffset);
									iClientReceivingOffset = uiPacketOffset;

									// for debug
									if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
									{
										TRACE("\r\n[TcpClientVUHFUnit_4ThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
									}

									if ( iClientReceivingOffset < (int)sizeof(StMissionHeader) )
									{
										bReceiveStart = false;

										if ( iClientReceivingOffset != NULL )
										{
											// ���� �̵�
											// COMMM CONVERT
											//memcpy(cGRCommObj->m_aucTcpClientRecvBuf_2, cGRCommObj->m_aucTcpClientRecvBuf_2+uiPacketStartOffset, CConvertMng::ConvertIntToUINT(iClientReceivingOffset) );
											memcpy(cGRCommObj->m_aucTcpClientRecvBuf_4, cGRCommObj->m_aucTcpClientRecvBuf_4+uiPacketStartOffset, iClientReceivingOffset );
											//TRACE("[TcpClientVUHFUnit_4ThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
										}
									}
									else
									{
										bReceiveStart = true;

										// ���� �̵�	
										// COMM CONVERT
										//memcpy(cGRCommObj->m_aucTcpClientRecvBuf_2, cGRCommObj->m_aucTcpClientRecvBuf_2+uiPacketStartOffset, CConvertMng::ConvertIntToUINT(iClientReceivingOffset) );
										memcpy(cGRCommObj->m_aucTcpClientRecvBuf_4, cGRCommObj->m_aucTcpClientRecvBuf_4+uiPacketStartOffset, iClientReceivingOffset );
										//TRACE("[TcpClientVUHFUnit_4ThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
									}
								}
								else
								{ //EX_DTEC_Else
									//DoNothing();
								}
							}												
						}

					}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
				}
			}// for
		}//if(fdNum != 0) 
	}

	TRACE("[### TCP CLIENT SptrOper_4 THREAD END ####]\n");

	return NULL;
}

/**
* @brief TCP Client Thread �Լ�
* @return ���� ���
*/
UINT CGRCommObj::TcpRadarLinkClientThreadFunc(LPVOID arg)
{	
	TRACE("[### TCP RADAR_LINK CLIENT THREAD START ####]\n");

	int iClientReceivingOffset = NULL;
	StMissionPacket pstClientPacket = StMissionPacket();

	unsigned int uiPacketOffset = NULL;
	unsigned int uiPacketStartOffset = NULL;
	bool bReceiveStart = false;

	fd_set stReads = fd_set();
	FD_ZERO(&stReads);
	fd_set stCopyReads = fd_set();
	FD_ZERO(&stCopyReads);
	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hClientSocket = *pSocket;
	FD_SET(hClientSocket, &stReads);

	StMissionHeader stMissionHeader;
	memset(&stMissionHeader, NULL, sizeof(StMissionHeader));

	stMissionHeader.stOpcode.ucSource = 0xFF;
	stMissionHeader.stOpcode.ucDestination = 0x00;
	stMissionHeader.stOpcode.ucCommandCode = 0x01;
	stMissionHeader.stOpcode.ucClass = 0x01;
	stMissionHeader.ucLinkID = 0;
	stMissionHeader.ucRevOprID = 0;
	stMissionHeader.usSize = 0;

	int iFdNum = NULL;
	int iLength = NULL;
	bool bConnected = true;

	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();

	memset(cGRCommObj->m_aucRadarLink_1RecvBuf, NULL, MAX_BUF_SIZE);
	
	TIMEVAL stTimeout = TIMEVAL();
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second

	while( bConnected == true && cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if( NULL != iFdNum && SOCKET_ERROR != iFdNum ) // select timeout �� �ƴϸ� 
		{
			// ������ ���� �κ� 
			for( unsigned int i=0; i< stReads.fd_count; i++ )
			{
				if( bConnected == false )
				{
					break;
				}

				if( FD_ISSET(stReads.fd_array[i], &stCopyReads) != NULL )
				{
					if( stReads.fd_array[i] == hClientSocket )
					{
						iLength = recv(stReads.fd_array[i], (char *)cGRCommObj->m_aucRadarLink_1RecvBuf+iClientReceivingOffset, MAX_BUF_SIZE -1 , 0);

						//TRACE( "�ߡ� TcpRadarLinkClientThreadFunc iLenght = %d, ioffser = %d\n", iLength, iClientReceivingOffset);

						if( NULL >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							cGRCommObj->SetConnectionInfo(CLIENT_NO_1, false);

							TRACE("[### TCP CLIENT GMI_1 THREAD CLOSE ####]\n");
							TRACE("������ #1���� ���������ϴ�." );						
							bConnected = false;
						}
						else
						{
							// Dummy Mission Header �߰��Ͽ� ���ǵ� OPCODE�� Enqueue�ϴ� ��ƾ �߰�
							if(iLength == 23)
							{
								//TRACE( "�ߡ� TcpRadarLinkClientThreadFunc iLenght = %d, ioffser = %d\n", iLength, iClientReceivingOffset);

								stMissionHeader.usSize = iLength;
								
								memcpy(&pstClientPacket.stMissionHeader, &stMissionHeader,sizeof(StMissionHeader));
								memcpy(&pstClientPacket.pcData, (char *)cGRCommObj->m_aucRadarLink_1RecvBuf+iClientReceivingOffset, stMissionHeader.usSize);
							
								cGRCommObj->EnqueuePacket(&pstClientPacket, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader) );
							}
							
							//iClientReceivingOffset = iLength + iClientReceivingOffset;

							//// for debug
							//if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
							//{
							//	TRACE("\r\n[TcpRadarLinkClientThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
							//}

							//if ( iClientReceivingOffset >= (int)sizeof(StMissionHeader) )
							//{
							//	if ( bReceiveStart == false )
							//	{
							//		memcpy(&stMissionHeader, cGRCommObj->m_aucRadarLink_1RecvBuf, sizeof(StMissionHeader));
							//		ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
							//		bReceiveStart = true;
							//	}

							//	if ( iClientReceivingOffset == (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
							//	{
							//		//TRACE("[TcpRadarLinkClientThreadFunc ����ϰ� �� ���]\n");

							//		pstClientPacket = new struct StMissionPacket;
							//		memcpy(pstClientPacket, cGRCommObj->m_aucRadarLink_1RecvBuf, (stMissionHeader.usSize+sizeof(StMissionHeader)));
							//		ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

							//		if ( cGRCommObj->RecvFromLink(cGRCommObj->m_aucRadarLink_1RecvBuf, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
							//		{
							//			delete pstClientPacket;
							//			pstClientPacket = NULL;
							//		}

							//		iClientReceivingOffset = 0;
							//		bReceiveStart = false;
							//	}
							//	else if ( iClientReceivingOffset > (int)(stMissionHeader.usSize+sizeof(StMissionHeader)) )
							//	{
							//		//TRACE("[TcpRadarLinkClientThreadFunc ���ļ� �� ���]\n");

							//		// COMM CONVERT
							//		//uiPacketOffset = CConvertMng::ConvertIntToUINT(iClientReceivingOffset);
							//		uiPacketOffset = iClientReceivingOffset;
							//		uiPacketStartOffset = 0;

							//		//TRACE("[Start uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

							//		while ( uiPacketOffset >= (stMissionHeader.usSize+sizeof(StMissionHeader)) )
							//		{
							//			pstClientPacket = new struct StMissionPacket;
							//			memcpy(pstClientPacket, cGRCommObj->m_aucRadarLink_1RecvBuf+uiPacketStartOffset, (stMissionHeader.usSize+sizeof(StMissionHeader)));
							//			ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));

							//			if ( cGRCommObj->RecvFromLink(cGRCommObj->m_aucRadarLink_1RecvBuf, (int)stMissionHeader.usSize + (int)sizeof(StMissionHeader)) == false )
							//			{
							//				delete pstClientPacket;
							//				pstClientPacket = NULL;
							//			}

							//			uiPacketOffset = uiPacketOffset - (stMissionHeader.usSize+sizeof(StMissionHeader));
							//			uiPacketStartOffset = uiPacketStartOffset + (stMissionHeader.usSize+sizeof(StMissionHeader));
							//			//TRACE("[Update uiPacketOffset = %d, iPacketStartOffset = %d]\n", uiPacketOffset, iPacketStartOffset);

							//			// �ű��߰�
							//			if ( uiPacketOffset >= sizeof(StMissionHeader) )
							//			{
							//				memcpy(&stMissionHeader, cGRCommObj->m_aucRadarLink_1RecvBuf+uiPacketStartOffset, sizeof(StMissionHeader));
							//				ChangeEndianness(&stMissionHeader, sizeof(StMissionHeader));
							//			}										
							//		}

							//		// COMM CONVERT
							//		//iClientReceivingOffset = CConvertMng::ConvertUINTToInt(uiPacketOffset);
							//		iClientReceivingOffset = uiPacketOffset;

							//		// for debug
							//		if ( iClientReceivingOffset < NULL || iClientReceivingOffset > (int)65535 )
							//		{
							//			TRACE("\r\n[TcpRadarLinkClientThreadFunc ����(TCP) �峭��? iClientReceivingOffset=%d", iClientReceivingOffset);
							//		}

							//		if ( iClientReceivingOffset < (int)sizeof(StMissionHeader) )
							//		{
							//			bReceiveStart = false;

							//			if ( iClientReceivingOffset != NULL )
							//			{
							//				// ���� �̵�
							//				// COMM CONVERT
							//				//memcpy(cGRCommObj->m_aucTcpClientRecvBuf_1, cGRCommObj->m_aucTcpClientRecvBuf_1+uiPacketStartOffset, CConvertMng::ConvertIntToUINT(iClientReceivingOffset) );
							//				memcpy(cGRCommObj->m_aucRadarLink_1RecvBuf, cGRCommObj->m_aucRadarLink_1RecvBuf+uiPacketStartOffset, iClientReceivingOffset );
							//				//TRACE("[TcpRadarLinkClientThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
							//			}
							//		}
							//		else
							//		{
							//			bReceiveStart = true;

							//			// ���� �̵�
							//			// COMM CONVERT
							//			//memcpy(cGRCommObj->m_aucTcpClientRecvBuf_1, cGRCommObj->m_aucTcpClientRecvBuf_1+uiPacketStartOffset, CConvertMng::ConvertIntToUINT(iClientReceivingOffset) );
							//			memcpy(cGRCommObj->m_aucRadarLink_1RecvBuf, cGRCommObj->m_aucRadarLink_1RecvBuf+uiPacketStartOffset, iClientReceivingOffset );
							//			//TRACE("[TcpRadarLinkClientThreadFunc �����̵� iPacketStartOffset = %d, iSize = %d]\n", iPacketStartOffset, iClientReceivingOffset);
							//		}
							//	}
							//	else
							//	{ //EX_DTEC_Else
							//		//DoNothing();
							//	}
							//}												
						}

					}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
				}
			}// for
		}//if(fdNum != 0) 
	}

	TRACE("[### TCP CLIENT GMI_1 THREAD END ####]\n");

	return NULL;
}

UINT CGRCommObj::TcpInterCommClientThreadFunc(LPVOID arg)
{
	TRACE("[### TCP INTERCOMM CLIENT THREAD START ####]\n");

	// TODO : ���� ������ ���� ������ static�� �ʿ�� ����	
	static int iClientReceivingOffset = 0;
	static StMissionPacket* pstClientPacket = NULL;
	static void *pvClientPacketAddress = NULL;

	fd_set stReads, stCopyReads;
	FD_ZERO(&stReads);
	SOCKET *pSocket = (SOCKET *)arg;
	SOCKET hClientSocket = *pSocket;
	FD_SET(hClientSocket, &stReads);

	int iFdNum, iLength;
	bool bConnected = true;

	BYTE RecvBuf[MAX_BUF_SIZE];
	memset(RecvBuf, NULL, MAX_BUF_SIZE);

	TIMEVAL stTimeout;
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second

	while(true == bConnected)
	{
		stCopyReads = stReads;
		iFdNum = select(0, &stCopyReads, 0, 0, &stTimeout);

		if(SOCKET_ERROR == iFdNum)  // -1 
		{
			TRACE("[### TCP INTERCOMM CLIENT THREAD SELECT ERROR ####]\n");
			break; // ����
		}
		else if(0 != iFdNum) // select timeout �� �ƴϸ� 
		{
			// ������ ���� �κ� 
			for(unsigned int i=0; i<stReads.fd_count; i++)
			{
				if(false == bConnected)
				{
					break;
				}

				if(FD_ISSET(stReads.fd_array[i], &stCopyReads))
				{
					if(stReads.fd_array[i] == hClientSocket)
					{
						iLength = recv(stReads.fd_array[i], (char *)RecvBuf, MAX_BUF_SIZE -1 , 0);

						//TRACE("TcpInterCommClientThreadFunc iLenght = %d\n", iLength);

						if(0 >= iLength) //  0 : �ݴ��� ������ close�� , -1 :error => �� ���� ���� ���� ��Ŵ 
						{
							// ����͸� ���� �������� ����
							FD_CLR(stReads.fd_array[i], &stReads);
							closesocket(stCopyReads.fd_array[i]);

							TRACE("TcpInterCommClientThreadFunc close\n");
							bConnected = false;
						}
						else
						{
							CGRCommObj& cGRCommObj = CGRCommObj::GetInstance();
							cGRCommObj.RecvPacketFromInterCommServer(RecvBuf, iLength);
						}

					}//if(FD_ISSET(reads.fd_array[i], &cpyReads))
				}
			}// for
		}//if(fdNum != 0) 
	}

	//CLog::printf_log(LOG_INFO, "@@@ TcpClientThreadFunc end (%d)", entry->getID());
	TRACE("[### TCP INTERCOMM CLIENT THREAD END ####]\n");

	return 0;
}

/**
* @brief TCP Packet ó�� Thread �Լ�
* @return ���� ���
*/

//void CGRCommObj::SetDlgPointer(CASAE_DEMONDlg* i_pDlg)
//{
//	m_pDlg = i_pDlg;
//}

UINT CGRCommObj::TcpPacketProcessingThreadFunc(LPVOID arg)
{
	// TODO : thread �Լ� classȭ �ʿ�
	TRACE("[### TCP PACKET PROC THREAD START ####]\n");

	// TODO : ���� ������ ���� ������ static�� �ʿ�� ����
	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();
	void *pvPacket = NULL;
	int iLength;
	unsigned int uiLoopCnt = NULL;

	list<StClientSocket*>::iterator iter;
	int iCnt = 0;

	CString str;

	SOCKADDR_IN addrUdpSock;
	SOCKET hUdpSocket;

	memset(&addrUdpSock, NULL, sizeof(SOCKADDR_IN));
	memcpy(&addrUdpSock, &cGRCommObj->GetUdpSocketAddr(), sizeof(SOCKADDR_IN));
	hUdpSocket = NULL;
	hUdpSocket = cGRCommObj->GetUdpSocket();	
	StMissionPacket* pstPacket;
	unsigned int uiOpcode = 0;
	unsigned int uiRegisterOpcode = 0;
	unsigned int uiRecvOpcode = 0;
	unsigned short usSize;
	unsigned char ucRevOperID;
	unsigned char ucLinkID;
	unsigned char ucOpertorID;
	void *pvData;
	CCommonMngr *pcMngr = nullptr;
	unsigned int uiInterval1 = NULL;
	unsigned int uiInterval2 = NULL;
	bool bRst = false;

	while(1)
	{
		bRst = cGRCommObj->DequeuePacket(&pvPacket, &iLength);

		if( bRst == true )
		{
			pstPacket = (StMissionPacket *)pvPacket;					
			memcpy(&uiOpcode, &(pstPacket->stMissionHeader.stOpcode), sizeof(unsigned int));
			usSize = pstPacket->stMissionHeader.usSize;

			ucRevOperID = pstPacket->stMissionHeader.ucRevOprID;
			ucLinkID = pstPacket->stMissionHeader.ucLinkID;
			ucOpertorID = pstPacket->stMissionHeader.iOperID;
			pvData = pstPacket->pcData;


			if ( cGRCommObj->GetSocketMode() == SOCKET_SERVER ) // Server Mode
			{
				//V/UHF, PDW�߻��ǿ� ������ ����
				if ( (uiOpcode == 0x00000000) || (pstPacket->stMissionHeader.stOpcode.ucSource == 0x00))	// Source�� 0x00�� ��� ����ó�� �߰�
				{
					if ( uiOpcode == 0x00000000 )
					{
						TRACE("[����! OPCODE�� NULL�̳�.. �ƽ���...]" );						
					}

					if ( pstPacket->stMissionHeader.stOpcode.ucSource == 0x00 )
					{
						TRACE( "[����! �ҽ��� NULL�̳� �̷� �� Ǯ ������... �ƽ���...]" );
					}
				}
				else
				{
					if ( (uiOpcode & 0xffff0000) == 0x40820000 )
					{
						// Step#2 : �Ŵ����� ����
						for( uiLoopCnt=0; uiLoopCnt< cGRCommObj->m_uiRegisteredOpcodeCount; uiLoopCnt++)
						{
							uiRegisterOpcode = cGRCommObj->m_auiOpcode[uiLoopCnt] & 0xffffff00;
							uiRecvOpcode = uiOpcode & 0xffffff00;

							if(uiRegisterOpcode == uiRecvOpcode)
							{
								if ( cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
								{
									pcMngr = cGRCommObj->m_apcMngr[uiLoopCnt];
									pcMngr->Receive(uiRecvOpcode, usSize, ucLinkID, ucRevOperID, ucOpertorID, pvData);	
								}						
							}
						}
					}

					if ( (uiOpcode & 0xffff0000) == 0xFF000000 )
					{
						for( uiLoopCnt=0; uiLoopCnt< cGRCommObj->m_uiRegisteredOpcodeCount; uiLoopCnt++)
						{
							uiRegisterOpcode = cGRCommObj->m_auiOpcode[uiLoopCnt] & 0xffffff00;
							uiRecvOpcode = uiOpcode & 0xffffff00;

							if(uiRegisterOpcode == uiRecvOpcode)
							{
								if ( cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
								{
									pcMngr = cGRCommObj->m_apcMngr[uiLoopCnt];
									pcMngr->Receive(uiRecvOpcode, usSize, ucLinkID, ucRevOperID, ucOpertorID, pvData);
								}						
							}
						}
					}
					//if ( (uiOpcode & 0x80000000) == NULL ) // ���ñ�->���ű�			
					//{
					//	ChangeEndianness(&(pstPacket->stMissionHeader), sizeof(StMissionHeader));
					//
					//	for(iter = cGRCommObj.m_pstClientList.begin(); iter != cGRCommObj.m_pstClientList.end(); ++iter)
					//	{
					//		pTempClient = *iter;

					//		if ( pTempClient->bDataLink == true )
					//		{
					//			// COMM CONVERT
					//			//iRet = send(pTempClient->hSocket, (const char *)pstPacket, (int)usSize + CConvertMng::ConvertUINTToInt(sizeof(StMissionHeader)), 0);
					//			iRet = send(pTempClient->hSocket, (const char *)pstPacket, usSize + sizeof(StMissionHeader), 0);
					//			if( SOCKET_ERROR == iRet)
					//			{
					//				TRACE("TCP_SEND_ERROR_SEND(bypass to DATALINK\n");
					//			}
					//		}
					//	}					
					//}
					//else //���ű�->���ñ�
					//{
					//	for( iter = cGRCommObj.m_pstClientList.begin(); iter != cGRCommObj.m_pstClientList.end(); iter++ )
					//	{
					//		pTempClient = *iter;
					//		if ( pTempClient->bDataLink == false )
					//		{
					//			if ( pTempClient->usOperatorID == ucOD )
					//			{
					//				uiInterval1 = GetTickCount();											
					//				// COMM CONVERT
					//				// iRet = send(pTempClient->hSocket, (const char *)cGRCommObj->m_aucTcpRecvBuf, (int)usSize + CConvertMng::ConvertUINTToInt(sizeof(StMissionHeader)), 0);
					//				iRet = send(pTempClient->hSocket, (const char *)cGRCommObj.m_aucTcpRecvBuf, usSize + sizeof(StMissionHeader), 0);
					//				uiInterval2 = GetTickCount();

					//				if( SOCKET_ERROR == iRet)
					//				{
					//					TRACE("TCP_SEND_ERROR(to POSN) ID:%d(SOCKET:%d), size:%d, interval:%d)\n", ucOD, pTempClient->hSocket, usSize, (uiInterval2 - uiInterval1));
					//					shutdown(pTempClient->hSocket, SD_SEND);
					//				}
					//				break;
					//			}
					//		}
					//	}								
					//}
				}
			}
			else //client Mode
			{
				for( uiLoopCnt=0; uiLoopCnt < cGRCommObj->m_uiRegisteredOpcodeCount; uiLoopCnt++)
				{
					uiRegisterOpcode = cGRCommObj->m_auiOpcode[uiLoopCnt] & 0xffffff00;
					uiRecvOpcode = uiOpcode & 0xffffff00;
					if(uiRegisterOpcode == uiRecvOpcode)
					{
						if ( cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
						{
							pcMngr = cGRCommObj->m_apcMngr[uiLoopCnt];
							pcMngr->Receive(uiRecvOpcode, usSize, ucLinkID, ucRevOperID, ucOpertorID, pvData);
						}					
					}
				}
			}
	
			delete pvPacket;		
			iCnt++;
		}
	}

	//CLog::printf_log(LOG_INFO, "@@@ TcpClientThreadFunc end (%d)", entry->getID());
	TRACE("[### TCP PACKET PROC THREAD END ####]\n");

	return 0;
}

UINT CGRCommObj::TcpPacketProcessingADSBThreadFunc(LPVOID arg)
{
	// TODO : thread �Լ� classȭ �ʿ�
	TRACE("[### TCP PACKET PROC THREAD START ####]\n");

	// TODO : ���� ������ ���� ������ static�� �ʿ�� ����
	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();
	void *pvPacket = NULL;
	int iLength;
	unsigned int uiLoopCnt = NULL;
	list<StClientSocket*>::iterator iter;
	int iCnt = 0;
	CString str;
	SOCKADDR_IN addrUdpSock;
	SOCKET hUdpSocket;

	memset(&addrUdpSock, NULL, sizeof(SOCKADDR_IN));
	memcpy(&addrUdpSock, &cGRCommObj->GetUdpSocketAddr(), sizeof(SOCKADDR_IN));
	hUdpSocket = NULL;
	hUdpSocket = cGRCommObj->GetUdpSocket();	
	StMissionPacket* pstPacket;
	unsigned int uiOpcode = 0;
	unsigned int uiRegisterOpcode = 0;
	unsigned int uiRecvOpcode = 0;
	unsigned short usSize;
	unsigned char ucRevOperID;
	unsigned char ucLinkID;
	unsigned char ucOpertorID;
	void *pvData;
	CCommonMngr *pcMngr = nullptr;
	unsigned int uiInterval1 = NULL;
	unsigned int uiInterval2 = NULL;
	bool bRst = false;

	while(1)
	{
		bRst = cGRCommObj->DequeuePacket(&pvPacket, &iLength);

		if( bRst == true )
		{
			pstPacket = (StMissionPacket *)pvPacket;					
			memcpy(&uiOpcode, &(pstPacket->stMissionHeader.stOpcode), sizeof(unsigned int));
			usSize = pstPacket->stMissionHeader.usSize;

			ucRevOperID = pstPacket->stMissionHeader.ucRevOprID;
			ucLinkID = pstPacket->stMissionHeader.ucLinkID;
			ucOpertorID = pstPacket->stMissionHeader.iOperID;
			pvData = pstPacket->pcData;


			if ( cGRCommObj->GetSocketMode() == SOCKET_SERVER ) // Server Mode
			{
				//V/UHF, PDW�߻��ǿ� ������ ����
				if ( (uiOpcode == 0x00000000) || (pstPacket->stMissionHeader.stOpcode.ucSource == 0x00))	// Source�� 0x00�� ��� ����ó�� �߰�
				{
					if ( uiOpcode == 0x00000000 )
					{
						TRACE("[����! OPCODE�� NULL�̳�.. �ƽ���...]" );						
					}

					if ( pstPacket->stMissionHeader.stOpcode.ucSource == 0x00 )
					{
						TRACE( "[����! �ҽ��� NULL�̳� �̷� �� Ǯ ������... �ƽ���...]" );
					}
				}
				else
				{
					if ( (uiOpcode & 0xffff0000) == 0x40820000 )
					{
						// Step#2 : �Ŵ����� ����
						for( uiLoopCnt=0; uiLoopCnt< cGRCommObj->m_uiRegisteredOpcodeCount; uiLoopCnt++)
						{
							uiRegisterOpcode = cGRCommObj->m_auiOpcode[uiLoopCnt] & 0xffffff00;
							uiRecvOpcode = uiOpcode & 0xffffff00;

							if(uiRegisterOpcode == uiRecvOpcode)
							{
								if ( cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
								{
									pcMngr = cGRCommObj->m_apcMngr[uiLoopCnt];
									pcMngr->Receive(uiRecvOpcode, usSize, ucLinkID, ucRevOperID, ucOpertorID, pvData);
								}						
							}
						}
					}
					//if ( (uiOpcode & 0x80000000) == NULL ) // ���ñ�->���ű�			
					//{
					//	ChangeEndianness(&(pstPacket->stMissionHeader), sizeof(StMissionHeader));
					//
					//	for(iter = cGRCommObj.m_pstClientList.begin(); iter != cGRCommObj.m_pstClientList.end(); ++iter)
					//	{
					//		pTempClient = *iter;

					//		if ( pTempClient->bDataLink == true )
					//		{
					//			// COMM CONVERT
					//			//iRet = send(pTempClient->hSocket, (const char *)pstPacket, (int)usSize + CConvertMng::ConvertUINTToInt(sizeof(StMissionHeader)), 0);
					//			iRet = send(pTempClient->hSocket, (const char *)pstPacket, usSize + sizeof(StMissionHeader), 0);
					//			if( SOCKET_ERROR == iRet)
					//			{
					//				TRACE("TCP_SEND_ERROR_SEND(bypass to DATALINK\n");
					//			}
					//		}
					//	}					
					//}
					//else //���ű�->���ñ�
					//{
					//	for( iter = cGRCommObj.m_pstClientList.begin(); iter != cGRCommObj.m_pstClientList.end(); iter++ )
					//	{
					//		pTempClient = *iter;
					//		if ( pTempClient->bDataLink == false )
					//		{
					//			if ( pTempClient->usOperatorID == ucOD )
					//			{
					//				uiInterval1 = GetTickCount();											
					//				// COMM CONVERT
					//				// iRet = send(pTempClient->hSocket, (const char *)cGRCommObj->m_aucTcpRecvBuf, (int)usSize + CConvertMng::ConvertUINTToInt(sizeof(StMissionHeader)), 0);
					//				iRet = send(pTempClient->hSocket, (const char *)cGRCommObj.m_aucTcpRecvBuf, usSize + sizeof(StMissionHeader), 0);
					//				uiInterval2 = GetTickCount();

					//				if( SOCKET_ERROR == iRet)
					//				{
					//					TRACE("TCP_SEND_ERROR(to POSN) ID:%d(SOCKET:%d), size:%d, interval:%d)\n", ucOD, pTempClient->hSocket, usSize, (uiInterval2 - uiInterval1));
					//					shutdown(pTempClient->hSocket, SD_SEND);
					//				}
					//				break;
					//			}
					//		}
					//	}								
					//}
				}
			}
			else //client Mode
			{
				for( uiLoopCnt=0; uiLoopCnt < cGRCommObj->m_uiRegisteredOpcodeCount; uiLoopCnt++)
				{
					uiRegisterOpcode = cGRCommObj->m_auiOpcode[uiLoopCnt] & 0xffffff00;
					uiRecvOpcode = uiOpcode & 0xffffff00;
					if(uiRegisterOpcode == uiRecvOpcode)
					{
						if ( cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
						{
							pcMngr = cGRCommObj->m_apcMngr[uiLoopCnt];
							pcMngr->Receive(uiRecvOpcode, usSize, ucLinkID, ucRevOperID, ucOpertorID, pvData);
						}					
					}
				}
			}

			//delete pvPacket;		
			iCnt++;
		}
	}

	//CLog::printf_log(LOG_INFO, "@@@ TcpClientThreadFunc end (%d)", entry->getID());
	TRACE("[### TCP PACKET PROC THREAD END ####]\n");

	return 0;
}

UINT CGRCommObj::TcpPacketProcessingThreadFuncForSend(LPVOID arg)
{
	TRACE("[### TCP PACKET PROC THREAD FOR SEND START ####]\n");

	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();

	void *pvPacket = NULL;
	int iLength = NULL;
	unsigned int uiLoopCnt = NULL;
	SOCKET hRadarLink1ClientSocket = NULL;
	StMissionOperPacket* pstPacket = nullptr;
	StMissionOperHeader stMHeader = StMissionOperHeader();
	unsigned short usSize = NULL;
	unsigned int uiOpcode = NULL;
	unsigned int uiRegisterOpcode = NULL;
	unsigned int uiRecvOpcode = NULL;
	unsigned short usOperatorID = NULL;
	unsigned char ucRevOperID = NULL;
	unsigned char ucLinkID = NULL;
	unsigned char ucOpertorID = NULL;
	CCommonMngr *pcMngr = nullptr;
	void *pvData = nullptr;
	bool bRst = false;
	int iPeriod = NULL;
	char cSysType = 0;

	while ( cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
	{
		bRst = cGRCommObj->DequeuePacketForSend(&pvPacket, &iLength);

		if( bRst == true )
		{ 
			
			pstPacket = (StMissionOperPacket *)pvPacket;

			memcpy(&uiOpcode, &(pstPacket->stMissionHeader.stOpcode), sizeof(unsigned int));
			usSize = pstPacket->stMissionHeader.usSize;

			ucRevOperID = pstPacket->stMissionHeader.ucRevOprID;
			ucLinkID = pstPacket->stMissionHeader.ucLinkID;
			ucOpertorID = pstPacket->stMissionHeader.iOperID;
			pvData = pstPacket->pcData;
			//usOperatorID = pstPacket->stMissionHeader.usOperatorID;
								
			//if ( (uiOpcode & 0xffff0000) == 0x82400000 ) //������ ���̴��м� ��� �޴����� ���Ѵ�.
			{
				// Step#2 : �Ŵ����� ����
				for( uiLoopCnt=0; uiLoopCnt< cGRCommObj->m_uiRegisteredOpcodeCount; uiLoopCnt++)
				{
					uiRegisterOpcode = cGRCommObj->m_auiOpcode[uiLoopCnt] & 0xffffff00;
					uiRecvOpcode = uiOpcode & 0xffffff00;

					if(uiRegisterOpcode == uiRecvOpcode)
					{
						if ( cGRCommObj->m_bPacketProcessingThreadEndFlag == false )
						{
							pcMngr = cGRCommObj->m_apcMngr[uiLoopCnt];
							pcMngr->Receive(uiRecvOpcode, usSize, ucLinkID, ucRevOperID, ucOpertorID, pvData);
						}						
					}
				}
			}
			//else
			//{
			//	memcpy(&stMHeader, pstPacket, sizeof(StMissionHeader));
			//	TRACE("[����<fromPOSN>(OperatorID = %d) M-Size = %d, Source = 0x%02x, Destination = 0x%02x, Cmd Code = 0x%02x\r\n", stMHeader.ucSourcdeOprID, stMHeader.usSize, stMHeader.stOpcode.ucSource, stMHeader.stOpcode.ucDestination, stMHeader.stOpcode.ucCommandCode);

			//	ChangeEndianness(&(pstPacket->stMissionHeader), sizeof(StMissionHeader));

			//	hRadarLink1ClientSocket = cGRCommObj->GetRadarLinkClientSocket();				

			//	//server�� ����
			//	cGRCommObj->SendToLink(hRadarLink1ClientSocket, (const char *)pstPacket, (int)usSize + sizeof(StMissionHeader), uiOpcode);								

			//	delete pvPacket;
			//}
			delete pvPacket;
		}
	}

	TRACE("[### TCP PACKET PROC THREAD FOR SEND END ####]\n");

	return NULL;
}

UINT CGRCommObj::RadarLinkPacketProcessingThreadFunc(LPVOID arg)
{
	TRACE("[### LINK PACKET PROCESSING THREAD START ####]\n");

	CGRCommObj* cGRCommObj = &CGRCommObj::GetInstance();

	DWORD dwWait;	
	void *pvPacket = NULL;
	int iLength = NULL;
	int iType = NULL;
	unsigned short usFrameCnt = NULL;
	bool bRecvQueueEmpty = false;
	bool bCheckRst = false;
	int iEncMode = NULL;

	while(cGRCommObj->m_bPacketProcessingThreadEndFlag == false)
	{
		dwWait = WaitForSingleObject(cGRCommObj->m_hEncEvent, COMM_TIMEOUT);

		if ( dwWait == WAIT_OBJECT_0 )
		{
			EnterCriticalSection( &cs_GRCommLinkRecvQueue);
			bRecvQueueEmpty = cGRCommObj->m_pstRecvPacketInfoQueue.empty();
			LeaveCriticalSection( &cs_GRCommLinkRecvQueue);

			while ( bRecvQueueEmpty == false )
			{				
				EnterCriticalSection( &cs_GRCommLinkRecvQueue);
				cGRCommObj->DequeueRecvPacketInfo(&pvPacket, &iLength, &iType);
				LeaveCriticalSection( &cs_GRCommLinkRecvQueue);					

				// ���ȣȭ - �ٷ� ó������
				if ( iType == PACKET_TYPE_SEND ) // �۽�
				{
					cGRCommObj->SendPacketFrameToLink((BYTE*)pvPacket, (unsigned short)iLength);
				}
				else // ����
				{
					cGRCommObj->RecvPacketFromDataLink((BYTE*)pvPacket, iLength);
				}

				// �޸� ����
				delete [] pvPacket;
								
				///////////////////////////////////////////////////////////////////////////////////////////////

				EnterCriticalSection( &cs_GRCommLinkRecvQueue);
				bRecvQueueEmpty = cGRCommObj->m_pstRecvPacketInfoQueue.empty();
				LeaveCriticalSection( &cs_GRCommLinkRecvQueue);
			} // while
		} // if
	} // while

	TRACE("[### LINK PACKET PROCESSING THREAD END ####]\n");
	return NULL;
}


void CGRCommObj::SetUdpSocket(SOCKET i_hSocket)
{
	m_hUdpSocket = i_hSocket;	
}

SOCKET CGRCommObj::GetUdpSocket()
{
	return m_hUdpSocket;
}

void CGRCommObj::SetUdpSocketAddr(SOCKADDR_IN i_addrSocket)
{
	memcpy(&m_addrUdpSocket, &i_addrSocket, sizeof(SOCKADDR_IN));
}

SOCKADDR_IN CGRCommObj::GetUdpSocketAddr()
{
	return m_addrUdpSocket;
}

/*
UINT		LSize               : 23;	// ��ü ũ��
UINT		F					: 1	;	// Frame ���� ����
UINT		R					: 1	;	// ������ �ʿ� ����
UINT		A					: 1	;	// ACK ����
UINT		C					: 1	;	// ���� ����
UINT		E					: 1	;	// ��ȣȭ ����
UINT		ACID				: 4	;	// �װ��� �ĺ� ��ȣ
*/
int CGRCommObj::SendToLink(SOCKET i_hSocket, const char* i_pcBuf, int i_iSize, unsigned int i_uiOpcode)
{
	EnterCriticalSection( &cs_GRCommLinkSend);

	// COMM CONVERT
	//char* pvData = new char[CConvertMng::ConvertIntToUINT(i_iSize)];
	char* pvData = new char[i_iSize];
	// memcpy(pvData, i_pcBuf, CConvertMng::ConvertIntToUINT(i_iSize));
	memcpy(pvData, i_pcBuf, i_iSize);
	EnqueueSendPacketInfo((void*)pvData, i_iSize);

	LeaveCriticalSection( &cs_GRCommLinkSend);

	return 1;

	/*EnterCriticalSection( &cs_GRCommLinkSend);
	int iRet = 0;

	iRet = send(i_hSocket, (const char*)i_pcBuf, i_iSize, 0);

	if( SOCKET_ERROR == iRet)
	{
	TRACE("DATA LINK SEND ERROR\n");
	}

	LeaveCriticalSection( &cs_GRCommLinkSend);
	return iRet;*/

	//QueryPerformanceCounter(&liCounter1);     // Start
	//TRACE("==START in SendToLink==(Counter=%d)\n", liCounter1.QuadPart);	
	//int iRet = 0;
	//bool bRetry = false;

	//// STEP#1 : ����
	//ULONG ulResultLen;
	//UCHAR* ucResultBuffer;
	//ucResultBuffer = m_ComplibFacade.inflate( &ulResultLen, (UCHAR *)i_pcBuf+sizeof(StMissionHeader),  i_iSize-sizeof(StMissionHeader) );	
	//memcpy(m_aucSendBuf+sizeof(stLHdr1Frame)+sizeof(StMissionHeader), ucResultBuffer, ulResultLen); // ����� ������ ����

	//// STEP#2 : LINK Header �߰�
	//stLHdr1Frame stLinkHeader;	
	//stLinkHeader.LHdrFlag.stData.ACID = AIRCRAFT_ID_1; // 1ȣ��. ���� ���ó���⿡������ 1/2ȣ�⸦ ������ �� ����
	//stLinkHeader.LHdrFlag.stData.E = FLAG_ENCRY_NO; // ���ȣȭ : ���� ��ȣ��� ���� �Ұ�
	//stLinkHeader.LHdrFlag.stData.C = FLAG_COMP_YES; // Ư���޽����� �������� ����
	//stLinkHeader.LHdrFlag.stData.A = FLAG_ACK_NO; // �۽� �޽���
	//
	//if ( i_uiOpcode == RETRY_NO_OPCODE_GR_OPERATOR_AUDIO ) // ����� ���������� ��ȣȭ ���� ����
	//{
	//	stLinkHeader.LHdrFlag.stData.R = FLAG_RETRY_NO;		
	//}
	//else
	//{
	//	stLinkHeader.LHdrFlag.stData.R = FLAG_RETRY_YES;
	//	bRetry = true;
	//}
	//
	//stLinkHeader.LHdrFlag.stData.F = FLAG_FRAME_NO; // ������ ������ ����	
	//stLinkHeader.LHdrFlag.stData.LSize = ulResultLen + sizeof(StMissionHeader) + sizeof(stLHdr1Frame) - sizeof(stLHdrFlag);	
	//stLinkHeader.LHdrFlag.uiData = htonl(stLinkHeader.LHdrFlag.uiData);
	//stLinkHeader.uiSeqNum = htonl(m_uiLinkSendSeqNo);

	//memcpy(m_aucSendBuf, &stLinkHeader, sizeof(stLHdr1Frame)); // ��ũ��� ����
	//memcpy(m_aucSendBuf+sizeof(stLHdr1Frame), i_pcBuf, sizeof(StMissionHeader)); //�ӹ���� ����
	//
	//// STEP#3 : ����
	//iRet = send(i_hSocket, (const char*)m_aucSendBuf, ulResultLen+sizeof(stLHdr1Frame)+sizeof(StMissionHeader), 0);

	//if( SOCKET_ERROR == iRet)
	//{
	//	TRACE("DATA LINK SEND ERROR\n");
	//}
	//else
	//{
	//	//TRACE("[Send Data (SeqNo : %d, size : %d) to Data Link\n", htonl(stLinkHeader.uiSeqNum), ulResultLen+sizeof(stLHdr1Frame)+sizeof(StMissionHeader));		
	//	
	//	if ( bRetry == true )
	//	{
	//		if ( i_uiOpcode != LINK_RETRY_CODE ) // �ű�����
	//		{
	//			memcpy(&m_stMissionPacketForResend, i_pcBuf, i_iSize);
	//			m_uiMissionPacketSizeForResend = i_iSize;
	//			StartTimer(m_uiLinkSendSeqNo);
	//		}
	//	}
	//	else
	//	{
	//		m_uiLinkSendSeqNo++;
	//	}
	//}

	//QueryPerformanceCounter(&liCounter2);      // End
	//TRACE("==END in SendToLink==(Counter=%d)\n", liCounter2.QuadPart);
	//TRACE("==Send Time = %f sec==\n",(double)(liCounter2.QuadPart - liCounter1.QuadPart) / (double)liFrequency.QuadPart);

	
}

void CGRCommObj::RecvPacketFromInterCommServer(BYTE* i_pucData, int i_iSize)
{
	/*
	int i;
	CCommonMngr *pcMngr = NULL;

	// Step#1 : ����
	for( i=0; i< m_uiRegisteredOpcodeCount; i++)
	{
		if( m_auiOpcode[i] == INTER_COMM_CODE )
		{
			pcMngr = m_apcMngr[i];					
			pcMngr->Receive(INTER_COMM_CODE, i_iSize, 0, i_pucData, true);
		}
	}
	*/
}

void CGRCommObj::RecvPacketFromDataLink1Frame(BYTE* i_pucData, int i_iSize)
{
	static StMissionPacket* pstClientPacket = NULL;
	static void *pvClientPacketAddress = NULL;
	ULONG ulResultLen;
	UCHAR* ucResultBuffer;

	stLHdr1Frame stLinkHeader;
	memcpy(&stLinkHeader, i_pucData, sizeof(stLHdr1Frame));
	stLinkHeader.LHdrFlag.uiData = htonl(stLinkHeader.LHdrFlag.uiData);

	/*
	StMissionHeader stMHeader;
	memcpy(&stMHeader, i_pucData + sizeof(stLHdr1Frame),sizeof(StMissionHeader));
	ChangeEndianness(&stMHeader, sizeof(StMissionHeader));
	TRACE("L-Size = %d, M-Size = %d\n", m_stLinkHeader1Frame.LHdrFlag.stData.LSize, stMHeader.usSize);
	*/

	if ( stLinkHeader.LHdrFlag.stData.A == FLAG_ACK_NO ) 
	{
		// �װ��� ACK ����
		SendAckToDataLink(FLAG_FRAME_NO, i_pucData);

		// �װ���� ����
		if ( stLinkHeader.LHdrFlag.stData.C == FLAG_COMP_YES )
		{
			// ����
			ucResultBuffer = m_ComplibFacade.deflate(&ulResultLen, (UCHAR *)i_pucData+sizeof(stLHdr1Frame)+sizeof(StMissionHeader),  i_iSize-sizeof(stLHdr1Frame)-sizeof(StMissionHeader) );
			memcpy(m_aucCompRecvBuf+sizeof(StMissionHeader), ucResultBuffer, ulResultLen); // ������ ������ ����			
		}
		else
		{
			ulResultLen = i_iSize-sizeof(stLHdr1Frame)-sizeof(StMissionHeader);
			memcpy(m_aucCompRecvBuf+sizeof(StMissionHeader), i_pucData+sizeof(stLHdr1Frame)+sizeof(StMissionHeader), ulResultLen); // ������ ������ ����
		}
		memcpy(m_aucCompRecvBuf, i_pucData+sizeof(stLHdr1Frame), sizeof(StMissionHeader));

		pstClientPacket = new struct StMissionPacket;
		pvClientPacketAddress = pstClientPacket;

		memcpy(pvClientPacketAddress, m_aucCompRecvBuf, sizeof(StMissionHeader)+ulResultLen);
		ChangeEndianness(&(pstClientPacket->stMissionHeader), sizeof(StMissionHeader));
		if ( false == EnqueuePacket(pvClientPacketAddress, pstClientPacket->stMissionHeader.usSize + sizeof(StMissionHeader)) )
		{
			delete pstClientPacket;
		}
	}
	else // ������� ���� ACK
	{
		StopTimer();
		//TRACE("RetryTimer Stop!(ACK �޽��� ����)\n");
	}			
}

// �װ��� ACK ����
int CGRCommObj::SendAckToDataLink(int i_iFrameType, BYTE* i_pucData)
{
	int iRet=0;

	if ( i_iFrameType == FLAG_FRAME_YES )
	{
		stLHdrNFrame stLinkNHeader;
		memcpy(&stLinkNHeader, i_pucData, sizeof(stLHdrNFrame));
		stLinkNHeader.LHdrFlag.uiData = htonl(stLinkNHeader.LHdrFlag.uiData);

		stLinkNHeader.LHdrFlag.stData.A = FLAG_ACK_YES;
		stLinkNHeader.LHdrFlag.stData.F = FLAG_FRAME_YES;
		stLinkNHeader.LHdrFlag.stData.LSize = sizeof(stLHdrNFrame) - sizeof(stLHdrFlag);

		stLinkNHeader.LHdrFlag.uiData = htonl(stLinkNHeader.LHdrFlag.uiData);

		//iRet = send(m_hDataLinkClientSocket, (const char*)&stLinkNHeader, sizeof(stLHdrNFrame), 0);
				
		/*
		if( SOCKET_ERROR == iRet)
			TRACE("DATA LINK SEND(ACK) ERROR\n");
		else
			TRACE("[Send Ack (SeqNo : %d, TotalFrameNo : %d, CurrentFrameNo : %d) to Data Link\n", htonl(stLinkNHeader.uiSeqNum), htons(stLinkNHeader.usTotalFrameCount), htons(stLinkNHeader.usFrameNum) );
		*/
	}
	else
	{
		stLHdr1Frame stLink1Header;
		memcpy(&stLink1Header, i_pucData, sizeof(stLHdr1Frame));
		stLink1Header.LHdrFlag.uiData = htonl(stLink1Header.LHdrFlag.uiData);

		stLink1Header.LHdrFlag.stData.A = FLAG_ACK_YES;
		stLink1Header.LHdrFlag.stData.F = FLAG_FRAME_NO;		
		stLink1Header.LHdrFlag.stData.LSize = sizeof(stLHdr1Frame) - sizeof(stLHdrFlag);
		
		stLink1Header.LHdrFlag.uiData = htonl(stLink1Header.LHdrFlag.uiData);

		//iRet = send(m_hDataLinkClientSocket, (const char*)&stLink1Header, sizeof(stLHdr1Frame), 0);
				
		/*
		if( SOCKET_ERROR == iRet)
			TRACE("DATA LINK SEND(ACK) ERROR\n");
		else
			TRACE("[Send Ack (SeqNo : %d) to Data Link\n", htonl(stLink1Header.uiSeqNum) );		
		*/
	}

	return iRet;
}

// �ٽ�SW �ҿ�����
// 
// ����ü�� S/W ǰ�������� ���� ���� �ŷڼ� ���� ���� ����
// �����, �躸��
// Parsing, ����ȭ, �߼ұ�� �ı�ȿ��, �پ��� ����, Ȯ�强
// 4�� 24��
// 
// ���α� Ž���� ���� �н��� ����м� S/W
// ����ȣ, ����ö
// �����ν�, �����ν�, �н�(�Ű��&�����ڸ�), GPU�� �̿��� ���� ����ó��, Ŭ����
// 3�� 20��
// 
// ��뷮 ��Ž�ȣ���� ����ó�� S/W
// �̱���, ������
// CEP/CPL, FFT, ����/���ڵ�, �޸�DB, �м�����, GPU, Ŭ����, �ؽǽð�
// 4��30��
// 
// ��뷮 ���ڽ�ȣ���� ����ó�� S/W
// �ڰ���, ������
// CEP/CPL, PDW, ����/��Ʈ�� �м�, �޸�DB, �м�����, GPU, Ŭ����, �ؽǽð�
// 4��30��
void CGRCommObj::RecvPacketFromDataLinkNFrame(BYTE* i_pucData, int i_iSize)
{	
	stLHdrNFrame stLinkHeader;
	memcpy(&stLinkHeader, i_pucData, sizeof(stLHdrNFrame));
	stLinkHeader.LHdrFlag.uiData = htonl(stLinkHeader.LHdrFlag.uiData);
	stLinkHeader.usFrameNum = htons(stLinkHeader.usFrameNum);
	stLinkHeader.usTotalFrameCount = htons(stLinkHeader.usTotalFrameCount);

	// Variable
	// stLinkHeader.usFrameNum;
	// stLinkHeader.usTotalFrameCount;
	// m_aucNFrameBuf
	// m_uiNFrameBufOffset;
	// m_aucNFrameBuf, m_uiNFrameBufOffset;

	if ( stLinkHeader.usFrameNum == START_FRAME_INDEX )
	{
		memcpy(m_aucNFrameBuf, i_pucData, i_iSize);
	}

	// ������ ������
	if ( stLinkHeader.usFrameNum == stLinkHeader.usTotalFrameCount )
	{
		if ( stLinkHeader.LHdrFlag.stData.A == FLAG_ACK_NO ) 
		{
			// �װ��� ACK ����
			SendAckToDataLink(FLAG_FRAME_YES, i_pucData);
		}
	}

	// ���� ������ ��� M-SIZE�� ��� �Ǵ°���? ���ҵǱ� �� SIZE �ΰ�?
}

int CGRCommObj::RecvFromLink(BYTE* i_pucData, int i_iSize)
{
	// STX, CRC, SIZE ��ȿ�� üũ�� ���⿡�� �߰��� �ʿ䰡 �ִ�
	BYTE* pvData = new BYTE[i_iSize];
	memcpy(pvData, i_pucData, i_iSize);

	EnterCriticalSection( &cs_GRCommLinkRecvQueue);
	EnqueueRecvPacketInfo((void*)pvData, i_iSize, PACKET_TYPE_RECV);
	LeaveCriticalSection( &cs_GRCommLinkRecvQueue);

	SetEvent(m_hEncEvent);

	return 1;
}

void CGRCommObj::RecvPacketFromDataLink(BYTE* i_pucData, int i_iSize)
{
	StMissionPacket* pstClientPacket = nullptr;
	void *pvClientPacketAddress = nullptr;

	pstClientPacket = new struct StMissionPacket;
	pvClientPacketAddress = pstClientPacket;

	memcpy(pvClientPacketAddress, i_pucData, i_iSize);

	if ( EnqueuePacket(pvClientPacketAddress, (int)(pstClientPacket->stMissionHeader.usSize + sizeof(StMissionHeader)) ) == false )
	{
		delete pstClientPacket;
		pstClientPacket = NULL;
	}

	//stLHdr1Frame stLinkHeader;
	//memcpy(&stLinkHeader, i_pucData, sizeof(stLHdr1Frame));
	//stLinkHeader.LHdrFlag.uiData = htonl(stLinkHeader.LHdrFlag.uiData);
	//
	//// ��Ŷ ���� ���� Ȯ��
	//if ( stLinkHeader.LHdrFlag.stData.F == FLAG_FRAME_YES )
	//	RecvPacketFromDataLinkNFrame(i_pucData, i_iSize);
	//else
	//	RecvPacketFromDataLink1Frame(i_pucData, i_iSize);
}

void CALLBACK GRCommObjTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	if(lpParam != NULL)
	{
		CGRCommObj* pThis = ((CGRCommObj*)lpParam);	
		if ( pThis->GetPacketProcessingThreadEndFlag() == false )
		{
			pThis->GenerateSendPacketFrame();
		}
	}
	
	//CGRCommObj* pThis = ((CGRCommObj*)lpParam);
	//pThis->m_iRetryCnt++;
	//	
	//if ( pThis->m_uiCurrentTimerNo != NULL )
	//{		
	//	int iRet;
	//	//iRet = pThis->SendToLink(pThis->m_hDataLinkClientSocket, (const char*)&pThis->m_stMissionPacketForResend, pThis->m_uiMissionPacketSizeForResend, LINK_RETRY_CODE);
	//	if ( iRet == SOCKET_ERROR )
	//		TRACE("[������� ������ ����(%d)]\n", pThis->m_iRetryCnt);
	//	else
	//		TRACE("[������� ������ �Ϸ�(%d)]\n", pThis->m_iRetryCnt);

	//	if ( pThis->m_iRetryCnt >= MAX_RETRY_CNT )
	//	{
	//		pThis->StopTimer();
	//		TRACE("[RetryTimer Stop!(������� �ִ� ���� Ƚ�� �ʰ�)\n", pThis->m_iRetryCnt);
	//	}		
	//}	
}

void CGRCommObj::InsertUnRecvMsgSeqNo(unsigned int i_uiSeqNo)
{	
	m_listUnRecvMsg.push_back(i_uiSeqNo);
	m_uiUnRecvMsgCnt++;
}

void CGRCommObj::InsertUnSendMsgSeqNo(unsigned int i_uiSeqNo)
{
	m_listUnSendMsg.push_back(i_uiSeqNo);
	m_uiUnSendMsgCnt++;
}

void CGRCommObj::RecvUnRecvMsg(unsigned int i_uiSeqNo)
{	
	for(std::list<int>::iterator it = m_listUnRecvMsg.begin(); it != m_listUnRecvMsg.end(); it++)
	{
		if ( (*it) == i_uiSeqNo )
		{
			m_listUnRecvMsg.erase(it++);
			m_uiUnRecvMsgCnt--; 
			break;
		}
	}
}

// �װ����� ���Ź��� SeqNo�� ���Ӽ��� �������� �̼��� �޽����� �ĺ��ϰ�, list�� �����Ѵ�
void CGRCommObj::UpdateRecvMsgInfo(unsigned int i_uiSeqNo)
{	
	if ( i_uiSeqNo != NULL )
	{
		if ( (m_uiLinkRecvSeqNo+1) == i_uiSeqNo )
		{
			m_uiLinkRecvSeqNo = i_uiSeqNo;
		}
		else if ( (m_uiLinkRecvSeqNo+1) < i_uiSeqNo )
		{			
			for ( int i = (m_uiLinkRecvSeqNo+1); i < i_uiSeqNo; i++ )
			{
				InsertUnRecvMsgSeqNo(i);
				m_uiUnRecvMsgCnt++;
			}
		}
		else
		{
			TRACE("[�峭 �� ������?]\n");
		}
	}

	m_uiLinkRecvSeqNo = i_uiSeqNo;
	m_uiTotalRecvMsgCnt = m_uiLinkRecvSeqNo; 
}

// ������ Ÿ�̸Ӱ� 3ȸ �������� ����� LinkSendSeqNo�� �̼۽� �޽����� �ĺ��Ѵ�.
void CGRCommObj::UpdateSendMsgInfo(unsigned int i_uiSeqNo, bool i_bRecvAck)
{	
	if ( i_bRecvAck ) // Ack ���Ž� ��ü �۽� �޽��� ����
	{
		m_uiTotalSendMsgCnt = i_uiSeqNo;
	}
	else // Ack �����Ž� ���۽� �޽��� ����
	{
		InsertUnSendMsgSeqNo(i_uiSeqNo);
		m_uiUnSendMsgCnt++;
	}
}

// ������ DB ���� ä���� ����
// 3�ʸ��� �޽��� �ۼ��� ��Ȳ�� DB�� ����Ѵ�.
// STL List�� �̼��� �޽��� ����� ����صд�
// ���Ž� �̼��� ����Ʈ�� �ִ� ����� ���Ͽ� ���ŵ� ������ �����Ѵ�.
bool CGRCommObj::GetConnectionInfo(int i_iConnectionType)
{
	bool bRst = false;

	if ( i_iConnectionType == CLIENT_NO_1 )
	{
		bRst = m_bHeartbeatConnection_1;
	}
	
	if ( i_iConnectionType == CLIENT_NO_2 )
	{
		bRst = m_bHeartbeatConnection_2;
	}

	if ( i_iConnectionType == CLIENT_NO_3 )
	{
		bRst = m_bHeartbeatConnection_3;
	}

	if ( i_iConnectionType == CLIENT_NO_4 )
	{
		bRst = m_bHeartbeatConnection_4;
	}

	return bRst;
}

void CGRCommObj::SetConnectionInfo(int i_iConnectionType, bool i_bConnection)
{
	char readBuf[100] = {0};
	char *envini_path = ("..\\ICAA\\config.ini");
	CString	strPort = _T("");
	int iPort = NULL;
	CString strGrpName = _T("");

	if ( i_iConnectionType == CLIENT_NO_1 )
	{
		m_bHeartbeatConnection_1 = i_bConnection;
	}
	else if(  i_iConnectionType == CLIENT_NO_2 )
	{
		m_bHeartbeatConnection_2 = i_bConnection;
	}
	else if(  i_iConnectionType == CLIENT_NO_3 )
	{
		m_bHeartbeatConnection_3 = i_bConnection;
	}
	else if(  i_iConnectionType == CLIENT_NO_4 )
	{
		m_bHeartbeatConnection_4 = i_bConnection;
	}
	else;
	

	if ( m_iServerID == SERVER_ID_LINK_1 )
	{
		if ( i_iConnectionType == CLIENT_NO_1 )
		{
			strGrpName.Format("ADSBD");
		}		
	}	
	else 
	{
		if ( i_iConnectionType == CLIENT_NO_1 )
		{
			strGrpName.Format("DF_1");
		}
		else if( i_iConnectionType == CLIENT_NO_2 )
		{
			strGrpName.Format("PDW_2");
		}
		else if( i_iConnectionType == CLIENT_NO_3 )
		{
			strGrpName.Format("RDUnit_3");
		}
		else if( i_iConnectionType == CLIENT_NO_4 )
		{
			strGrpName.Format("VUHFUnit_4");
		}
		else;
	} 

	GetPrivateProfileString(strGrpName, ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
	strPort.Format(("%s"), readBuf);
	iPort = _ttoi(strPort);

	GetPrivateProfileString(strGrpName, ("IP"), NULL, readBuf, _countof(readBuf), envini_path);				

	if ( i_bConnection == true )
	{		
		TRACE ("[PORT : %d,  ���Ἲ��]\n", iPort);
	}
	else
	{
		TRACE ("[PORT : %d,  ������� �Ǵ� ����]\n", iPort);
	}
}

void CGRCommObj::FinishPacketProcessingThread()
{
	//StopTimer();
	m_bPacketProcessingThreadEndFlag = true;
}

bool CGRCommObj::GetInfoToUseHeartbeat()
{
	return m_bUseHeartbeat;
}

void CGRCommObj::LoadHeartbeatInfo()
{
	// heartbeat ��� ����
	int iInfo = NULL;
	CString strInfo = _T("");
	char readBuf[100] = {0};
	char *envini_path = (".\\..\\COMMON\\Config.ini");

	// HEART_BEAT_NO_USE
	GetPrivateProfileString(("HEART_BEAT"), ("HEART_BEAT_NO_USE"), NULL, readBuf, _countof(readBuf), envini_path);
	strInfo.Format(("%s"), readBuf);
	iInfo = _ttoi(strInfo);

	// Default : 0 => ���
	if ( iInfo != NULL )
	{
		m_bUseHeartbeat = false;
	}
	else
	{
		m_bUseHeartbeat = true;
	}
}

void CGRCommObj::SetServerID(int i_iServerID)
{
	m_iServerID = i_iServerID;
	/*if ( m_bPacketProcessingThreadEndFlag == false )
	{
	GP_MGR_REPLAY->SetServerID(i_iServerID);
	}*/
}

int CGRCommObj::GetServerID()
{
	return m_iServerID;
}

bool CGRCommObj::GetPDWConnStatus()
{
	return m_bHeartbeatConnection_2;
}

bool CGRCommObj::EnqueuePacketForSend(void* i_pvPacket, int i_iLength)
{
	bool bRst = true;
	DWORD dRet = NULL;

	dRet = WaitForSingleObject(m_hEnqueueSemaphoreForSend, COMM_TIMEOUT);
	if (WAIT_OBJECT_0 == dRet)
	{
		StMissionPacketContainer *stPacketContainer = new StMissionPacketContainer;
		stPacketContainer->pvPacket = i_pvPacket;
		stPacketContainer->iLength = i_iLength;

		m_pstPacketQueueForSend.push(stPacketContainer);

		if( ReleaseSemaphore( m_hDequeueSemaphoreForSend, 1, NULL ) == TRUE )
		{			
			bRst = true;
		}
		else
		{ //DTEC_TryCatchException
			TRACE("EnqueuePacket DequeueSemaphoreForSend Release Error");
			bRst = false;
		}
	}
	else
	{ //EX_DTEC_TryCatchException
		//TRACE("EnqueuePacketForSend FAILED\n");
		bRst = false;
	}

	return bRst;
}

bool CGRCommObj::DequeuePacketForSend(void** i_pvPacket, int *i_piLength)
{
	bool bRst = false;
	DWORD dRet = NULL;
	dRet = WaitForSingleObject(m_hDequeueSemaphoreForSend, COMM_TIMEOUT);

	if (WAIT_OBJECT_0 == dRet)
	{
		StMissionPacketContainer *stPacketContainer = m_pstPacketQueueForSend.front();

		*i_pvPacket = stPacketContainer->pvPacket;
		*i_piLength = stPacketContainer->iLength;

		delete stPacketContainer;
		m_pstPacketQueueForSend.pop();

		if( ReleaseSemaphore( m_hEnqueueSemaphoreForSend, 1, NULL ) == TRUE )
		{
			bRst = true;
		}
		else
		{ //DTEC_TryCatchException
			TRACE("DequeuePacketForSend EnqueueSemaphore Release Error");
			bRst = false;
		}
	}
	else
	{ //EX_DTEC_TryCatchException
		//TRACE("DequeuePacketForSend FAILED\n");
		bRst = false;
	}

	return bRst;
}

// EnqueueSendPacketInfo �� i_pvPacket �޸� ����. i_iLength : MissionHeader ���� Size
void CGRCommObj::EnqueueSendPacketInfo(void* i_pvPacket, int i_iLength)
{
	StSendPacketInfo *stSendPacketInfo = new StSendPacketInfo;
	stSendPacketInfo->pvPacket = i_pvPacket;
	stSendPacketInfo->iLength = i_iLength;

	m_pstSendPacketInfoQueue.push(stSendPacketInfo);
}

// DequeueSendPacketInfo �� i_pvPacket �޸� ����. i_iLength : MissionHeader ���� Size
void CGRCommObj::DequeueSendPacketInfo(void** i_pvPacket, int *i_piLength)
{
	StSendPacketInfo *stSendPacketInfo = m_pstSendPacketInfoQueue.front();

	*i_pvPacket = stSendPacketInfo->pvPacket;
	*i_piLength = stSendPacketInfo->iLength;

	delete stSendPacketInfo;
	m_pstSendPacketInfoQueue.pop();	
}


bool CGRCommObj::EnqueueRecvPacketInfo(void* i_pvPacket, int i_iLength, int i_iType)
{
	StRecvPacketInfo *stRecvPacketInfo = new StRecvPacketInfo;
	stRecvPacketInfo->pvPacket = i_pvPacket;
	stRecvPacketInfo->iLength = i_iLength;
	stRecvPacketInfo->iType = i_iType;

	m_pstRecvPacketInfoQueue.push(stRecvPacketInfo);

	return true;
}


bool CGRCommObj::DequeueRecvPacketInfo(void** i_pvPacket, int *i_piLength, int *i_iType)
{
	StRecvPacketInfo *stRecvPacketInfo = m_pstRecvPacketInfoQueue.front();

	*i_pvPacket = stRecvPacketInfo->pvPacket;
	*i_piLength = stRecvPacketInfo->iLength;
	*i_iType = stRecvPacketInfo->iType;

	delete stRecvPacketInfo;
	m_pstRecvPacketInfoQueue.pop();

	int iSize = m_pstRecvPacketInfoQueue.size();

	if ( iSize > 100 )
	{
		if ( iSize%100 == 1 )
		{
			CString str;
			str.Format("[ENC] Queue Size = %d", iSize);
			TraceTime(str);
		}
	}

	return true;
}

void CGRCommObj::TraceTime(CString i_strMsg)
{
	SYSTEMTIME stTime = SYSTEMTIME();
	memset(&stTime, NULL, sizeof(stTime));
	GetLocalTime(&stTime);
	TRACE("\r\n[Time(%s) : %02d(h)%02d(m)%02d(s)%02d(ms)]\r\n", i_strMsg, stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds);
}

void CGRCommObj::SendPacketFrameToLink(BYTE* i_aucSendBuf, unsigned short i_usSize)
{
	int iRet = NULL;
	// STEP#4 : �۽�
	SOCKET hSocket = GetRadarLinkClientSocket();
	if ( hSocket != NULL )
	{
		iRet = send(hSocket, (const char*)i_aucSendBuf, i_usSize, 0);
		//TRACE("    ====> [Send Data (SeqNo : %d, PacketFrameCnt : %d, PacketFrameTotalSize : %d) to Data Link\n", htonl(stLinkHeader.uiSeqNum), i_usFrameCnt, i_usFrameSize);				
		m_uiLinkSendSeqNo++;
	}
	else
	{
		iRet = SOCKET_ERROR;
		TRACE("DATA LINK SEND ERROR\n");
	}
}

bool CGRCommObj::GetPacketProcessingThreadEndFlag()
{
	return m_bPacketProcessingThreadEndFlag;
}

void CGRCommObj::GenerateSendPacketFrame()
{	
	EnterCriticalSection( &cs_GRCommLinkSend);

	if ( m_pstSendPacketInfoQueue.empty() == true )
	{
		LeaveCriticalSection( &cs_GRCommLinkSend);
	}
	else
	{
		//// Step#1 : Genenrate Send Packet
		void *pvPacket = nullptr;
		int iLength = NULL;
		char aucBuf[MAX_PACKET_SIZE] = {0,};
		USHORT usPacketCnt = NULL;
		USHORT usTotalPacketSize = NULL;
		int iMsgSize = NULL;
		//StMissionHeader stMHeader = StMissionHeader();

		if( m_pstSendPacketInfoQueue.empty() == false )
		{
			DequeueSendPacketInfo(&pvPacket, &iLength);
			if ( iLength < 0 || iLength > 10000 )
			{
				TRACE("SEND PACK Length Error \n");
			}
			else
			{
				memcpy(aucBuf, pvPacket, iLength);
				delete [] pvPacket;
			}
		}
		LeaveCriticalSection( &cs_GRCommLinkSend);

		USHORT usMsgSize = iLength;
		BYTE* pvData = new BYTE[usMsgSize];
		memset(pvData, NULL, usMsgSize);

		memcpy(pvData, aucBuf, iLength);

		EnterCriticalSection( &cs_GRCommLinkRecvQueue);
		EnqueueRecvPacketInfo((void*)pvData, iLength, PACKET_TYPE_SEND);
		LeaveCriticalSection( &cs_GRCommLinkRecvQueue);

		SetEvent(m_hEncEvent);	
	}
}