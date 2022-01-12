#if !defined(AFX_SOCKSERVER_H__61D0F509_6873_4635_B109_6861A321464E__INCLUDED_)
#define AFX_SOCKSERVER_H__61D0F509_6873_4635_B109_6861A321464E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SockServer.h : header file
//

#include "SockCommon.h"
#include "SockClient.h"

/////////////////////////////////////////////////////////////////////////////
// CSockServer command target

class _SocketFunc_API CSockServer : public CAsyncSocket
{
// Attributes
public:
	HWND				m_hWnd;							// �޼����� ���� �ڵ�
	BOOL				m_bCreated;						// ���� ���� ���� �÷��� (TRUE:���� FALSE:����)
	BOOL				m_bConnected;					// ���� ���� ���� �÷��� (TRUE:���� FALSE:����)
	CSockCommon			m_pSocketCommon;				// ���� ���� �Լ� Ŭ����
	struct sockaddr_in	m_StSockAddrLocal;				// Local ������ ��ũ��ũ �ּ� ����ü (internet style)
	unsigned int		m_uiEventAccept;				// ���� ACCEPT �̺�Ʈ ID
	int					m_iSocketID;					// ���� ���� ID
	unsigned int		m_uiConnectClinetPort;			// ������ ���ӵ� Ŭ���̾�Ʈ ������ ��Ʈ ���� (Ȯ�ο�!)
	CString				m_sConnectClientIPAddress;		// ������ ���ӵ� Ŭ���̾�Ʈ ������ ��ũ��ũ �ּ� (Ȯ�ο�!)
	char				m_sErrorString[BUFFMAX];		// ���� �޼���

// Operations
public:
			CSockServer();
	virtual ~CSockServer();

			// �������κ��Ϳ��� �޼����� ���� �ڵ��� ����
	void	SetMessageHandle(HWND hWnd);			

			// ������ �̺�Ʈ �������� ����
	void	SetConfig(unsigned int uiEventAccept, int iSocketID);	
			
			// ������ TCP (Stream Socket)������� ���� (Host,Services ���� ���)
	BOOL	CreateSocket(char* sPortNo);			

			// ������ TCP (Stream Socket)������� ���� (Host,Services ���� �̻��)
	BOOL	CreateSocket(int iPortNo);					

	void	CloseSocket();

			// OnAccept �Լ������� ���ο� Ŭ���̾�Ʈ�� ����� Ŭ���̾�Ʈ ���Ͽ� �Ѱ��ش�
	void	GetAcceptSocket(CSockClient* pCSockClient);	

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSockServer)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSockServer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKSERVER_H__61D0F509_6873_4635_B109_6861A321464E__INCLUDED_)
