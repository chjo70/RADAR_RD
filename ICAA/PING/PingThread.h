// PingThread.h: interface for the CPingThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_)
#define AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ping.h"

/** 
* SDD �ĺ��� : D-CM-SFD-ACIF
*
* (1) ���
*  - ������� ������ ���� ���
*  - �������� ��Ż��� Ȯ��(CPing)�� ���� ������ ���� ��� ���
*
* (2) �����������
*  - �ش� ���� ����
*/
class CPingThread  
{
public:
	CPingThread();
	virtual ~CPingThread();

	/**
	* ������ ID
	*/
	UINT	m_dwID;
	/**
	* �������ڵ�
	*/
	HANDLE	m_hThread;
	/**
	* ������ ���Ḧ ���� �̺�Ʈ �ڵ�
	*/
	HANDLE	m_hKillEvent;
	/**
	* ������ �̺�Ʈ ó���� ���� ��ȣ�߻� �ڵ�
	*/
	HANDLE	m_hSignalEvent;

	static UINT __stdcall  ThreadProc(void* pThis);
	void StartPing(int i_nBitType, int i_nUnitID);

	/**
	* ��ſ������ Ȯ�� ������ �����ϴ� Ŭ���� �ν��Ͻ�
	*/
	CPing	m_ping;
	/**
	* ȣ��Ʈ IP
	*/
	CString m_strHost;

	CString m_strLinkIP;
	CString m_strServerIP;

	CString m_strHeartbeatIP_1;
	CString m_strHeartbeatIP_2;

	CString m_strHeartbeatIP_3;
	CString m_strHeartbeatIP_4;

	void SetHeartbeatIP_1(CString i_strIP);
	void SetHeartbeatIP_2(CString i_strIP);

	void SetHeartbeatIP_3(CString i_strIP);
	void SetHeartbeatIP_4(CString i_strIP);

	void SetLinkIP(CString i_strLinkIP);
	void SetServerIP(CString i_strServerIP);

	/**
	* ������ �޽��� �ڵ�
	*/
	HWND	m_hWnd;
	/**
	* ��õ� ȸ��
	*/
	UINT	m_nRetries;
	
	/**
	* ��ü���� ����
	*/
	int		m_iBITKind;
		
	int		m_nUnitID;
};

#endif // !defined(AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_)
