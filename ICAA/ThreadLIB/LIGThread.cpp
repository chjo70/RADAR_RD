#include "stdafx.h"

#include <windows.h>
#include <process.h> //_beginthreadex()

#include <string.h> //memset
#include "LIGThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define CRTDBG_MAP_ALLOC
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void LIGThread::Init() //���������� �ʱ�ȭ
{
  m_ptrRun = NULL;
  m_pRunFunc = NULL;
	m_pRunFuncWithParameter = NULL;
	m_parameter = NULL;

	m_threadStatus = LIGThread::THREAD_STOP;
	m_hThread = NULL;
	m_threadID = 0;
}

LIGThread::LIGThread()
{
	Init();
}

/**
* @brief		�ܺ� �������̽� class�� ȣ���� ��쿡 ���̴� ������
* @param[in]	ptr : IThread �������̽��� ��ӹ��� class
*/
//LIGThread::LIGThread(ILIGThread *ptr)
//{
//	Init();
//  m_ptrRun = ptr;
//}
//
//LIGThread::LIGThread(RUN_ROUTINE runRoutine)
//{
//  Init();
//	m_pRunFunc = runRoutine;
//}
//
//LIGThread::LIGThread(RUN_ROUTINE_WITH_PARAMETER runRoutine, void* parameter)
//{
//  Init();
//  m_pRunFuncWithParameter = runRoutine;
//	m_parameter = parameter;
//}

LIGThread::~LIGThread()
{
    Stop();
    Init();
}

/**
* @brief		�ܺ� �������̽� class�� Attach�Ͽ� ����ϵ��� ����
* @param[in]	ptr : IThread �������̽��� ��ӹ��� class
* @return		true : ����, false : �̹� Thread�� �������� ���
*/
//bool LIGThread::Attach(ILIGThread *ptr)
//{
//    m_ptrRun = ptr;
//    return true;
//}
//
//bool LIGThread::Attach(RUN_ROUTINE runRoutine)
//{
//    m_pRunFunc = runRoutine;
//    return true;
//}
//
//bool LIGThread::Attach(RUN_ROUTINE_WITH_PARAMETER runRoutine, void* parameter)
//{
//	m_pRunFuncWithParameter = runRoutine;
//	m_parameter = parameter;
//	return true;
//}

bool LIGThread::Start() //#FA_Q_4020_T1
{
	ScopedLock lock(&m_threadStatusLock);
	if (m_threadStatus == LIGThread::THREAD_STOP)
	{
		m_threadStatus = LIGThread::THREAD_RUNNING;

		//STOP -> RUNNING
		m_hThread = (HANDLE)_beginthreadex(NULL,
										   0,
										   &MainProc,
										   this,
										   0,
										   &m_threadID);
		if (m_hThread == NULL)
		{//DTEC_NullPointCheck
			m_threadStatus = LIGThread::THREAD_STOP;
			return false; //Thread���� ����
		}

	}	
    return true;
}

bool LIGThread::Stop() //#FA_Q_4020_T1
{
	ScopedLock lock(&m_threadStatusLock);
	
	if (m_threadStatus == LIGThread::THREAD_RUNNING)
	{
		//RUNNING -> STOPPING
		m_threadStatus = LIGThread::THREAD_STOP;
		return true;
	}
	else if (m_threadStatus == LIGThread::THREAD_STOP)
	{
		//STOP
		return false;
	}
	else
	{//DTEC_Else
	}
	return true;
}

//void LIGThread::Join_withMsgWait(const DWORD lTimeOut)
//{
//	if (m_hThread == NULL) 
//	{//DTEC_NullPointCheck
//		return;
//	}
//
//	DWORD dwStartTick = 0;
//	if( lTimeOut != INFINITE )
//	{
//		dwStartTick = GetTickCount();
//	}
//
//#pragma warning(disable:4127)
//	while( true )
//	{    
//		switch (::MsgWaitForMultipleObjects(1,&m_hThread,FALSE,INFINITE,QS_ALLEVENTS))    
//		{    
//		case WAIT_OBJECT_0:
//				return; //return true;
//		case WAIT_OBJECT_0+1:
//			{            
//				MSG msg;            
//				//@start_WJH
//				while (::PeekMessage(&msg,0,0,0,PM_REMOVE)!=0)
//				//@end_WJH
//				{                
//					if (msg.message == WM_QUIT) 
//					{
//						return; 
//					}
//						
//					::TranslateMessage(&msg);
//					::DispatchMessage(&msg);
//				} 
//			}
//			break;
//		default:
//			{//DTEC_Else
//				break;
//			}
//		}
//
//		if( lTimeOut != INFINITE && GetTickCount() - dwStartTick > lTimeOut )
//		{
//			return; 
//		}
//
//		Sleep(1);
//	}
//#pragma warning(default:4127)
//
//	//return; 
//}


