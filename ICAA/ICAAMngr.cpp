#include "stdafx.h"
#include "ICAAMngr.h"
#include "MsgQueueThread.h"
#include "ThreadTask/DFMsgDefnt.h"

CICAAMngr* CICAAMngr::uniqueInstance=nullptr;

CICAAMngr::CICAAMngr()
:m_hCommIF_ICAAMngr(m_hCommIF)
{
	// ini���Ͽ��� Server/Client ���� ���� ������
	char readBuf[100] = {0};
	char *envini_path = (".\\config.ini");
	CString	cstrServerClient = CString("");
	GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("MODE"), NULL, readBuf, _countof(readBuf), envini_path);
	cstrServerClient.Format(("%s"), readBuf);	// SERVER or CLIENT
	CString	strPort = CString("");

	if(strcmp(cstrServerClient, "SERVER") == 0)
	{
		int iPort = 0;
		TRACE("���� ����\n");
		m_hCommIF.SetServerID(SERVER_ID_LINK_1);
		GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("PORT1"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.CreateServer(iPort);
	}
	else if(strcmp(cstrServerClient, "ADSBD") == 0)
	{
		int iPort = 0;
		TRACE("���� ����\n");
		m_hCommIF.SetServerID(SERVER_ID_LINK_1);
		GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("PORT1"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.CreateServer(iPort);

		// �� �κ��� ADSB �κ����� �����ؾ� ��
		//RADAR ��Ž�� OR RADAR ��Ž ����
		GetPrivateProfileString(("ADSBD"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_1(iPort);
		GetPrivateProfileString(("ADSBD"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_1(readBuf);	
	}
	else if(strcmp(cstrServerClient, "CLIENT") == 0)
	{
		int iPort = 0;

		// ���̴��м� ���� /////////////////////////////////////////////////////////////////////////////////
		TRACE("Ŭ���̾�Ʈ ����\n");
		GetPrivateProfileString(("CLIENT1"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_1(iPort);
		GetPrivateProfileString(("CLIENT1"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_1(readBuf);


		// PDW�߻��� ���� /////////////////////////////////////////////////////////////////////////////////
		GetPrivateProfileString(("CLIENT2"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_2(iPort);
		GetPrivateProfileString(("CLIENT2"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_2(readBuf);


		// ���̴���Ž ��� (������) /////////////////////////////////////////////////////////////////////////////////
		GetPrivateProfileString(("CLIENT3"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_3(iPort);
		GetPrivateProfileString(("CLIENT3"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_3(readBuf);

		// V/UHF ��� (������) /////////////////////////////////////////////////////////////////////////////////
		GetPrivateProfileString(("CLIENT4"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_4(iPort);
		GetPrivateProfileString(("CLIENT4"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_4(readBuf);
		// ���PC (Only ����Ʈ�� ����) /////////////////////////////////////////////////////////////////////////////////
	}
	else
	{

	}
}

CICAAMngr::~CICAAMngr()
{
	
}

CICAAMngr* CICAAMngr::GetInstance()
{
	if (uniqueInstance == nullptr)
	{
		uniqueInstance = new CICAAMngr();		
	}

	return uniqueInstance;
}

void CICAAMngr::Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData)
{
	TRACE("[Log] Message is received.\n");

	// ���Ŵܿ��� Msg ������ ����
	
	// ������ ó���� ���� Msg Queue�� ����ֱ�
}

void CICAAMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE���� ��츦 �����Ͽ� ó���ϴ� ���� ���� �ʿ�
	// �ϴܿ� ����

}