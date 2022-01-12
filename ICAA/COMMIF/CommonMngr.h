
/**
* [�ĺ��� : CLS-GMU-GR-L-COMM]
*
* [������ : SRS-G-SOFR-009]
*
* @class CCommonMngr
* @brief ���� Manager �θ� Ŭ����
*
* (1) Ŭ���� ����
* -  C/E/F �Ŵ��� Ŭ������ ��ӹ��� �� �ִ� �θ� Ŭ����. ��� �� Observer ���� ��� ����.
*
* (2) �����������
* - C/E/F Manager Ŭ������ �� Ŭ������ ��ӹ޾Ƽ� �����Ѵ�.
*
* (3) ���� �� ����ó��
* - �ش���׾���
*/
#pragma once

#include "CGRCommIF.h"

#include <map>
#include <list>
#include <queue>

using namespace std;

typedef unsigned long uint32_tt;


class CCommonMngr
{

public:
	
	/**
	* @brief       �������Լ�
	* @param       void   
	* @return         
	*/
	CCommonMngr(void);
	
	/**
	* @brief       �Ҹ����Լ�
	* @param       void   
	* @return         
	*/
	virtual ~CCommonMngr(void);

	/**
	* @brief �Ŵ��� �̸��� ����ϱ� ���� ��Ʈ��
	* - Ư�� �Ŵ����� ã���� �� ��, ������ ����� �Ŵ��� �̸����� ���Ͽ� �ش� �Ŵ����� ã�Ƴ� �� ����. 2014.12.30. 
	*/
	string m_strIAM; 

protected:
	/**
	* @brief STL list �����̳� ��ü
	* - Observer�� ��ϵǴ� Controller Ŭ���� �����͸� �����ϱ� ���� list �����̳� ��ü
	*/
	//std::list<CCommonControllerIF*> m_listData;

	/*!
	 * @brief STL list �����̳� ��ü
	 * - Ư�� �Ŵ����� ���� ��ȭ�� ���� �ٸ� �Ŵ����� �� ������ observer ������ ����� �޾ƾ��� �ʿ伺 ���. 2013.10.10. ������
	 */
	std::list<CCommonMngr*> m_listMngr;

	/**
	* @brief CGRCommIF ��ü
	* - ����� �����ϱ� ���� ��� ��ü
	*/
	CGRCommIF& m_hCommIF;
	/*CGRDMapIF& m_hDMapIF;
	CGRInterCommIF& m_hInterCommIF;*/

	bool m_bIsServerMode; // 2013.12.10. ������ �߰�
	bool m_bIsReplayMode; // 2015.02.06. ������ �߰�
	
	/*LIGLock m_commonLock;
	LIGLock m_commonMngrLock;*/

protected:
	void InitTimeStruct(tm& i_Time);

public:
	
	/**
	* @brief       COMM Interface�κ��� �޽����� �����ϴ� �Լ� (������)
	* @param    i_uiOpcode		op �ڵ� 
	* @param    i_usSize		�޽��� ũ�� 
	* @param    i_usOperatorID		����� ID ��ȣ
	* @param    i_pvData		���ŵ� �������� ���� �ּҰ�
	* @param    i_bServer		�����ӹ� ������ �Ǵ� POSN ����
	* @param   i_bReplay		�ӹ��翬 ��� ����
	* @return     void
	*/
	virtual void Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOper, unsigned char ucOpertorID, void *i_pvData) = 0;
	
	// ����(���α׷� ����/�α׿���/�ý��� ����) �� ó�� �Լ�, 2014.06.19. - ���⺴
	// �ڽĵ��� �������ؼ� ��� ��
	
	/**
	* @brief       MakeFSigID �����Լ�
	* @return      UINT   MakeFSigID
	*/
	UINT MakeFSigID();
	unsigned int m_unLastSigID; ///< ������ ��ȣ ID ����

	
	// float�� double�� ������ network���� ������ ���� endian ó���� �Լ� - 2013.12.24. ���⺴
	
	/**
	* @brief       ntohf ��ȯ�Լ�
	* @param       float fSrc   �÷��� ����
	* @return      float   �÷��� ����
	*/
	float ntohf( float fSrc );
	
	/**
	* @brief       htonf ��ȯ�Լ�
	* @param       float fSrc   �÷��� ����
	* @return      float   �÷��� ����
	*/
	float htonf( float fSrc );
	
	/**
	* @brief       ntohd ��ȯ�Լ�
	* @param       double dblSrc   ������ ����
	* @return      float   �÷��� ����
	*/
	float ntohd( double dblSrc );
	
	/**
	* @brief       htond ��ȯ�Լ�
	* @param       double dblSrc   ������ ����
	* @return      float   �÷��� ����
	*/
	float htond( double dblSrc );

	
	/**
	* @brief       ���� �ð� ��û �Լ�
	* @return      string  �ð� ���ڿ� ��ü  
	*/
	string GetCurrentTimeInfo();	// CGRTaskMngr���� CCommonMngr�� �̵� - 2014.04.10. ���⺴
	
	/**
	* @brief       ���� DF �ð� ��û �Լ�
	* @param       unsigned int unSec  ��  
	* @param       unsigned int unNanoSec  ������  
	* @return      string   �ð� ���ڿ� ��ü
	*/
	string GetCurrDFTimeInfo( unsigned int unSec, unsigned int unNanoSec );	// F���� C�� ���й�Ž ��û �� �������� �� �ð����� ���ϴ� �Լ�, 2016.07.12 - ���⺴
	
	/**
	* @brief       �����ID ȹ���Լ�
	* @return      unsigned short  �����ID 
	*/
	unsigned short GetLogInOperatorId();	// CGRTaskMngr���� CCommonMngr�� �̵� - 2014.04.10. ���⺴
	//2����_������(����)=================================================
	
	/**
	* @brief       �����ID ȹ�� �Լ�
	* @param       int i_nLinkNum   ��ũ��ȣ
	* @return      unsigned short   �����ID
	*/
	unsigned short GetLogInOperatorId(int i_nLinkNum);
	//2����_������(��)=================================================

	
	/**
	* @brief       ��ũ��ȣ ȹ���Լ�
	* @param       unsigned short & i_usOperId   �����ID
	* @return      int   ��ũ��ȣ
	*/
	int GetLinkNum(unsigned short& i_usOperId); // 2015.1.5. ������ �߰�. ����� id �� ������ �����͸�ũ ��ȣ(1 Ȥ�� 2)�� �������ִ� �Լ�

	unsigned int MakeOPCode(unsigned char i_uchCMDCode, unsigned char i_uchSrc=DEVICECODE_BRA, 
		unsigned char i_uchDest=DEVICECODE_TRD, unsigned char i_uchEncrypt=0, unsigned char i_uchClass=0);	
	
