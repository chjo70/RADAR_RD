#include "StdAfx.h"
#include "CommonMngr.h"

#define DF_OPER_ID_BORDER_FOR_LINK_TWO 10000
//////////////////////////////////////////////////////////////////////////
/*!
 * @brief     ������
 * @param     void
 * @return    void
 * @version   0.0.1
 * @exception
 * @author    ������ (jeongnam.lee@lignex1.com)
 * @date      2013-06-20 ���� 4:04 
 * @warning   
 */
CCommonMngr::CCommonMngr(void) :
m_hCommIF(CGRCommIF::GetInstance()),	
	m_bIsServerMode(false),
	m_bIsReplayMode(false)
{
	m_strIAM = "";

	m_unLastSigID = 0;

}
//////////////////////////////////////////////////////////////////////////
/*!
 * @brief     �Ҹ���
 * @param     void
 * @return    void
 * @version   0.0.1
 * @exception
 * @author    ������ (jeongnam.lee@lignex1.com)
 * @date      2013-06-20 ���� 4:03 
 * @warning   
 */
CCommonMngr::~CCommonMngr(void)
{

}

/*!
 * @def		float�� ���� network endian�� host endian���� �����Ѵ�.
 * @brief   ó���� ���(float)
 */
float CCommonMngr::ntohf( float fSrc )
{
	float fDest;
	uint32_tt* p32;

	fDest = fSrc;

	p32 = (uint32_tt*)&fDest;

	*p32 = ntohl( *p32 );

	return fDest;
}

/*!
 * @def		float�� ���� host endian�� network endian���� �����Ѵ�.
 * @brief   ó���� ���(float)
 */
float CCommonMngr::htonf( float fSrc )
{
	return ntohf( fSrc );
}

/*!
 * @def		double�� ���� network endian�� host endian���� �����Ѵ�.
 * @brief   ó���� ���(float)
 */
float CCommonMngr::ntohd( double dblSrc )
{
	typedef union un
	{
		double dbl;
		uint32_tt ul[2];
	} _UN;

	_UN _tmpSrc, _dest;

	_tmpSrc.dbl = dblSrc;

	_dest.ul[1] = ntohl( _tmpSrc.ul[0] );
	_dest.ul[0] = ntohl( _tmpSrc.ul[1] );

	return float( _dest.dbl );
}

/*!
 * @def		float�� ���� host endian�� network endian���� �����Ѵ�.
 * @brief   ó���� ���(float)
 */
float CCommonMngr::htond( double dblSrc )
{
	return ntohd( dblSrc );
}

string CCommonMngr::GetCurrentTimeInfo()
{
	CString csCurrentTM = _T( "" );
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	csCurrentTM.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds);

	return (LPCTSTR)csCurrentTM;
}

void CCommonMngr::InitTimeStruct(tm& i_Time)
{
	i_Time.tm_hour = 0;
	i_Time.tm_isdst = 0;
	i_Time.tm_mday = 0;
	i_Time.tm_min = 0;
	i_Time.tm_mon = 0;
	i_Time.tm_sec = 0;
	i_Time.tm_wday = 0;
	i_Time.tm_yday = 0;
	i_Time.tm_year = 0;
}

UINT CCommonMngr::MakeFSigID()
{
//	Sleep( 10 );

	UINT unSigID = 0;
	char szSigID[12] = {0,};
	SYSTEMTIME _SystemTime;
	GetLocalTime( &_SystemTime );

	unSigID = (unsigned int)_SystemTime.wMilliseconds * 10;
	unSigID += (unsigned int)_SystemTime.wSecond * 10000;
	unSigID += (unsigned int)_SystemTime.wMinute * 1000000;
	unSigID += (unsigned int)_SystemTime.wHour * 100000000;

	if( m_unLastSigID == unSigID )
	{
		unSigID++;
	}
	else
	{
		;
	}

	m_unLastSigID = unSigID;

	return unSigID;
}

//////////////////////////////////////////////////////////////////////////
/*!
* @brief     ���� �α����� POS ������� ID (����)�� ��� �Լ�
* @param     void
* @return    unsigned short : ����� id (����)
* @version   0.0.1
* @exception �ش���� ����
* @author    ������ (jeongnam.lee@lignex1.com)
* @date      2013-07-02 ���� 6:29 
* @warning   
*/
unsigned short CCommonMngr::GetLogInOperatorId()
{
	// ����� ���� Ŭ�������� operator id ��ȣ�� ���;� �Ѵ�. �ϴ� �ϵ��ڵ�����.

	unsigned short rtn = 4;

	return rtn;
}

//2����_������(��)=================================================

void CCommonMngr::CStrToChar(CString &i_str, char *i_pch, int i_nCnt)
{
	if (i_nCnt<=0)
	{
		i_nCnt=i_str.GetLength();
	}
	alignStrCnt(i_str, i_nCnt);
	for(int i=0;i<i_nCnt;i++)
	{
		*i_pch++ = i_str[i];
	}
}

void CCommonMngr::CStrToUChar(CString &i_str, unsigned char *i_puch, int i_nCnt)
{
	if (i_nCnt<=0)
	{
		i_nCnt=i_str.GetLength();
	}
	alignStrCnt(i_str, i_nCnt);
	for(int i=0;i<i_nCnt;i++)
	{
		*i_puch++ = i_str[i];
	}
}

void CCommonMngr::alignStrCnt(CString& i_str, int i_nCnt)
{
	int i, nLength;

	nLength = i_str.GetLength();
	if(nLength < i_nCnt) {
		for(i=0;i<i_nCnt-nLength;i++)
		{
			i_str += " ";
		}
	}
	i_str.GetBuffer(i_nCnt);
	i_str.ReleaseBuffer(i_nCnt);
}

unsigned int CCommonMngr::MakeOPCode(unsigned char i_uchCMDCode, unsigned char i_uchSrc, unsigned char i_uchDest, unsigned char i_uchO, unsigned char i_uchClass)
{
	SOPCode stOPCode;
	stOPCode.uchSrc=i_uchSrc;
	stOPCode.uchDest=i_uchDest;
	stOPCode.uchCMDCode=i_uchCMDCode;
	stOPCode.uchClass=i_uchClass;
	stOPCode.uchEncrpytion=i_uchO;

	unsigned int nOPCode = (unsigned int)((stOPCode.uchSrc << 24) | (stOPCode.uchDest << 16) | (stOPCode.uchCMDCode << 8) | (stOPCode.uchEncrpytion << 4) | (stOPCode.uchClass));

	return nOPCode;
}