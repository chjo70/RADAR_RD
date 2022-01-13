#pragma once

#ifdef MATHFUNCSDLL_EXPORTS
#define MATHFUNCSDLL_API __declspec(dllexport)

#else
#define MATHFUNCSDLL_API __declspec(dllimport)
#endif


#define WM_USER_LOGMSG				(7011)


//
// RADARDIR 프로젝트 내에서 최소 내용만 구조체 정의를 복사한 것임.

#ifndef MATHFUNCSDLL_EXPORTS

#ifndef _LOG_TYPE_
#define _LOG_TYPE_
enum LogType {
	enDebug=0,
	enNormal,
	enLineFeed,
	enError,
	enEnd,
};
#endif

#define LENGTH_OF_TASK_ID			(19+1)		//과제ID 문자열 길이 (TBD)

#define _MAX_PDW							(4096)

#ifndef _PDW_STRUCT
#define _PDW_STRUCT
typedef struct {
	long long int llTOA;

	int iFreq;
	int iPulseType;
	int iPA;
	int iPW;
	int iPFTag;
	int iAOA;

	float fPh1;
	float fPh2;
	float fPh3;
	float fPh4;

} _PDW;
#endif

#ifndef ENUM_BANDWIDTH_ENUM
#define ENUM_BANDWIDTH_ENUM
typedef enum {
	en5MHZ_BW=0,
	en50MHZ_BW,

} ENUM_BANDWIDTH ;
#endif

#ifndef _STR_PDWDATA
#define _STR_PDWDATA
typedef struct {
	unsigned char	aucTaskID[LENGTH_OF_TASK_ID];
	unsigned int iIsStorePDW;
	int iCollectorID;
	ENUM_BANDWIDTH enBandWidth;

	UINT count;
	_PDW stPDW[_MAX_PDW];

} STR_PDWDATA ;
#endif

#define MAX_RADARNAME						(9+1)
#define MAX_FREQ_PRI_STEP				(16)

#ifndef SRxLOBData_STRUCT
#define SRxLOBData_STRUCT
typedef struct
{
	unsigned int uiLOBID;
	unsigned int uiABTID;
	unsigned int uiAETID;

	__time32_t tiContactTime;				// 32비트 time_t 로 선언해야 함. 
	unsigned int tiContactTimems;

	int iSignalType;	

	float	fMeanDOA;										// [0.1도]
	float fMaxDOA;
	float fMinDOA;

	int iDIRatio;										// [1 %]

	int iFreqType;
	int	iFreqPatternType;			
	float fFreqPatternPeriod;	  // [us]
	float fMeanFreq;										// [10KHz]
	float fMaxFreq;
	float fMinFreq;
	int	iFreqPositionCount;
	float fFreqSeq[MAX_FREQ_PRI_STEP];	// 주파수 단값

	int	iPRIType;
	int	iPRIPatternType;
	float fPRIPatternPeriod;							// [us]
	float fMeanPRI;											// [1ns]
	float	fMaxPRI;
	float	fMinPRI;
	float	fPRIJitterRatio;							// [%]
	int		iPRIPositionCount;
	float	fPRISeq[MAX_FREQ_PRI_STEP];

	float	fMeanPW;											// 1ns
	float fMaxPW;
	float fMinPW;

	float fMeanPA;											// 기존대로
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

#endif

//////////////////////////////////////////////////////////////////////////
// 컴파일 방법

/* 이 헤더파일을 프로젝트 폴더에 복사하고 아래 함수를 호출하려고 하는 소스에 아래와 같이 include 한다.

#include "RadarDirAlgorithm.h"

:
:

RadarDirAlgotirhm::RadarDirAlgotirhm::Init();

:
:

RadarDirAlgotirhm::RadarDirAlgotirhm::Close();

*/

//////////////////////////////////////////////////////////////////////////
// 실행 방법

/* 1. 시작시 한번만 호출하면 됩니다.
	 응용 프로그램에서 초기화시에 아래 루틴을 한번 호출하면 됩니다.
   RadarDirAlgotirhm::RadarDirAlgotirhm::Init();

	 2. 프로그램 종료시 한번만 호출하면 됩니다.
	 RadarDirAlgotirhm::RadarDirAlgotirhm::Close();


	 3. 신호 분석시 PDWDATA 구조에 아래와 같이 데이터가 있어야 합니다.

typedef struct {
	char aucTaskID[LENGTH_OF_TASK_ID];			<---- 과제 정보
	unsigned int iIsStorePDW;								<---- 저장 여부. 일단 항상 1로 함.
	int iCollectorID;												<---- 수집소 위치. 소방대 분소는 1, 관제 수신소는 2, 신불 레이더#1/신불 레이더#2 은 3/4 로 할당
	ENUM_BANDWIDTH enBandWidth;							<---- 수집한 PDW의 수집과제 에서 대역폭을 저장

	UINT count;															<---- PDW 수집 개수
	_PDW stPDW[_MAX_PDW];										<---- PDW 데이터

} STR_PDWDATA ;

RadarDirAlgotirhm::RadarDirAlgotirhm::Start( & stPDWData );

		4. 분석 결과는 아래 함수를 호출하여 LOB 개수와 데이터를 얻는다.
		
		int nCoLOB=RadarDirAlgotirhm::RadarDirAlgotirhm::GetCoLOB();							<-- LOB 개수
		SRxLOBData *pLOBData=RadarDirAlgotirhm::RadarDirAlgotirhm::GetLOBData();	<--- LOB 데이터

		pLOBData[0] 로 첫번째 LOB 데이터, pLOBData[1] 로 두번째 LOB 데이터 등으로 사용하면 됩니다. 

		*주의 사항 : PDWDATA 구조체에 과제 정보가 없거나 PDW 수집개수가 4096 개 초과되면 Start()에서 바로 리턴한다. LOB 개수는 0 으로 리턴한다.

*/
namespace RadarDirAlgotirhm
{
	// This class is exported from the MathFuncsDll.dll
	class RadarDirAlgotirhm
	{
	public: 
		static MATHFUNCSDLL_API void Init( HWND hWnd=0, bool bLocal=false );
		static MATHFUNCSDLL_API void SetMute( bool bEnable );
		static MATHFUNCSDLL_API void SWInit();

		static MATHFUNCSDLL_API void Close();

		static MATHFUNCSDLL_API void Start( STR_PDWDATA *pPDWData );

		static MATHFUNCSDLL_API int GetCoLOB();
		static MATHFUNCSDLL_API SRxLOBData *GetLOBData();

		static MATHFUNCSDLL_API void Log( int nType, const char *fmt, ... );
	};

}



