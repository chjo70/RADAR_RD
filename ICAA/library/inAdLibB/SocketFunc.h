#if !defined(AFX_SOCKETFUNC_H__CE082EB7_D55B_410A_B184_9ED1ECEE1480__INCLUDED_)
#define AFX_SOCKETFUNC_H__CE082EB7_D55B_410A_B184_9ED1ECEE1480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketFunc.h : header file
//

#include "../library/inAdLibB\SockServer.h"

//////////////////////////////////////////////////////////////////////////

#include "../library/inAdLibB\def_socket.h"

//////////////////////////////////////////////////////////////////////////

#define	SOCKET_POLL_EVENT		TRUE				// ���� ���� �б� ó�� ��� (TRUE:�ý����̺�Ʈ FALSE:����)
#define	CONV_BYTE_ORDER			TRUE				// ��Ʈ��ũ ����Ʈ �迭�� ���� ���� (TRUE:���� FALSE:������)

#define	SOCKET_SEND_MAX			SIZE_40K			// �ѹ��� �۽� ������ ������ ũ��
#define	SOCKET_SEND_BUFF		40976//SIZE_40K + SIZE_16	// ���� �۽ſ� ���� ���� (+ Header Size ����, ����� �ִ� 16����Ʈ���� ��������� ���)
#define	SOCKET_RECV_BUFF		SIZE_64K			// ���� ���ſ� ���� ����

/////////////////////////////////////////////////////////////////////////////
// CSocketFunc command target

class _SocketFunc_API CSocketFunc : public CObject
{
// Attributes
public:
			CSocketFunc();
			
	virtual ~CSocketFunc();

// Operations
public:


	//-----------------------------------------------------------------------------
	// TCP Socket Server Create : using the service file
	//-----------------------------------------------------------------------------
	BOOL	CreateSocketServer(HWND hWnd, CSockServer* pSockServer, char* szSockPort, UINT uWmSocketAccept, int nSocketID, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// TCP Socket Server Create : not using the service file
	//-----------------------------------------------------------------------------
	BOOL	CreateSocketServer(HWND hWnd, CSockServer * pSockServer, int nSockPort, UINT uWmSocketAccept, int nSocketID, CString * pstrMsg);

	//-----------------------------------------------------------------------------
	// UDP Socket Server Create : not using the service file
	//-----------------------------------------------------------------------------
	BOOL	CreateUdpSocketServer(HWND hWnd, CSockClient* pClientSocket, UINT uSocketEvent, char* szRemoteIP, char* szLocalIP, int nSockPort, int nSocketID, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// ����ױ� ��ȿ�� �˻�
	//-----------------------------------------------------------------------------
	BOOL	CheckDamagePacket(int iLength, int emTag);

	//-----------------------------------------------------------------------------
	// ������ '����'�����̸� ���� ���� ����
	//-----------------------------------------------------------------------------
	void	CloseSocketClient(CSockClient* pClientSocket);

	//-----------------------------------------------------------------------------
	// ������ '����'�����̸� ���� ���� ���� / ���� ����	- TCP ����
	//-----------------------------------------------------------------------------
	void	CloseTcpSocketClient(CSockClient** ppClientSocket);

	//-----------------------------------------------------------------------------
	// ������ '����'�����̸� ���� ���� ���� / ���� ����	- UDP ����
	//-----------------------------------------------------------------------------
	void	CloseUdpSocketClient(CSockClient** ppClientSocket);

	//-----------------------------------------------------------------------------
	// TCP ���� �������� ���� : service ���� ���
	//-----------------------------------------------------------------------------
	BOOL	ConnectSocketServer(HWND hWnd, CSockClient** ppClientSocket, UINT uSocketEvent, char* szSocketIP, char* szSockPort, int nSocketID);

	//-----------------------------------------------------------------------------
	// ���� �������� ���� : service ���� �̻��
	//-----------------------------------------------------------------------------
	BOOL	ConnectSocketServer(HWND hWnd, CSockClient** ppClientSocket, UINT uSocketEvent, char* szSocketIP, int nSockPort, int nSocketID);

	//-----------------------------------------------------------------------------
	// ���� Ŭ���̾�Ʈ Accept 
	//-----------------------------------------------------------------------------
	void	AcceptSocketClient(HWND hWnd, CSockServer* pServerSocket, CSockClient* pClientSocket, UINT uSocketEvent, int nSocketID);

	//-----------------------------------------------------------------------------
	// �۽��� ������ �ۼ� (1)
	//-----------------------------------------------------------------------------
	BOOL	MakeupSendSocketData(CSockClient* pClientSocket, int emTag, void* szData, int iLength, BOOL bConvByteOrder, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// TCP �������� �޼����� �۽�
	//-----------------------------------------------------------------------------
	BOOL	SendSocketPacket(CSockClient* pClientSocket, void* sMessage, int iLength, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// (UDP) �۽��� ������ �ۼ� (1)
	//-----------------------------------------------------------------------------
	BOOL	MakeupUdpSendSocketData(CSockClient* pClientSocket, int emTag, void* szData, int iLength, CString strRemoteIpAddress, UINT uPort, BOOL bConvByteOrder, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// UDP �������� �޼����� �۽�
	//-----------------------------------------------------------------------------
	BOOL	SendUdpSocketPacket(CSockClient* pClientSocket, void* sMessage, int iLength, CString strRemoteIpAddress, UINT uPort, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// ���ο� ����Ÿ�� �������� TCP ���Ͽ��� ����Ÿ�� �д´�
	//-----------------------------------------------------------------------------
	LONG	ReceiveTcpSocketPacket(CSockClient* pClientSocket, char* szBuff, int* nReadByte, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// ���ο� ����Ÿ�� �������� UDP ���Ͽ��� ����Ÿ�� �д´�
	//-----------------------------------------------------------------------------
	LONG	ReceiveUdpSocketPacket(CSockClient* pClientSocket, char* szBuff, int* nReadByte, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// ���ŵ� ������ �м�
	//-----------------------------------------------------------------------------
	BOOL	DevideReceiveSocketData(CSockClient* pClientSocket, int iReadSize, char* szData, int* emTag, BOOL bConvByteOrder);

	//-----------------------------------------------------------------------------
	// �ش�ä�ο� �ش�Ǵ� OP ���� IP Address ��������
	//-----------------------------------------------------------------------------
	CString GetPeerIPAddress(CSockClient* pClientSocket);

	//-----------------------------------------------------------------------------
	// ���� ����ױ׿� �ش�Ǵ� �ױ׹��ڿ� ��������
	//-----------------------------------------------------------------------------
	CString GetPacketTagString(int emTag);

	SOCKADDR_IN		m_sockAddrRemote;

// Overrides

// Implementation
protected:


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETFUNC_H__CE082EB7_D55B_410A_B184_9ED1ECEE1480__INCLUDED_)
