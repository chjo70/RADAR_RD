#if !defined(AFX_SOCKCOMMON_H__B114E321_E906_4CFB_B671_2E4A2616C933__INCLUDED_)
#define AFX_SOCKCOMMON_H__B114E321_E906_4CFB_B671_2E4A2616C933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SockCommon.h : header file
//

#ifdef _SocketFunc_DLL_
#define _SocketFunc_API  _declspec(dllexport)
#else
#define _SocketFunc_API  _declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
#define SPEC_DP_SZ					1601

#define	SIZE_16					16			// ��� ���� ������
#define SIZE_64					64
#define SIZE_256				256			
#define SIZE_1K					1024
#define SIZE_2K					2048
#define SIZE_4K					4096
#define SIZE_10K				10240
#define SIZE_20K				20480
#define SIZE_40K				40960
#define SIZE_64K				65536
#define SIZE_512K				524288

//////////////////////////////////////////////////////////////////////////

#define	BUFFMIN					64//SIZE_64		// ���� ũ��
#define	BUFFMAX					4096//SIZE_4K		// �ӽ� ���� ũ��		// SIZE_256

#define	NORMAL_ERROR			0			// �Ϲ��� ����
#define	FATAL_ERROR				1			// ġ���� ����

#define	SOCKET_QUEUE			10240 //DEL SIZE 524288		// ���� ���ſ� ���� ((1024 * 512) + 1024) * 2

// ���� �ð� ����
#define	SOCKET_CONNECT_DELAY	200		// ���� ������� ���� �ð� (���� msec) 1000
#define	SOCKET_DISCONNECT_DELAY	400		// ���� ���ܽ��� ���� �ð� (���� msec) 2000
#define	SOCKET_SEND_DELAY		3			// �������� ���� �۽Ž��� ���� �ð� (���� msec)

// Ÿ�̸� ����
#define	SOCKET_POLL_TIMER		100			// ������ ���۷κ��� ������ �д� Ÿ�̸��� ID
#define	SOCKET_POLL_INTEVAL		300			// ������ ���۷κ��� ������ �д� �ð� ���� (���� msec)

//////////////////////////////////////////////////////////////////////////
// [ykhong 11/18/2009] 
#define SCKRECV_RECVERR					-1
#define SCKRECV_PACKET_BROKEN			-2

#define SCKRECV_PACKET_COMPLETED		1
#define SCKRECV_PACKET_CONTINUE			2

#define  MAX_SOCKET_RECV_SIZE			16384
#define  MAX_SOCKET_PACKET_SIZE			16384


// ��� ���� ��� ���� ����
typedef enum {
	ePacketHeader_1		= 0,
	ePacketHeader_2,						// ��� ���� ��� 2 : ����
	ePacketHeader_3,						// ��� ���� ��� 3 : ����! ������Ž���迡�� ���[RMS ID] [2006.12.13 SWR. ������ �߰�!]
	ePacketHeader_4,						// ��� ���� ��� 4 : ����! ������Ž���迡�� ���[FDF ID] [2006.12.13 SWR. ������ �߰�!]
	
	eCountPacketHeader,
} enumPacketHeader;


/////////////////////////////////////////////////////////////////////////////
// CSockCommon command target

class _SocketFunc_API CSockCommon : public CObject
{
// Attributes
public:

// Operations
public:
			CSockCommon();
	virtual ~CSockCommon();

	char	m_szLocalName[BUFFMIN];										// �ý��� PC��
	char	m_szLocalIP[BUFFMIN];										// �ý��� IP ����

	BOOL	GetCurrentNetworkInformation();								// �� �۾� �ý����� ��Ʈ��ũ ������ ����
	void	GetIPAddressFromName(char* szHostName, char* szIPAddrss);	// ȣ��Ʈ������ �ش� ��ũ��ũ �ּҸ� ã�Ƽ� ����

	unsigned short	HostToPcShort(short data);									// �������� ������ ���� PC�� short���������� ��ȯ
	int		HostToPcInt(int data);										// �������� ������ ���� PC�� int���������� ��ȯ
	float	HostToPcFloat(float data);									// �������� ������ ���� PC�� �ε��Ҽ��������� ��ȯ
	double	HostToPcDouble(double data);								// �������� ������ ���� PC�� ������ �ε��Ҽ��������� ��ȯ

	short	PcToHostShort(short data);									// ������ �۽��� ���� ������ short���������� ��ȯ
	int		PcToHostInt(int data);										// ������ �۽��� ���� ������ int���������� ��ȯ
	float	PcToHostFloat(float data);									// ������ �۽��� ���� ������ �ε��Ҽ��������� ��ȯ
	double	PcToHostDouble(double data);								// ������ �۽��� ���� ������ ������ �ε��Ҽ��������� ��ȯ

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSockCommon)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSockCommon)
	//}}AFX_MSG

// Implementation

public:
	// [ykhong 11/18/2009] 
	void SetMinMaxTag(int min, int max);
	BOOL CheckSckPacket(int iLength, int emTag);

	static int GetServbyname(const char* sPortNo);
protected:

	int m_iMinTag, m_iMaxTag;
};

_SocketFunc_API extern CSockCommon g_SockCommon;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKCOMMON_H__B114E321_E906_4CFB_B671_2E4A2616C933__INCLUDED_)