//* @return		true : STOPPING -> STOP, STOP -> STOP �ΰ��
//*				false : RUNNING������
//void LIGThread::Join()
//{
//	//���°� LIGThread::THREAD_STOP�� �� ����Ȱ��̹Ƿ� Join�� �ʿ� ����.
//	//Stop() ���ڸ��� Thread�� �� ����ƴµ��� WaitForSingleObject�� ����ϸ� ���Ѵ���ϰ� �Ǽ� ��������߰���
//	//RUNNING�̳� STOPPING ���¸�
//	::WaitForSingleObject(m_hThread, INFINITE);
//	::CloseHandle(m_hThread);
//	m_hThread = NULL;
//	m_threadID = 0;
//	//STOPPING -> STOP
//	{
//		ScopedLock lock(&m_threadStatusLock);
//		m_threadStatus = LIGThread::THREAD_STOP;
//	}	
//}

bool LIGThread::IsRunning()
{
    ScopedLock lock(&m_threadStatusLock);
    return ((m_threadStatus == LIGThread::THREAD_RUNNING) ? true : false);
}

//bool LIGThread::IsStop()
//{
//    ScopedLock lock(&m_threadStatusLock);
//    return ((m_threadStatus == LIGThread::THREAD_STOP) ? true : false);
//}

BOOL LIGThread::SetPriority(int priority) //#FA_Q_4020_T1
{
    if (IsRunning() == false) //thread�� ���������̸� m_hThread�� ����
	{
        return FALSE;
	}
    
	BOOL bRtn = ::SetThreadPriority(m_hThread, priority);
	return bRtn;
}

//HANDLE LIGThread::GetThreadHandle()
//{
//    if (IsRunning() == false) //thread�� ���������̸�
//	{
//        return NULL;
//	}
//
//    return m_hThread;
//}

//unsigned int LIGThread::GetThreadID()
//{
//    if (IsRunning() == false) //thread�� ���������̸�
//	{
//        return 0;
//	}
//
//    return m_threadID;
//}

/**
* @brief		dwMilliseconds ���� Sleep
*/
//::Sleep((DWORD)dwMilliseconds);
//Suspends the execution of the current thread for at least the specified interval.
//
//dwMilliseconds : A value of zero causes the thread to relinquish the remainder of its time slice(quantum)
//to any other thread of equal priority that is ready to run. If there are no other threads of equal priority
//ready to run, the function returns immediately, and the thread continues execution.
//A value of INFINITE indicates that the suspension should not time out.
//void LIGThread::LIGSleep(int dwMilliseconds)
//{
//    ::Sleep((DWORD)dwMilliseconds);
//}

//void LIGThread::YieldThread()
//{
//	//about twice as fast as Sleep(0)
//	Sleep(0); //yields to any thread of same or higher priority on any processor
//}

//void LIGThread::LIGSuspendThread()
//{
//	Sleep(1); //yields to any thread on any processor
//				//will usually suspend the thread by about 15ms if timeBeginPeriod/timeEndPeriod [win32] are not used
//}

void LIGThread::Run()
{
    if (m_ptrRun != nullptr)
	{
        m_ptrRun->Run();
	}
}

