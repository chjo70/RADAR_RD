#pragma once

#ifdef MATHFUNCSDLL_EXPORTS
#define MATHFUNCSDLL_API __declspec(dllexport)

#else
#define MATHFUNCSDLL_API __declspec(dllimport)
#endif

#define WM_USER_LOGMSG				(7011)

//
// RADARDIR ������Ʈ ������ �ּ� ���븸 ����ü ���Ǹ� ������ ����.

#define LENGTH_OF_TASK_ID			(19+1)		//����ID ���ڿ� ���� (TBD)

#ifndef ENUM_BANDWIDTH_ENUM
#define ENUM_BANDWIDTH_ENUM
typedef enum {
	en5MHZ_BW=0,
	en50MHZ_BW,

} ENUM_BANDWIDTH ;
#endif

#ifndef SRxLOBData_STRUCT
#define SRxLOBData_STRUCT

#define MAX_FREQ_PRI_STEP				(16)
#define MAX_RADARNAME						(9+1)
typedef struct
{
	unsigned int uiLOBID;
	unsigned int uiABTID;
	unsigned int uiAETID;

	__time32_t tiContactTime;				// 32��Ʈ time_t �� �����ؾ� ��. 
	unsigned int tiContactTimems;

	int iSignalType;	

	float	fMeanDOA;										// [0.1��]
	float fMaxDOA;
	float fMinDOA;

	int iDIRatio;										// [1 %]

	int		iFreqType;
	int		iFreqPatternType;			
	float fFreqPatternPeriod;	  // [us]
	float fMeanFreq;										// [10KHz]
	float fMaxFreq;
	float fMinFreq;
	int		iFreqPositionCount;
	float fFreqSeq[MAX_FREQ_PRI_STEP];	// ���ļ� �ܰ�

	int		iPRIType;
	int		iPRIPatternType;
	float	fPRIPatternPeriod;							// [us]
	float	fMeanPRI;											// [1ns]
	float	fMaxPRI;
	float	fMinPRI;
	float	fPRIJitterRatio;							// [%]
	int		iPRIPositionCount;
	float	fPRISeq[MAX_FREQ_PRI_STEP];

	float	fMeanPW;											// 1ns
	float fMaxPW;
	float fMinPW;

	float fMeanPA;											// �������
	float fMaxPA;
	float fMinPA;

	int		iIsStorePDW;
	int		iNumOfPDW;
	int		iCollectorID;

	double	dRadarCollectionLatitude;
	double	dRadarCollectionLongitude;	

	char aucRadarName[MAX_RADARNAME];
	int iRadarModeIndex;
	int iThreatIndex;

	unsigned int uiSeqNum;
	char	aucTaskID[LENGTH_OF_TASK_ID];

}  SRxLOBData;
#endif

#ifndef _STR_LOBHEADER
#define _STR_LOBHEADER
typedef struct 
{
	int iNumOfLOB;

} SRxLOBHeader;
#endif

#define MAX_LOB_DATA			(100)

#ifndef _STR_LOBDATA
#define _STR_LOBDATA
typedef struct {
	SRxLOBHeader stLOBHeader;

	SRxLOBData stLOBData[MAX_LOB_DATA];

} STR_LOBDATA ;
#endif

#ifndef _STR_ABTHEADER
#define _STR_ABTHEADER
struct SRxABTHeader
{
	int iNumOfABT;

};
#endif

#ifndef _STR_ABTDATA_STRUCT
#define _STR_ABTDATA_STRUCT
typedef struct
{
	unsigned int uiABTID;
	unsigned int uiAETID;

	int iSignalType;

	unsigned int uiCoLOB;

	__time32_t tiFirstSeenTime;				// 32��Ʈ time_t �� �����ؾ� ��. 
	__time32_t tiLastSeenTime;

	int iValidity;

	int		iFreqType;
	int		iFreqPatternType;			
	float fFreqPatternPeriodMean;	  // [us]
	float fFreqPatternPeriodMin;	  // [us]
	float fFreqPatternPeriodMax;	  // [us]
	int		iFreqPositionCount;
	float fFreqSeq[MAX_FREQ_PRI_STEP];	// ���ļ� �ܰ�
	float fMeanFreq;										// [10KHz]
	float fMaxFreq;
	float fMinFreq;

	int		iPRIType;
	int		iPRIPatternType;
	float	fPRIPatternPeriodMean;							// [us]
	float	fPRIPatternPeriodMin;							// [us]
	float	fPRIPatternPeriodMax;							// [us]
	float	fMeanPRI;											// [1ns]
	float	fMaxPRI;
	float	fMinPRI;
	float	fPRIJitterRatio;							// [%]
	int		iPRIPositionCount;
	float	fPRISeq[MAX_FREQ_PRI_STEP];

	float	fMeanPW;											// 1ns
	float fMaxPW;
	float fMinPW;

	float fMeanPA;											// �������
	float fMaxPA;
	float fMinPA;

	unsigned int uiTotalOfPDW;

	int iRadarModeIndex;
	int iThreatIndex;

	int iPEValid;
	double dLatitude;							// [deg]
	double dLongitude;							// [deg]
	float fCEP;										// [m]
	float fMajorAxis;							// [m]
	float fMinorAxis;							// [m]
	float fTheta;									// [0.1��]
	float fDistanceErrorOfThreat;	// [m]

	__time32_t tiFinalAlarmTime;

	int iStat;

	char aucRadarName[MAX_RADARNAME];

}  SRxABTData;
#endif

