#pragma once

#include "CommStruct.h"
#include <queue>
#include <deque>
#include <list>

//class CASAE_DEMONDlg;
#define PING_TIMER_DATA_LINK	7777
#define PING_TIMER_SERVER	6000
#define PING_INTERVAL	5000
#define PING_START_INTERVAL 3000

#define PING_ID_HEARTBEAT_SVR 1000
#define PING_ID_HEARTBEAT_CLT 2000
#define CLIENT_NO_1	(0)
#define CLIENT_NO_2	(1)
#define CLIENT_NO_3	(2)
#define CLIENT_NO_4	(3)

#define SERVER_ID_LINK_1 (1)

#include "ComplibFacade.h"

using namespace std;

typedef struct stNEXSANPacket
{
	unsigned char ucID;				// NEXSAN : 100
	unsigned char Opcode;			// 
	unsigned char aucData[1024];	// ���Ű��. �ִ� 1024 ����Ʈ.

	stNEXSANPacket()
	{
		memset(this,0, sizeof(stNEXSANPacket));
	}
}SNEXSANPacket;


typedef struct stADSBPacket
{
	unsigned char uc1a;
	unsigned char ucNum;
	unsigned char MLAT[6];
	unsigned char SigLevel;
	unsigned char buf[14];

	stADSBPacket()
	{
		memset(this,0, sizeof(stADSBPacket));
	}
}SADSBPacket;


/**
* [�ĺ��� : -]
* [������ : -]
*
* @class CGRCommObj
* @brief �������̽� ������ Ŭ����
*
* (1)��Ŭ���� ����
*  - ���� �������̽��� �����͸� �����ϴ� ������ Ŭ�����̴�.
* (2) �����������
*  - �� Ŭ�������� OPCODE���� ��Ī�Ǵ� Mngr Ŭ������ �ּҰ��� �����Ѵ�.
*  - Opcode ������ ���� �� ���� 3byte�� �ʿ��ϴ�.
*  - ���� �������̽��� �����ʹ� IF������, ȭ���̹���, ��ü���� �׸� ������ �̷�����ִ�. (����)
* (3) ���� �� ����ó��
*  - �ش���׾���.
*/
class CGRCommObj
{
// Operation
public :
    CGRCommObj();
    ~CGRCommObj();

	static CGRCommObj& GetInstance();

	CCommonMngr* GetMngr(unsigned int i_uiOpcode);
	bool RegisterOpcode(unsigned int i_uiOpcode, CCommonMngr *i_pcMngr);
	bool UnregisterOpcode(unsigned int i_uiOpcode, CCommonMngr *i_pcMngr);	// �̱����� �ƴ� �������� ��ü������ OP�ڵ带 ���/������ �� �ֵ��� �ϱ����� ����
																			// �ʳ��� ����Ͽ� �ۼ��Ͽ��� �ּ�ó����. - 2014.04.22., ���⺴
																			// �׽�Ʈ ��� �̻���� �۵������� ��ü�躰�� UnregisterOpcode()�� ������ ������ �� ��ƾ� �� ���� 
	vector <unsigned int> m_auiOpcode;
	vector <CCommonMngr*> m_apcMngr;

	//void SetDlgPointer(CASAE_DEMONDlg* i_pDlg);
	//CASAE_DEMONDlg* m_pDlg;

	bool EnqueuePacket(void* i_pvPacket, int i_iLength);
	bool DequeuePacket(void** i_pvPacket, int *i_piLength);

	bool AddClientInfo(SOCKET i_hClientSocket, unsigned char i_ucLastIP, unsigned short i_usOperatorID, bool i_bDataLink);
	bool DeleteClientInfo(SOCKET i_hClientSocket);
	StClientSocket* GetClientInfo(unsigned char i_ucLastIP);

	void SetSocketMode(int i_iSocketMode);
	int GetSocketMode();

public:
	unsigned int m_uiRegisteredOpcodeCount;
//	unsigned int m_auiOpcode[MAX_OPCODE];
//	CCommonMngr* m_apcMngr[MAX_OPCODE];

	CComplibFacade   m_ComplibFacade;

	void StartTcpServerThreadFunc(LPVOID arg);
	void StartTcpClientThreadFunc(LPVOID arg);

	void StartTcpClientADSBThreadFunc(LPVOID arg);
	void StartTcpClientDF_ThreadFunc(LPVOID arg);
	void StartTcpClientPDW_ThreadFunc(LPVOID arg);
	void StartTcpClientRDUnit_ThreadFunc(LPVOID arg);
	void StartTcpClientVUHFUnit_ThreadFunc(LPVOID arg);