unsigned __stdcall LIGThread::MainProc(void *parameter)
{
    LIGThread *pThis = (LIGThread *)parameter;
	
	pThis->Run();
	if (pThis->m_pRunFuncWithParameter != NULL && pThis->m_parameter != NULL)
	{
		pThis->m_pRunFuncWithParameter(pThis->m_parameter);
	}
	//@start_WJH
	else if ((pThis->m_pRunFunc)!=0)
	{
		pThis->m_pRunFunc();
	}
	else
	{//EX_DTEC_Else
		;
	}
	

	//LIGThread::THREAD_STOP�� �ƴ϶� RUN�̳� THREAD_STOPPING�� ��쿡�� Join�� �����ϵ��� �Ϸ���
	{
        ScopedLock lock(&pThis->m_threadStatusLock);
		//Stop�Լ��� ȣ��� ���¸� THREAD_STOPPING�ε� �� ���°� �ƴϸ� �׳� ������ ���̹Ƿ� THREAD_STOP �ǰ�
		pThis->m_threadStatus = LIGThread::THREAD_STOP;
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////////
//#include "GRThread.h"


/**
 * @brief     CGRThread
 * @param     void
 * @return    void
 * @exception 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-03-29, ���� 2:17 
 * @warning   
 */
CGRThread::CGRThread(void) : m_hThread(NULL), m_nThreadID(0), m_hEvent(NULL), m_iCommand(0)
{
	m_hEvent = ::CreateEvent(NULL, FALSE/*�⺻��:�ڵ�����*/, FALSE/*�⺻��:��ñ׳λ���*/, NULL );

}


/**
 * @brief     ~CGRThread
 * @param     void
 * @return    void
 * @exception 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-03-29, ���� 2:17 
 * @warning   
 */
CGRThread::~CGRThread(void)
{
	if( m_hThread != NULL ) {
		CloseHandle( m_hEvent );
	}

	if( m_hThread != NULL ) {
		CloseHandle( m_hThread );
	}


}

/**
 * @brief     StaticThreadStart
 * @param     LPVOID lpParam
 * @return    DWORD
 * @exception 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-03-29, ���� 2:17 
 * @warning   
 */
DWORD CGRThread::StaticThreadStart( LPVOID lpParam )
{
	CGRThread *pThread;

	pThread = ( CGRThread * ) lpParam;
	return pThread->RunThread();
}


/**
 * @brief     Start
 * @param     void
 * @return    bool
 * @exception 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-03-29, ���� 2:17 
 * @warning   
 */
bool CGRThread::StartThread(void)
{
	bool bRet=false;
	if( m_hThread != NULL ) {
		if( WaitForSingleObject( m_hThread, 0 ) == WAIT_TIMEOUT ) {
			bRet = false;
		}
		CloseHandle( m_hThread );

	}
	else {
		m_hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) CGRThread::StaticThreadStart, this, 0, & m_nThreadID );

		if( m_hThread != NULL )
		{
			bRet = true;
		}
	}

	return bRet;
}

/**
 * @brief     Stop
 * @param     void
 * @return    void
 * @exception 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-03-29, ���� 2:17 
 * @warning   
 */
//void CGRThread::StopThread(void)
//{
//	if( IsRunning() == true ) {
//		::TerminateThread( m_hThread, (DWORD) -1 );
//	}
//
//	if( m_hThread != NULL ) {
//		CloseHandle( m_hThread );
//		m_hThread = NULL;
//	}
//	return;
//}

/**
 * @brief     Sleep
 * @param     DWORD dwMilliseconds
 * @return    void
 * @exception 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-03-29, ���� 2:17 
 * @warning   
 */
//void CGRThread::GRThreadSleep( DWORD dwMilliseconds )
//{
//	::Sleep( dwMilliseconds );
//}

/**
 * @brief     IsRunning
 * @param     void
 * @return    bool
 * @exception 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-03-29, ���� 2:17 
 * @warning   
 */
bool CGRThread::IsRunning()
{
	bool bRet = false;
	if( m_hThread ) {
		DWORD dwExitCode=0;

		::GetExitCodeThread( m_hThread, & dwExitCode );
		if( dwExitCode == STILL_ACTIVE) {
			bRet = true;
		}
	}
	return bRet;
}

/**
 * @brief     SetThreadCommand
 * @param     int iCommand
 * @return    void
 * @exception 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-04-03, ���� 8:23 
 * @warning   
 */
//void CGRThread::SetThreadCommand( int iCommand )
//{
//	m_iCommand = iCommand;
//	SetEvent( m_hEvent );
//
//}

/**
 * @brief     GetCommand
 * @param     void
 * @return    int
 * @exception 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-04-03, ���� 9:26 
 * @warning   
 */
int CGRThread::GetThreadCommand() //#FA_Q_4020_T1
{
	if( m_iCommand != 0 )
	{
		return m_iCommand;
	}
	else
	{
		return 0;
	}

}