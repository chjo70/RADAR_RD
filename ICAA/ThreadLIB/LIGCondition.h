/**
* @file			LIGCondition.h
* @date			2010/10/22
* @author		Jang, MinKi(Greg), Revised by Lee, JeongNam
* @brief			Condition Variables
* @remarks		
*/

#ifndef LIGCondition_H
#define LIGCondition_H

#include "LIGLock.h"

/**
 * @class    LIGCondition
 * @brief   Thread Event ������ ���� wrapping Ŭ����
 *
 * (1) Ŭ���� ����
 * -   Thread Event ������ ���� wrapping Ŭ����
 *
 * (2) �����������
 * - �������� Thread Event �Լ� ���� �� ����
 *
 * (3) ���� �� ����ó��
 * - �ش���׾���
 */
class LIGCondition
{
public:
	/**
	* @brief     ������ �Լ�
	*/
	LIGCondition(bool isAutoCreate = false);
	/**
	* @brief     �Ҹ��� �Լ�
	*/
	~LIGCondition();

	/**
	* @brief		Thread Event ����
	* @return		true : �̺�Ʈ ����, false : �̹� Thread�� �������̰ų� �̺�Ʈ�� ������ ���
	* @remark		�ڵ�����, ��ñ׳λ��°� �⺻
	*/

	/**
	* @brief     Thread Event ����
	* @param     bManualReset		�Ŵ���/�ڵ� ���� �̺�Ʈ�� �ʱ⿡ �����ϵ��� �����ϴ� ������
	* @return    ������ true, ���н� false
	*/
	bool CreateLIGEvent(bool bManualReset = false);
	
	/**
	* @brief     Event �ı� �Լ�	
	* @return    ������ true, ���н� false
	*/
	//bool DestroyEvent();

	/**
	* @brief		Thread Event �� �߻��� ������ dwMilliseconds ���� ��� (Wait)
	* @param[in]	dwMilliseconds : -1(���Ѵ��), 0~(���ڸ�ŭ ���)
					unlock : ���ڸ� unlock ��Ű�� wait �� �������ö� �ٽ� lock
	* @return		true : �̺�Ʈ�� �޾Ұų� ���ð� ����, false : Thread�� �������� �ƴϰų� �̺�Ʈ�� �����Ǿ� ���� ���� ���
	*/

	/**
	* @brief     Thread Event �� �߻��� ������ dwMilliseconds ���� ��� (Wait)
	* @param    dwMilliseconds : -1(���Ѵ��), 0~(���ڸ�ŭ ���)	
	* @return    ������ true, ���н� false
	*/
    bool Wait(unsigned int dwMilliseconds = 0xffffffff);

	/**
	* @brief     Thread Event �� �߻��� ������ dwMilliseconds ���� ��� (Wait)
	* @param    unlock : ���ڸ� unlock ��Ű�� wait �� �������ö� �ٽ� lock
	* @param    dwMilliseconds : -1(���Ѵ��), 0~(���ڸ�ŭ ���)	
	* @return    ������ true, ���н� false
	*/
   // bool Wait(LIGLock* unlock, unsigned int dwMilliseconds = 0xffffffff);

	/**
	* @brief     Thread Event �߻� (Notify One)
	* @return    true : �̺�Ʈ �߻�, false : Thread�� �������� �ƴϰų� �̺�Ʈ�� �����Ǿ� ���� ���� ���
	*/
	bool LIGSetEvent(); 

	/**
	* @brief     �ڵ� ��ȿ���� üũ�ϴ� �Լ�
	* @return    true : ��ȿ��, false : ��ȿ���� ����
	*/
	inline bool IsValidHandle() { return m_hEvent != NULL; }

	/**
	* @brief     Thread Event �߻� (Reset) (Notify One)
	* @return    true : �̺�Ʈ �߻�, false : Thread�� �������� �ƴϰų� �̺�Ʈ�� �����Ǿ� ���� ���� ���
	*/
	bool LIGResetEvent();

private:
	HANDLE m_hEvent; 
	volatile bool isInit;
	LIGLock m_isInitLock; 
};

#endif 