	void StartTcpRadarLink_1ClientThreadFunc(LPVOID arg);

	void StartTcpInterCommClientThreadFunc(LPVOID arg);

	void StartTcpPacketProcessingThreadFunc(LPVOID arg);
	void StartTcpPacketProcessingThreadFuncForSend(LPVOID arg);
	void StartTcpPacketProcessingADSBThreadFunc(LPVOID arg);
	void StartRadarLinkPacketProcessingThreadFunc(LPVOID arg);

	void StartUdpClientThreadFunc(LPVOID arg);

	void SetDataLinkServerSocket(SOCKET i_hSocket);
	void SetRadarLink1ClientSocket(SOCKET i_hSocket);
	void SetInterCommClientSocket(SOCKET i_hSocket);

	SOCKET GetDataLinkServerSocket();
	SOCKET GetRadarLinkClientSocket();
	SOCKET GetInterCommClientSocket();

	BYTE m_aucSendBuf[MAX_BUF_SIZE];
	BYTE m_aucTcpRecvBuf[MAX_BUF_SIZE];
	BYTE m_aucUdpRecvBuf[MAX_BUF_SIZE];
	BYTE m_aucCompRecvBuf[MAX_BUF_SIZE];
	BYTE m_aucNFrameBuf[MAX_BUF_SIZE];

	BYTE m_aucTcpServerRecvBuf[MAX_SERVER_CONNECTION_CNT][MAX_BUF_SIZE_FOR_CHILD];

	// (MAX_BUF) ���̹��м� -> ���̴���Ž
	BYTE m_aucTcpClientRecvBuf_1[MAX_BUF_SIZE];
	// (MAX_BUF) PDW�߻��� -> ���̴���Ž
	BYTE m_aucTcpClientRecvBuf_2[MAX_BUF_SIZE];
	// (MAX_BUF) ���̴���� -> ���̴���Ž
	BYTE m_aucTcpClientRecvBuf_3[MAX_BUF_SIZE];
	// (MAX_BUF) ��� -> ���̴���Ž
	BYTE m_aucTcpClientRecvBuf_4[MAX_BUF_SIZE];

	// (MAX_BUF) RADAR -> DF
	BYTE m_aucRadarLink_1RecvBuf[MAX_BUF_SIZE];

	void SetDisplayCommLog(bool i_bDisplayCommLog);
	bool GetDisplayCommLog();

	bool GetConnectionInfo(int i_iConnectionType);
	void SetConnectionInfo(int i_iConnectionType, bool i_bConnection);
	
	SOCKADDR_IN GetUdpSocketAddr();
	void SetUdpSocketAddr(SOCKADDR_IN i_addrSocket);
	
	void SetUdpSocket(SOCKET i_hSocket);
	SOCKET GetUdpSocket();
		
	void RecvPacketFromInterCommServer(BYTE* i_pucData, int i_iSize);
	void RecvPacketFromDataLink(BYTE* i_pucData, int i_iSize);
	void RecvPacketFromDataLink1Frame(BYTE* i_pucData, int i_iSize);
	void RecvPacketFromDataLinkNFrame(BYTE* i_pucData, int i_iSize);

	int RecvFromLink(BYTE* i_pucData, int i_iSize);
	int SendToLink(SOCKET i_hSocket, const char* i_pcBuf, int i_iSize, unsigned int i_uiOpcode);	
	int SendAckToDataLink(int i_iFrameType, BYTE* i_pucData);	

	bool StartTimer();
	void StopTimer();

	unsigned int m_uiCurrentTimerNo;	

	HANDLE m_hTimer;
	HANDLE m_hTimerQueue;

	HANDLE m_hEncEvent;	

	StMissionPacket m_stMissionPacketForResend;
	unsigned int m_uiMissionPacketSizeForResend;

	int m_iRetryCnt;

	SOCKET m_hRadarLink1ClientSocket;

	LARGE_INTEGER liCounter1;
	LARGE_INTEGER liCounter2;
	LARGE_INTEGER liFrequency;
	list<StClientSocket*> m_pstClientList;

private:
	queue<StMissionPacketContainer*> m_pstPacketQueue;
	
	HANDLE m_hEnqueueSemaphore;
	HANDLE m_hDequeueSemaphore;
	HANDLE m_hClientSemaphore;
	int m_iSocketMode;

	bool m_bDataLinkConnection;
	bool m_bServerConnection;