#define MAX_ABT_DATA			(100)

#ifndef _STR_ABTDATA
#define _STR_ABTDATA
typedef struct {
	SRxABTHeader stABTHeader;

	SRxABTData stABTData[MAX_ABT_DATA];

} STR_ABTDATA ;
#endif


//////////////////////////////////////////////////////////////////////////
// ������ ���

/* �� ��������� ������Ʈ ������ �����ϰ� �Ʒ� �Լ��� ȣ���Ϸ��� �ϴ� �ҽ��� �Ʒ��� ���� include �Ѵ�.

#include "RadarAnlAlgorithm.h"

:
:

RadarAnlAlgotirhm::RadarAnlAlgotirhm::Init();

:
:

RadarAnlAlgotirhm::RadarAnlAlgotirhm::Close();

*/

//////////////////////////////////////////////////////////////////////////
// ���� ���

/* 1. ���۽� �ѹ��� ȣ���ϸ� �˴ϴ�.
	 ���� ���α׷����� �ʱ�ȭ�ÿ� �Ʒ� ��ƾ�� �ѹ� ȣ���ϸ� �˴ϴ�.
   RadarAnlAlgotirhm::RadarAnlAlgotirhm::Init();

	 2. ���α׷� ����� �ѹ��� ȣ���ϸ� �˴ϴ�.
	 RadarAnlAlgotirhm::RadarAnlAlgotirhm::Close();

	 3. ���� ���� ȣ��� STR_LOBDATA ������ �Ʒ��� ���� �����Ͱ� �־�� �մϴ�.

STR_LOBDATA stLOBData;

typedef struct 
{
	int iNumOfLOB;													<---- LOB ����, �ִ� 100�� �����̴�.
} SRxLOBHeader;

typedef struct {
	SRxLOBHeader stLOBHeader;								<---- LOB ��� ����ü
	SRxLOBData stLOBData[100];							<---- LOB ������

} STR_LOBDATA ;

RadarAnlAlgotirhm::RadarAnlAlgotirhm::Start( & stLOBData );

		4. �м� ����� �Ʒ� �Լ��� ȣ���Ͽ� LOB ������ �����͸� ��´�.
		
		STR_LOBDATA stResLOBData;
		STR_ABTDATA stResABTData;

		RadarAnlAlgotirhm::RadarAnlAlgotirhm::GetLOBData( & stResLOBData );			<---- LOB ���
		RadarAnlAlgotirhm::RadarAnlAlgotirhm::GetABTData( & stResABTData );			<---- ABT ���

		stResLOBData ����ü�� stResABTData ����ü�� ���� �̿��ϸ� �˴ϴ�.

		*���� ���� : ���� ���� ȣ��� LOB ������ 100 �� �ʰ��̸� ���� ������ �ߴ��Ѵ�. ���� ������ ���� ...
								 ���� �Է� LOB ����� 10�� �̸� LOB ���� ����� 10�� �̾�� �ϰ� ABT ����� 10 �� �̸��̾�� ��.
								 ����, LOB Ŭ�����͸��� �߰��ϰ� �Ǹ� LOB ���� ����� �� ��� ������ 10�� �̻�/�ִ� 10�� �̸� �� �� �ֽ��ϴ�.

		5. CED �� EOB ���̺귯���� ��������� ���� ������ CED/EOB�� �ε��ؾ� �Ѵٴ� �÷��׸� �������ִ� �Լ��̴�.
		   CED �� EOB ���̺귯���� ����� ��� ��� ����Ʈ����� ���̴� �м����� �˷��ְ� ���̴� �м������� �� �Լ��� ȣ���ؾ� �մϴ�.

		RadarAnlAlgotirhm::RadarAnlAlgotirhm::UpdateCEDEOBLibrary();						<---- CED/EOB ������Ʈ�� �˷��ش�.


		6. ��� �ʱ�ȭ ��� �ʱ�ȭ�ÿ� ���ü �� ��/LOB ��ȣ�� 1�� ���� ������ϸ� ���� ���� ���̺� �ʱ�ȭ�Ͽ� ó������ �м��� �����Ѵ�.
		RadarAnlAlgotirhm::RadarAnlAlgotirhm::SWInit();													<---- ��� �ʱ�ȭ�� �����Ѵ�.

*/

namespace RadarAnlAlgotirhm
{
	// This class is exported from the MathFuncsDll.dll
	class RadarAnlAlgotirhm
	{
	public: 
		static MATHFUNCSDLL_API void Init( HWND hWnd=0, bool bDBThread=true, bool bLocal=false );
		static MATHFUNCSDLL_API void Close();

		static MATHFUNCSDLL_API void Start( STR_LOBDATA *pLOBData );
		static MATHFUNCSDLL_API void SWInit();
		static MATHFUNCSDLL_API void UpdateCEDEOBLibrary();

		static MATHFUNCSDLL_API bool GetLOBData( STR_LOBDATA *pLOBData );
		static MATHFUNCSDLL_API bool GetABTData( STR_ABTDATA *pABTData );

	};
}