protected:
	//start_static_0918
	template <typename T> void DeletePointers(std::list<T*> &i_listContainer)	//#FA_Q_2502_T5
	{
		for (auto iter = i_listContainer.begin(); iter != i_listContainer.end(); iter++) //#FA_C_PotentialUnboundedLoop_T3
		{
			delete (*iter);
			// *iter = nullptr;
		}
		i_listContainer.clear();
	}
	//end_static_0918

	//////////////////////////////////////////////////////////////////////////
	// ���� �޸� ����
	//start_static_0918
	template <typename T> void DeletePointers(std::vector<T*> &i_vecContainer)	//#FA_Q_2502_T5
	{
		for (auto iter = i_vecContainer.begin(); iter != i_vecContainer.end(); iter++ ) //#FA_C_PotentialUnboundedLoop_T3
		{
			delete (*iter);
			// *iter = nullptr;
		}
		i_vecContainer.clear();
	}
	//end_static_0918

	template <typename T> void DeletePointers2(std::vector<T*> &i_vecContainer)
	{
		for( std::vector<T*>::iterator it=i_vecContainer.begin() ; it != i_vecContainer.end() ; ) { //#FA_C_PotentialUnboundedLoop_T3
			it = i_vecContainer.erase( it );
		}
		i_vecContainer.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	//start_static_0918
	template <typename T1, typename T2> void DeletePointers(std::map<T1, T2*> &i_mapContainer)	//#FA_Q_2502_T5
	{
		for (auto iter = i_mapContainer.begin(); iter != i_mapContainer.end(); ) //#FA_C_PotentialUnboundedLoop_T3
		{
			i_mapContainer.erase( iter++ );
		}
		i_mapContainer.clear();
	}
	//end_static_0918

	template <typename T> void DeleteRangePointers( typename list<T> *pListContainer, typename list<T>::iterator itStart, typename list<T>::iterator itEnd )
	{
		for (auto iter = itStart; iter != itEnd; iter++) //#FA_C_PotentialUnboundedLoop_T3
		{
			delete (*iter);
		}
		pListContainer->erase( itStart, itEnd );
	}

public:
	
	/**
	* @brief       alignStrCnt ó���Լ�
	* @param       CString & i_str   ��Ʈ�� ��ü
	* @param       int i_nCnt   ī��Ʈ
	* @return      void   
	*/
	void alignStrCnt(CString& i_str, int i_nCnt);
	
	/**
	* @brief       CStrToChar �Լ�
	* @param       CString & i_str    ��Ʈ�� ��ü 
	* @param       char * i_puch    ������ ��ü
	* @param       int i_nCnt     ī��Ʈ
	* @return      void   
	*/
	void CStrToChar(CString &i_str, char *i_puch, int i_nCnt=0);	
	
	/**
	* @brief       CStrToUChar �Լ�
	* @param       CString & i_str  ��Ʈ�� ��ü 
	* @param       unsigned char * i_puch   ������ ��ü
	* @param       int i_nCnt   ī��Ʈ
	* @return      void   
	*/
	void CStrToUChar(CString &i_str, unsigned char *i_puch, int i_nCnt=0);
};

// 
/*! \todo   Ŭ���� �ȿ� �ִ� ���÷� ���Ǹ� ������ ���� �����ص� �ٸ��� ������ ���� �� ������, ����� �״�� ��.
    \author ��ö�� (churlhee.jo@lignex1.com)
    \date 	2016-08-08 16:01:35
*/
template <typename T> void DeleteRangePointers( typename list<T> *pListContainer, typename list<T>::iterator itStart, typename list<T>::iterator itEnd )
{
	for (auto iter = itStart; iter != itEnd; iter++)
	{
		delete (*iter);
	}
	pListContainer->erase( itStart, itEnd );
}


template <typename T> void DeletePointers(std::list<T*> &i_listContainer)
{
	for (auto iter = i_listContainer.begin(); iter != i_listContainer.end(); iter++)
	{
		delete (*iter);
		// *iter = nullptr;
	}
	i_listContainer.clear();
}
template <typename T> void DeletePointers(std::vector<T*> &i_vecContainer)
{
	for (auto iter = i_vecContainer.begin(); iter != i_vecContainer.end(); iter++ )
	{
		delete (*iter);
		// *iter = nullptr;
	}
	i_vecContainer.clear();
}
template <typename T1, typename T2> void DeletePointers(std::map<T1, T2*> &i_mapContainer)
{
	for (auto iter = i_mapContainer.begin(); iter != i_mapContainer.end(); )
	{
		i_mapContainer.erase( iter++ );
	}
	i_mapContainer.clear();
}
