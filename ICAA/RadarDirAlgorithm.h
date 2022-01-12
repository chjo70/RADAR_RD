#pragma once

#ifdef MATHFUNCSDLL_EXPORTS
#define MATHFUNCSDLL_API __declspec(dllexport)

#else
#define MATHFUNCSDLL_API __declspec(dllimport)
#endif


#define WM_USER_LOGMSG				(7011)


//
// RADARDIR ������Ʈ ������ �ּ� ���븸 ����ü ���Ǹ� ������ ����.

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

#define LENGTH_OF_TASK_ID			(19+1)		//����ID ���ڿ� ���� (TBD)

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

	__time32_t tiContactTime;				// 32��Ʈ time_t �� �����ؾ� ��. 
	unsigned int tiContactTimems;

	int iSignalType;	

	float	fMeanDOA;										// [0.1��]
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
	float fFreqSeq[MAX_FREQ_PRI_STEP];	// ���ļ� �ܰ�

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

#endif

//////////////////////////////////////////////////////////////////////////
// ������ ���

/* �� ��������� ������Ʈ ������ �����ϰ� �Ʒ� �Լ��� ȣ���Ϸ��� �ϴ� �ҽ��� �Ʒ��� ���� include �Ѵ�.

#include "RadarDirAlgorithm.h"

:
:

RadarDirAlgotirhm::RadarDirAlgotirhm::Init();

:
:

RadarDirAlgotirhm::RadarDirAlgotirhm::Close();

*/

//////////////////////////////////////////////////////////////////////////
// ���� ���

/* 1. ���۽� �ѹ��� ȣ���ϸ� �˴ϴ�.
	 ���� ���α׷����� �ʱ�ȭ�ÿ� �Ʒ� ��ƾ�� �ѹ� ȣ���ϸ� �˴ϴ�.
   RadarDirAlgotirhm::RadarDirAlgotirhm::Init();

	 2. ���α׷� ����� �ѹ��� ȣ���ϸ� �˴ϴ�.
	 RadarDirAlgotirhm::RadarDirAlgotirhm::Close();


	 3. ��ȣ �м��� PDWDATA ������ �Ʒ��� ���� �����Ͱ� �־�� �մϴ�.

typedef struct {
	char aucTaskID[LENGTH_OF_TASK_ID];			<---- ���� ����
	unsigned int iIsStorePDW;								<---- ���� ����. �ϴ� �׻� 1�� ��.
	int iCollectorID;												<---- ������ ��ġ. �ҹ�� �мҴ� 1, ���� ���żҴ� 2, �ź� ���̴�#1/�ź� ���̴�#2 �� 3/4 �� �Ҵ�
	ENUM_BANDWIDTH enBandWidth;							<---- ������ PDW�� �������� ���� �뿪���� ����

	UINT count;															<---- PDW ���� ����
	_PDW stPDW[_MAX_PDW];										<---- PDW ������

} STR_PDWDATA ;

RadarDirAlgotirhm::RadarDirAlgotirhm::Start( & stPDWData );

		4. �м� ����� �Ʒ� �Լ��� ȣ���Ͽ� LOB ������ �����͸� ��´�.
		
		int nCoLOB=RadarDirAlgotirhm::RadarDirAlgotirhm::GetCoLOB();							<-- LOB ����
		SRxLOBData *pLOBData=RadarDirAlgotirhm::RadarDirAlgotirhm::GetLOBData();	<--- LOB ������

		pLOBData[0] �� ù��° LOB ������, pLOBData[1] �� �ι�° LOB ������ ������ ����ϸ� �˴ϴ�. 

		*���� ���� : PDWDATA ����ü�� ���� ������ ���ų� PDW ���������� 4096 �� �ʰ��Ǹ� Start()���� �ٷ� �����Ѵ�. LOB ������ 0 ���� �����Ѵ�.

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



