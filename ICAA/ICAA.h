
// ICAA.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CICAAApp:
// �� Ŭ������ ������ ���ؼ��� ICAA.cpp�� �����Ͻʽÿ�.
//

class CICAAApp : public CWinApp
{
public:
	CICAAApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CICAAApp theApp;