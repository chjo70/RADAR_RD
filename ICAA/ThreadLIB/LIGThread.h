/**
* @file			LIGCondition.h
* @date			2010/10/22
* @author		Jang, MinKi(Greg), Revised by Lee, JeongNam
* @brief			Thread
* @remarks		
*/

#ifndef LIGThread_H
#define LIGThread_H


#include "LIGLock.h"
#include "LIGCondition.h"


//warning: 'struct ILIGThread' has virtual functions but non-virtual destructor
struct ILIGThread
{
	virtual void Run() = 0;
};


/**
 * @class    LIGThread
 * @brief   ������ ����� ��ӹ޾� ����� �� �ְ� Wrapping ��Ŭ����
 *
 * (1) Ŭ���� ����
 * - ������ ����� ��ӹ޾� ����� �� �ְ� Wrapping ��Ŭ����
 *
 * (2) �����������
 * - �ش���׾���
 *
 * (3) ���� �� ����ó��
 * - �ش���׾���
 */
class LIGThread : public ILIGThread
{
	//!< Thread Status ����
	enum THREAD_STATUS { THREAD_STOP = 0, THREAD_RUNNING }; //, THREAD_STOPPING };

public:
	/**
	* @brief    ������ �Լ�
	*/
	LIGThread();
	/**
	* @brief    �Ҹ��� �Լ�
	*/
	virtual ~LIGThread();

public:
	//LIGThread(ILIGThread *ptr);
	//bool Attach(ILIGThread *ptr);

	/**
	* @brief    �ܺ� �Լ� ���� ����
	*/
	typedef void (*RUN_ROUTINE)();

	/**
	* @brief     �ܺ� �Լ� ���� ���� (�Ķ��Ÿ �ִ� ���)
	* @param     parameter		���� �Ķ��Ÿ
	*/
	typedef void (*RUN_ROUTINE_WITH_PARAMETER)(void* parameter);

	//�ܺ� �Լ� ȣ�� ����
//	LIGThread(RUN_ROUTINE runRoutine);
	//bool Attach(RUN_ROUTINE runRoutine);

//	LIGThread(RUN_ROUTINE_WITH_PARAMETER runRoutine, void* parameter);
//	bool Attach(RUN_ROUTINE_WITH_PARAMETER runRoutine, void* parameter);

public:
	/**
	* @brief		Thread ����
	* @return		true : STOP -> RUNNING, RUNNING -> RUNNING �ΰ��, false : Thread������ �����߰ų� STOPPING������					
	*/
	bool Start();

	/**
	* @brief		Thread ����
	* @return		true : RUNNING -> STOPPING, STOPPING -> STOPPING �ΰ��, 	false : STOP������
	*/
	bool Stop();

	/**
	* @brief		Thread ����ñ��� ���(SYNC) �Լ�, STOP������ ��� : �ٷ�return, STOPPING������ ��� : �����尡 ����� ������ ���Ѵ�� �� -> STOP
	* @param     lTimeOut		Ÿ�� �ƿ� ���� �Ķ��Ÿ
	*/
	//void Join_withMsgWait(const DWORD lTimeOut = INFINITE);

	/**
	* @brief		Thread ����ñ��� ����ϴ� �Լ�
	*/
	//void Join();		///< LIGThread�� concrete class�� destructor���� Join()�� ȣ���ϸ� �ȵ�.
	
	/**
	* @brief		Thread �켱���� ���� �Լ�
	* @param     priority		������ �켱���� ���� ����
	*/
	BOOL SetPriority(int priority = THREAD_PRIORITY_NORMAL);

	/**
	* @brief		Thread�� ���� ������ Ȯ���ϴ� �Լ�
	* @return		true : Thread ���� ��, false : Thread ���� ����
	*/
	bool IsRunning();	///< Thread�� ���� ������ Ȯ��
	
	/**
	* @brief		Thread�� �����Ǿ� �ִ��� Ȯ���ϴ� �Լ�
	* @return		true : Thread ���� ��, false : Thread ���� ����
	*/
	//bool IsStop();

	/**
	* @brief		Thread �ڵ��� ������ ���� �Լ�
	* @return		Thread �ڵ� ��
	*/
	//HANDLE GetThreadHandle();

	/**
	* @brief		Thread ID�� ������ ���� �Լ�
	* @return		Thread ID ��
	*/
	//unsigned int GetThreadID();

protected:
	/**
	* @brief		�ܺ� �������̽� class�� ȣ���� ��� ���
	*/
	virtual void Run();

private:
	inline void Init(); //���������� �ʱ�ȭ

public:
	///< dwMilliseconds ���� Sleep
	//static void LIGSleep(int dwMilliseconds);
	///< ���� ���μ��� ���� ���� �켱���� �̻��� ���� �����尡 ������� ��� ������ �纸
	//static void YieldThread();
	///< ���� ���μ��� ���� �ƹ� �����忡�� ������ �纸. �� 15ms ���� �ڽ��� thread�� suspend ��Ų��. 2014.05.27. ������
	//static void LIGSuspendThread(); 

private:
	static unsigned __stdcall MainProc(void *parameter);

private:
	volatile LIGThread::THREAD_STATUS m_threadStatus; //!< Thread Status ����
	ILIGThread *m_ptrRun; //!< �ܺ� �������̽� ȣ��
	RUN_ROUTINE m_pRunFunc; //!< �ܺ� �Լ� ȣ��
	RUN_ROUTINE_WITH_PARAMETER m_pRunFuncWithParameter; //!< �ܺ� �Լ� ȣ��
	void* m_parameter; //m_pRunFuncWithParameter��

	HANDLE m_hThread; //!< Thread Handle
	unsigned int m_threadID; //!< Thread ID

	LIGLock m_threadStatusLock;
};

/**
 * [�ĺ��� : CLS-GMU-EL-?-????]
 * [������ : SRS-G-SOFR-???]
 * 
 * @class     CGRThread
 * @brief     
 * 
 * (1)��Ŭ���� ����
 *  - ������ Ŭ�����μ� ȣ�� �Ǵ� ���� ����� �����Ѵ�.
 * 
 * (2)�������������
 *  - ������������� �������ּ���.
 * 
 * (3)������ �� ����ó��
 *  - ��ȣ �м�, �м� ����, CED �ҷ����� �� ���� �ð��� �ҿ�Ǵ� ����� �����Ѵ�.
 * 
 * @author    ��ö�� (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2017-03-29, ���� 2:15 
 * @warning   
 */
class CGRThread
{
protected:
	HANDLE m_hThread;
	HANDLE m_hEvent; 
	DWORD m_nThreadID;

	int m_iCommand;

private:
	static DWORD WINAPI StaticThreadStart( LPVOID lpParam );

protected :
	virtual DWORD RunThread(void)=0;

public:
	CGRThread(void);
	~CGRThread(void);


	bool StartThread();
	//void StopThread();

	//inline HANDLE GetThreadHandle() { return m_hThread; }
	//inline DWORD GetThreadID() { return m_nThreadID; }

	int GetThreadCommand();

	inline void ClearThreadCommand() { m_iCommand=0; }
	inline int GetCommand() { return m_iCommand; }

	//void SetThreadCommand( int iCommand );

	bool IsRunning();
	//void Join();
	//	bool Yield();
	//void GRThreadSleep( DWORD dwMilliseconds );
	DWORD Suspend();
	DWORD Resume();

};

#endif 