	SOCKET		m_hUdpSocket;
	SOCKADDR_IN	m_addrUdpSocket;

	UINT	m_uiLinkSendSeqNo;
	UINT	m_uiLinkRecvSeqNo;
	const unsigned int m_uiLinkFrameSize;

	bool m_bDisplayCommLog;

	SOCKET m_hDataLinkServerSocket;	
	SOCKET m_hInterCommClientSocket;

	static UINT TcpServerThreadFunc(LPVOID arg);
	static UINT TcpClientThreadFunc(LPVOID arg);

	static UINT TcpClientADSBThreadFunc(LPVOID arg);
	static UINT TcpClientDF_1ThreadFunc(LPVOID arg);
	static UINT TcpClientPDW_2ThreadFunc(LPVOID arg);
	static UINT TcpClientRDUnit_3ThreadFunc(LPVOID arg);
	static UINT TcpClientVUHFUnit_4ThreadFunc(LPVOID arg);
	
	static UINT TcpRadarLinkClientThreadFunc(LPVOID arg);
	static UINT TcpInterCommClientThreadFunc(LPVOID arg);
	static UINT UdpClientThreadFunc(LPVOID arg);	

	static UINT TcpPacketProcessingThreadFunc(LPVOID arg);
	static UINT TcpPacketProcessingThreadFuncForSend(LPVOID arg);
	static UINT TcpPacketProcessingADSBThreadFunc(LPVOID arg);
	static UINT RadarLinkPacketProcessingThreadFunc(LPVOID arg);	

public:	
	void UpdateRecvMsgInfo(unsigned int i_uiSeqNo);
	void UpdateSendMsgInfo(unsigned int i_uiSeqNo, bool i_bRecvAck);

	void RecvUnRecvMsg(unsigned int i_uiSeqNo);

	void StartUdp1ClientThreadFunc(LPVOID arg);
	//bool IsMsgForMulticast(unsigned char i_ucCMDCode);

private:
	void InsertUnRecvMsgSeqNo(unsigned int i_uiSeqNo);
	void InsertUnSendMsgSeqNo(unsigned int i_uiSeqNo);

	unsigned int m_uiTotalRecvMsgCnt;	// ��ü ���� �޽��� ����
	unsigned int m_uiUnRecvMsgCnt;		// �� ���� �޽��� ����
	unsigned int m_uiTotalSendMsgCnt;	// ��ü �۽� �޽��� ����
	unsigned int m_uiUnSendMsgCnt;		// �� �۽� �޽��� ����	
	std::list<int> m_listUnRecvMsg;		// �̼��� �޽��� Seq No ����
	std::list<int> m_listUnSendMsg;		// �̼۽� �޽��� Seq No ����

	bool m_bPacketProcessingThreadEndFlag;
	bool m_bUseHeartbeat;
	bool m_bHeartbeatConnection_1;
	bool m_bHeartbeatConnection_2;
	bool m_bHeartbeatConnection_3;
	bool m_bHeartbeatConnection_4;
	int m_iServerID;

public:
	void FinishPacketProcessingThread();		
	bool GetInfoToUseHeartbeat();
	void LoadHeartbeatInfo();
	void SetServerID(int i_iServerID);
	int GetServerID();
	bool GetPDWConnStatus();

	bool EnqueuePacketForSend(void* i_pvPacket, int i_iLength);
	bool DequeuePacketForSend(void** i_pvPacket, int *i_piLength);

	queue<StMissionPacketContainer*> m_pstPacketQueueForSend;


	//���� �޽��� 
	queue<StSendPacketInfo*> m_pstSendPacketInfoQueue;
	void EnqueueSendPacketInfo(void* i_pvPacket, int i_iLength);
	void DequeueSendPacketInfo(void** i_pvPacket, int *i_piLength);

	queue<StRecvPacketInfo*> m_pstRecvPacketInfoQueue;
	bool EnqueueRecvPacketInfo(void* i_pvPacket, int i_iLength, int i_iType);
	bool DequeueRecvPacketInfo(void** i_pvPacket, int *i_piLength, int *i_iType);


	void TraceTime(CString i_strMsg);

	void SendPacketFrameToLink(BYTE* i_aucSendBuf, unsigned short i_usSize);

	HANDLE m_hEnqueueSemaphoreForSend;
	HANDLE m_hDequeueSemaphoreForSend;

	unsigned short m_usOperatorID;

public:
	bool GetPacketProcessingThreadEndFlag();
	void GenerateSendPacketFrame();
};