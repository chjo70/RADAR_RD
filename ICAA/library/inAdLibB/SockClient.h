#if !defined(AFX_SOCKCLIENT_H__32F68844_012F_4421_8958_7135BE53BDCB__INCLUDED_)
#define AFX_SOCKCLIENT_H__32F68844_012F_4421_8958_7135BE53BDCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SockClient.h : header file
//

#include "SockCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CSockClient command target
class CRcvFuncTCP;

class _SocketFunc_API CSockClient : public CAsyncSocket
{
// Attributes
public:
	HWND				m_hWnd;						// �޼����� ���� �ڵ�
	BOOL				m_bCreated;					// ���� ���� ���� �÷��� (TRUE:���� FALSE:����)
	BOOL				m_bConnected;				// ���� ���� ���� �÷��� (TRUE:���� FALSE:����)
	CSockCommon			m_pSocketCommon;			// ���� ���� �Լ� Ŭ����
	struct sockaddr_in	m_StSockAddrRemote;			// Remote ������ ��ũ��ũ �ּ� ����ü (internet style)
	struct sockaddr_in	m_StSockAddrLocal;			// Local ������ ��ũ��ũ �ּ� ����ü (internet style)
	unsigned int		m_uiEventReceive;			// ���� ���� ���� �̺�Ʈ ID
	unsigned int		m_uiEventError;				// ���� ���� �̺�Ʈ ID
	unsigned int		m_uiEventClose;
	int					m_iSocketID;				// ���� ���� ID
	BOOL				m_bRecvEventPoll;			// ���� ���� �б� ó�� ��� (TRUE:�̺�Ʈ FALSE:����)
													// TRUE  : ������ ���� �̺�Ʈ�� ������ �д´�
													// FALSE : ���� ������ �ð����� ������ �д´�

	unsigned int		m_uiRecvQueueSize;			// �̿ϼ� ������ �ӽ� ���� ť�� ũ��
//	char				m_sRecvQueue[SOCKET_QUEUE];	// �̿ϼ� ������ �ӽ� ���� ť
    char				m_sRecvQueue[SIZE_512K];	// �̿ϼ� ������ �ӽ� ���� ť
	char				m_sErrorString[BUFFMAX];	// ���� �޼���

	CRcvFuncTCP			*m_CRcvFuncTCP;
	void SetThis(LPVOID lParam);
	BOOL m_bSetThis;
	char m_byteReceive[BUFFMAX];   //8-bit data item.

// Operations
public:
			CSockClient();

	virtual ~CSockClient();

			// �������κ��Ϳ��� �޼����� ���� �ڵ��� ����
	void	SetMessageHandle(HWND hWnd);			

			// ������ �̺�Ʈ �������� ����
	void	SetConfig(	UINT uiEventReceive, 
						UINT uiEventError, 
						UINT uiEventClose, 
						BOOL bFlag);					

			// ������ ID�� ����Ͽ� �̺�Ʈ �������� ����
	void	SetConfig(	UINT uiEventReceive, 
						UINT uiEventError, 
						UINT uiEventClose, 
						int iSocketID, 
						BOOL bFlag);	
			// ������ TCP (Stream Socket)������� ���� (Host,Services ���� ���)
	BOOL	CreateSocket(char* sIPAddressRemote, char* sPortNo);

			// ������ TCP (Stream Socket)������� ���� (Host,Services ���� �̻��)
	BOOL	CreateSocket();	

			// ������ UDP (Datagram Socket)������� ���� (Host,Services ���� ���)
	BOOL	CreateSocket(char* sIPAddressRemote, char* sIPAddressLocal, char* sPortNo);
			
			// ������ UDP (Datagram Socket)������� ���� (Host,Services ���� �̻��)
	BOOL	CreateSocket(char* sIPAddressLocal, int iPortNo);

	void	CloseSocket();																						


// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSockClient)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSockClient)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKCLIENT_H__32F68844_012F_4421_8958_7135BE53BDCB__INCLUDED_)
