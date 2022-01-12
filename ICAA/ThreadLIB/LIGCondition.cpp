#include "stdafx.h"

#include <windows.h>
#include <process.h> //_beginthreadex()

#include "LIGCondition.h"
#include "LIGLock.h"
#include <string.h> //memset

#ifdef _DEBUG
#define new DEBUG_NEW
#define CRTDBG_MAP_ALLOC
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LIGCondition::LIGCondition(bool isAutoCreate)
	: isInit(false)	
{	
	m_hEvent = NULL;
	if (isAutoCreate)
	{
		CreateLIGEvent();
	}
}

LIGCondition::~LIGCondition()
{
	CloseHandle(m_hEvent);
	m_hEvent = NULL;
}

//* @param[in]	bManualReset : TRUE(��������), FALSE(�⺻��:�ڵ�����)
//*               bInitialState : TRUE(�ñ׳λ���), FALSE(�⺻��:��ñ׳λ���)
bool LIGCondition::CreateLIGEvent(bool bManualReset) //#FA_Q_4020_T1
{
	ScopedLock lock(&m_isInitLock);

	if (isInit == true) //�̹� �̺�Ʈ�� ������ ���
	{
		return false;
	}
	m_hEvent = ::CreateEvent(NULL,
		(BOOL)bManualReset/*�⺻��:�ڵ�����*/,
		FALSE/*�⺻��:��ñ׳λ���*/,
		NULL);
	isInit = true;
	return true;
}

//bool LIGCondition::DestroyEvent()
//{
//	ScopedLock lock(&m_isInitLock);
//
//	if (isInit == false) //�̺�Ʈ�� �����Ǿ� ���� ���� ���
//	{
//		return false;
//	}
//
//	::SetEvent(m_hEvent);
//	CloseHandle(m_hEvent);
//	m_hEvent = NULL;
//
//	isInit = false;
//	return true;
//}

bool LIGCondition::Wait(unsigned int dwMilliseconds) //#FA_Q_4020_T1
{
	{
		ScopedLock lock(&m_isInitLock);
		if (isInit == false) //�̺�Ʈ�� �����Ǿ� ���� ���� ���
		{
			return false;
		}
	}

		switch (::WaitForSingleObject(m_hEvent, (DWORD)dwMilliseconds))
		{
		case WAIT_OBJECT_0:
			return true;
		case WAIT_TIMEOUT:
			return false;
		default:
			{//EX_DTEC_Else
				return false;
			}		
		}
	

}
//
//bool LIGCondition::Wait(LIGLock* unlock, unsigned int dwMilliseconds)
//{
//	{
//		ScopedLock lock(&m_isInitLock);
//		if (isInit == false) //�̺�Ʈ�� �����Ǿ� ���� ���� ���
//		{
//			return false;
//		}
//	}
//
//	unlock->unlock();
//
//	/*if (dwMilliseconds == -1)
//		dwMilliseconds = INFINITE;*/
//
//	::WaitForSingleObject(m_hEvent, (DWORD)dwMilliseconds);		
//
//	unlock->lock();
//	return true;
//}

///*bool LIGCondition::Wait(LIGLock* unlock, unsigned int dwMilliseconds)
//{
//	{
//		ScopedLock lock(&m_isInitLock);
//		if (isInit == false) //�̺�Ʈ�� �����Ǿ� ���� ���� ���
//			return false;
//	}
//
//	unlock->unlock();
//
//	/*if (dwMilliseconds == -1)
//		dwMilliseconds = INFINITE;*/
//
//	::WaitForSingleObject(m_hEvent, (DWORD)dwMilliseconds);		
//
//	unlock->lock();
//	return true;
//}*/

bool LIGCondition::LIGSetEvent() //#FA_Q_4020_T1
{
	{
		ScopedLock lock(&m_isInitLock);
		if (isInit == false) //�̺�Ʈ�� �����Ǿ� ���� ���� ���
		{
			return false;
		}
	}
	::SetEvent(m_hEvent);
	return true;
}

bool LIGCondition::LIGResetEvent() //#FA_Q_4020_T1
{
	{
		ScopedLock lock(&m_isInitLock);
		if (isInit == false) //�̺�Ʈ�� �����Ǿ� ���� ���� ���
		{
			return false;
		}
	}
	::ResetEvent(m_hEvent);
	return true;
}

