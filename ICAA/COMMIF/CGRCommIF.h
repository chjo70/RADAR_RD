#pragma once

#include "CGRCommMngr.h"
#include "CGRCommObj.h"

#include "..\PING\PingThread.h"

//class CASAE_DEMONDlg;
#define MAX_IP_STR_BYTE	100
/**
* [�ĺ��� : DD-GMU-SFR-0015]
* [������ : SRS-GMU-SOFR-015]
*
* @class CGRCommIF
* @brief LAN ��� Ŭ����
*
* (1)��Ŭ���� ����
*  - �ܺο� LAN���� ����� �����ϴ� Ŭ�����̴�.
* (2) �����������
*  - ��� ��� ���� �����ӹ�������, ���嵥����ó�����̴�.
* (3) ���� �� ����ó��
*  - ���ǵ��� ���� �޽����� ��쿡�� false�� ��ȯ�Ѵ�.
*/
class CGRCommIF
{
// Operation
public :
    CGRCommIF();
    ~CGRCommIF();

private:
	class Cleanup
	{
	public:
		~Cleanup();
	};

public:
    
	static CGRCommIF& GetInstance();

	bool Connect(int i_iPort, const char *i_pacIP);
	bool Connect(int i_iPort, const char *i_pacIP, int i_iNo);
    bool Send(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLinkID, unsigned char i_ucRevOperID, unsigned char i_ucOperID, const void *i_pvData);	
	bool SendToDataLink(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLinkID, unsigned char i_ucRevOperID, unsigned char i_ucOperID, const void *i_pvData);
	bool RegisterOpcode(unsigned int i_uiOpcode, CCommonMngr *i_pcMngr);
	bool UnregisterOpcode(unsigned int i_uiOpcode, CCommonMngr *i_pcMngr);


	//bool CreateUDPDemon(CASAE_DEMONDlg* i_pDlg);
	bool SendUDPData(int i_iSize, const void *i_pvData);
	int ConnectADSBS(int i_iPort, const char *i_pacIP);
	int SendNEXSANData(int i_iSize, const void *i_pvData);
	
	bool CreateServer(int i_iPort);

	bool SetReConnect(bool i_bReConnect);//�ӽ�-- �̱���

	void ReturnPingRst(int i_iConnectType, bool i_bRst, bool i_bSvr=false);

	void ReleaseSocket();

	HANDLE m_hTimer;
	HANDLE m_hTimerQueue;

	CPingThread m_PingThread;
	bool m_bServer;

	void SetHeartbeatIP_1(char* i_pcBuf);
	void SetHeartbeatPort_1(int i_iPort);

	void SetHeartbeatIP_2(char* i_pcBuf);
	void SetHeartbeatPort_2(int i_iPort);

	void SetHeartbeatIP_3(char* i_pcBuf);
	void SetHeartbeatPort_3(int i_iPort);

	void SetHeartbeatIP_4(char* i_pcBuf);
	void SetHeartbeatPort_4(int i_iPort);

	void SetServerID(int i_iServerID);
	int GetServerID();

	bool GetPDWConnStatus();

// Attribute
private:
	char m_acHeartbeatIP_1[MAX_IP_STR_BYTE];
	char m_acHeartbeatIP_2[MAX_IP_STR_BYTE];
	char m_acHeartbeatIP_3[MAX_IP_STR_BYTE];
	char m_acHeartbeatIP_4[MAX_IP_STR_BYTE];

	int m_iHeartbeatPort_1;	
	int m_iHeartbeatPort_2;	
	int m_iHeartbeatPort_3;	
	int m_iHeartbeatPort_4;	
	

public:	// ���� ���� public �Ӽ����� �ٲ� - 2014.03.22. - ���⺴
/**
* @brief ������ �����Ǿ����� ���θ� ��Ÿ���� flag
*  - true : ������ ������, false : ������ �������� ����
*/
	SOCKET m_hClientSocket;
	SOCKET m_hServerSocket;
	CGRCommMngr m_cCommMngr;
	CGRCommObj& m_hCommObj;

	std::list<int> m_listConnectPort;

	bool m_bConnect;
};

#define GP_COMM_IF CGRCommIF::GetInstance()
