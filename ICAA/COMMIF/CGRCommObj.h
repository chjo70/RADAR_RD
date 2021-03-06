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
	unsigned char aucData[1024];	// 수신결과. 최대 1024 바이트.

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
* [식별자 : -]
* [추적성 : -]
*
* @class CGRCommObj
* @brief 인터페이스 데이터 클래스
*
* (1)　클래스 설명
*  - 지상 인터페이스의 데이터를 저장하는 데이터 클래스이다.
* (2) 설계결정사항
*  - 본 클래스에는 OPCODE별로 매칭되는 Mngr 클래스의 주소값을 저장한다.
*  - Opcode 구조상 비교할 때 앞의 3byte만 필요하다.
*  - 지상 인터페이스의 데이터는 IF데이터, 화염이미지, 자체점검 항목 등으로 이루어져있다. (삭제)
* (3) 제한 및 예외처리
*  - 해당사항없음.
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
	bool UnregisterOpcode(unsigned int i_uiOpcode, CCommonMngr *i_pcMngr);	// 싱글톤이 아닌 동적생성 객체에서도 OP코드를 등록/해제할 수 있도록 하기위해 만듦
																			// 훗날을 기약하여 작성하였고 주석처리함. - 2014.04.22., 진기병
																			// 테스트 결과 이상없이 작동하지만 부체계별로 UnregisterOpcode()를 적용할 시점을 잘 잡아야 할 것임 
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

	// (MAX_BUF) 레이버분석 -> 레이더방탐
	BYTE m_aucTcpClientRecvBuf_1[MAX_BUF_SIZE];
	// (MAX_BUF) PDW발생판 -> 레이더방탐
	BYTE m_aucTcpClientRecvBuf_2[MAX_BUF_SIZE];
	// (MAX_BUF) 레이더장비 -> 레이더방탐
	BYTE m_aucTcpClientRecvBuf_3[MAX_BUF_SIZE];
	// (MAX_BUF) 운용 -> 레이더방탐
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

	unsigned int m_uiTotalRecvMsgCnt;	// 전체 수신 메시지 개수
	unsigned int m_uiUnRecvMsgCnt;		// 미 수신 메시지 개수
	unsigned int m_uiTotalSendMsgCnt;	// 전체 송신 메시지 개수
	unsigned int m_uiUnSendMsgCnt;		// 미 송신 메시지 개수	
	std::list<int> m_listUnRecvMsg;		// 미수신 메시지 Seq No 관리
	std::list<int> m_listUnSendMsg;		// 미송신 메시지 Seq No 관리

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


	//상향 메시지 
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