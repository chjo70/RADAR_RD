#include <stdAfx.h>
#include "MsgQueueThread.h"

CMsgQueueThread::CMsgQueueThread(void)
{	
	if(m_cond.CreateLIGEvent(true) == FALSE ) // unsignal �� ����. manual reset ���·� �̺�Ʈ�� �����.
	{
		return;
	}
	this->Start();	
}

CMsgQueueThread::~CMsgQueueThread(void)
{	
	this->Stop();

	DeleteAllQData();

	// Ÿ�̸� ������, Q������ ���� ��� ���� �ð��� ������� �Ѵ�. �׷��� ������ ������ �ȿ��� ����ϴ� Critical Section ��ü���� �̹� delete
	// �Ǿ������� ScopedLock���� critical section ��ü�� ���� lock�� �ɱ� ������ ���α׷� ���� �� �״� ���� �߻�. 2015.05.26. ������.
	Sleep(2000); 	
}

void CMsgQueueThread::Run()
{
	bool bRslt = true;
	STMsg stMsg;	

	while( IsRunning() == TRUE)
	{
		m_cond.Wait(); // �ñ׳��� ��ٸ��� ���̴�. 

		bRslt = GetMsgFromQ(stMsg);
		if(bRslt)
		{
			ProcessMsg(stMsg);			
		}
		else // ť�� �� �̻� �޽����� ���ٴ� �̾߱��.
		{
			m_cond.LIGResetEvent();
		}				
	}
}

bool CMsgQueueThread::GetMsgFromQ(STMsg& i_stMsg)
{
	bool bRtn = false;

	ScopedLock lock(&m_QLock);
	if(m_queMsg.size() != 0)
	{		
		i_stMsg = m_queMsg.front();
		m_queMsg.pop();
		bRtn = true;
	}
	
	return bRtn;
}

void CMsgQueueThread::PushMsg(STMsg& i_stMsg)
{
	ScopedLock lock(&m_QLock);
	m_queMsg.push(i_stMsg);
}

void CMsgQueueThread::DeleteAllQData()
{
	ScopedLock lock(&m_QLock);
	int nQsize = m_queMsg.size();
	if(nQsize != 0)
	{		
		for(int i=0; i< nQsize; ++i)
		{
			m_queMsg.pop();
		}		
	}
}

void CMsgQueueThread::ProcessMsg(STMsg& i_stMsg)
{
	// ��ӹ��� Ŭ�������� ������ �ʿ�
}