#pragma once
#ifndef _H_EL_MSG_DEFN
#define _H_EL_MSG_DEFN
#pragma once

#include <vector>
using namespace std;

//�߰���
template <class T>
static T SwapEndian (T* tObjp)
{
	UCHAR *puchMem=reinterpret_cast<UCHAR*>(tObjp);
	reverse(puchMem, puchMem+sizeof(T));
	return *tObjp;
}

#define ENDIAN16_C(x)		((UINT16)(x&0x00ff)<<8) | ((UINT16)(x&0xff00)>>8)
#define ENDIAN32_C(x)     (x & 0x000000FF) << 24 | (x & 0x0000FF00) << 8 | \
	(x & 0xFF000000) >> 24 | (x & 0x00FF0000) >> 8
#define ENDIAN64_C(x) (DWORD64) ( \
	(((DWORD64 (x) <<56) & 0xff00000000000000)) | \
	(((DWORD64 (x) <<40) & 0x00ff000000000000)) | \
	(((DWORD64 (x) <<24) & 0x0000ff0000000000)) | \
	(((DWORD64 (x) <<8)  & 0x000000ff00000000)) | \
	(((DWORD64 (x) >>8)  & 0x00000000ff000000)) | \
	(((DWORD64 (x) >>24) & 0x0000000000ff0000)) | \
	(((DWORD64 (x) >>40) & 0x000000000000ff00)) | \
	(((DWORD64 (x) >>56) & 0x00000000000000ff)))

#define ENDIAN16(x)		(unsigned short)( ((unsigned short)x & 0x00ff) << 8 | ((unsigned short)x & 0xff00)>>8)  
#define ENDIAN32(x)  (unsigned int)(  ((unsigned int)x & 0x000000FF) << 24 | ((unsigned int)x & 0x0000FF00) << 8 | \
	((unsigned int)x & 0xFF000000) >> 24 | ((unsigned int)x & 0x00FF0000) >> 8 )

static void AllEndian32(void* i_pData, UINT i_nSize)
{
	DWORD dwValue=0;
	UINT nStepSize=sizeof(DWORD);
	for(UINT i=0; i<i_nSize; i+=nStepSize)  //#FA_C_PotentialUnboundedLoop_T2
	{
		memcpy(&dwValue, &((BYTE*)i_pData)[i], nStepSize);
		dwValue=ENDIAN32(dwValue);
		memcpy(&((BYTE*)i_pData)[i], &dwValue, nStepSize);
	}
}

static void AllEndian64(void *i_pData, UINT i_nSize)
{
	unsigned __int64 un64Value=0;
	UINT nStepSize=sizeof(unsigned __int64);
	for(UINT i=0; i<i_nSize; i+=nStepSize)  //#FA_C_PotentialUnboundedLoop_T2
	{
		memcpy(&un64Value, &((BYTE*)i_pData)[i], nStepSize);
		un64Value=_byteswap_uint64(un64Value);
		memcpy(&((BYTE*)i_pData)[i], &un64Value, nStepSize);
	}
}

#define IP_TF			10
#define SYS_CLR_EQUIP1	14
#define SYS_CLR_EQUIP2	24
#define SYS_CLR_EQUIP3	34

#define CMDCODE_UDP								(0x00)

//������� (0x0X)
#define CMDCODE_DF_TX_INIT_SYS_RESULT			(0x01)  //�ʱ�ȭ ���
#define CMDCODE_DF_TX_SW_VERSION_RSLT			(0x02)	//SW���� ���
#define CMDCODE_DF_TX_PBIT_RSLT					(0x03)  //�ʱ���ü���� ���
#define CMDCODE_DF_TX_IBIT_RSLT					(0x04)  //�������ü���� ���
#define CMDCODE_DF_TX_CHNEL_CORECT				(0x05)	//ä�κ��� �������

//��� ����(0x1X)
#define CMDCODE_DF_TX_STARTCOLLECT_RSLT			(0x11)	//�������� ���
#define CMDCODE_DF_TX_EQUITREST_RSLT			(0x12)	//��񸮼� ���
#define CMDCODE_DF_TX_CTRLRECEIVER_RSLT			(0x13)	//���ű����� ���
#define CMDCODE_DF_TX_RADRSYSVAL_RSLT			(0x13)	//���̴���Ž �ý��ۺ��� ���� ���

//���� ����(0x2X)
#define CMDCODE_DF_RX_TASK_REQ_LIST				(0x21)  //�������� �䱸
#define CMDCODE_DF_TX_TASK_REV_RSLT				(0x21)  //�������� ���� ���
#define CMDCODE_DF_TX_INHIBITFREQ_SET_RSLT		(0x22)  //���Ź������ļ� ���� ���
#define CMDCODE_DF_RX_SYSTEM_SET_INFO			(0x23)  //�ý��� ����

//������ ����(0x3X)
#define CMDCODE_DF_TX_SEND_PDW					(0x32)  //PDW ������ ����
#define CMDCODE_DF_TX_SEND_LOB					(0x31)  //LOB ����

//PDW �߻��� ����
#define CMDCODE_DF_TX_PDW_REQ					(0x32)  //PDW ��û
#define CMDCODE_DF_RX_PDW_RSLT					(0x32)  //PDW ��û ���
#define CMDCODE_DF_RX_PDW_COLECT_DONE_STAT		(0x33)  //�����Ϸ� ���� ���� ���
#define CMDCODE_DF_TX_PDW_COLECT_SET			(0x21)	//��������
#define CMDCODE_DF_TX_PDW_SYS_SET				(0x22)	//�ý��� ����
#define CMDCODE_DF_TX_PDW_SPTR_REQ				(0x23)  //����Ʈ�� ��û
#define CMDCODE_DF_TX_PDW_IQDATA_REQ			(0x24)  //IQ ��û
#define CMDCODE_DF_RX_PDW_IQDATA_RSLT			(0x24)  //IQ ��û ���

//V/UHF �������
#define CMDCODE_UHF_TX_SET_CAL_FREQ				(0x81)  //���׳��й�� �������ļ� ����
#define CMDCODE_UHF_TX_SET_CAL_SG_OFF			(0x83)	//���׳��й�� ������ȣ OFF
#define CMDCODE_UHF_TX_SET_ANT_MODE				(0x84)	//���׳��й�� ������� ����

//LOB ����
#define CMDCODE_LOB_SEND_TO_DP				(0x31) 
//#define CMDCODE_TASK_REC_RESLT_TO_OPER			(0x21) 

#define RX_MAX_TASK					(100)
#define LENGTH_OF_TASK_ID			(19+1)		//����ID ���ڿ� ���� (TBD)

#define RX_MAX_PDW					(4096)
#define MAX_RADARNAME						(9+1)
#define MAX_FREQ_PRI_STEP				(32)
#define LENGTH_OF_TASK_ID			(19+1)		//����ID ���ڿ� ���� (TBD)
//#define MAX_PRI_PER_GROUP				(32)
//#define MAX_PADIFF_PER_GROUP		(32)

enum ACQ_START_REQ
{
	ACQ_START = 0,
	ACQ_STOP,
	ACQ_RESTART,
	ACQ_PAUSE
};

#define LENGTH_OF_TASK_ID			(19+1)		//����ID ���ڿ� ���� (TBD)

#define RX_MAX_PDW					(4096)


#pragma pack(push,1)

typedef struct {
	unsigned char uchClass: 4;
	unsigned char uchEncrypt: 4;
	unsigned char uchCMDCode;
	unsigned char uchDest;
	unsigned char uchSrc;

} SEL_BITMAP_OPCODE ;


typedef union {
	unsigned int w32;
	SEL_BITMAP_OPCODE bData;

} UEL_BITMAP_OPCode ;

struct SRxSpectrumData
{
	unsigned int uiAcqID;
	unsigned int uiAcqDataNum;
	USHORT usSprData[8192];

	SRxSpectrumData() :
	uiAcqID(0),
		uiAcqDataNum(0)
	{
		memset(usSprData, 0, 8192*sizeof(USHORT));
	}
};

struct SRxInitReqRslt
{
	unsigned int uiInitDFRslt; //0 : ������, 1: ����, 2: �̽ǽ�
	unsigned int uiInitReciverRslt; //0 : ������, 1: ����, 2: �̽ǽ�
	unsigned int uiInitPDWRslt; //0 : ������, 1: ����, 2: �̽ǽ�

	SRxInitReqRslt() :
	uiInitDFRslt(1),
		uiInitReciverRslt(1),
		uiInitPDWRslt(1)
	{
	}
};

struct SRxBITReqRslt
{
	unsigned int uiInitDFRslt; //0 : ������, 1: ����, 2: �̽ǽ�
	unsigned int uiRcvCH1;  //0 : ������, 1: ����, 2: �̽ǽ�
	unsigned int uiRcvCH2; 
	unsigned int uiRcvCH3; 
	unsigned int uiRcvCH4; 
	unsigned int uiRcvCH5; 
	unsigned int uiRcvLO1; 
	unsigned int uiRcvLO2; 
	unsigned int uiRcvSYN; 
	unsigned int uiInitPDWRslt; //0 : ������, 1: ����, 2: �̽ǽ�

	SRxBITReqRslt() :
	uiInitDFRslt(1),
		uiRcvCH1(1),
		uiRcvCH2(1),
		uiRcvCH3(1),
		uiRcvCH4(1),
		uiRcvCH5(1),
		uiRcvLO1(1),
		uiRcvLO2(1),
		uiRcvSYN(1),
		uiInitPDWRslt(1)
	{
	}
};

struct SRxSpectrumCmd
{
	unsigned int uiReqSprtinfo; //���� : 1, ���� :2
	unsigned char	aucTaskID[LENGTH_OF_TASK_ID];

	SRxSpectrumCmd() :
	uiReqSprtinfo(0)
	{
	}
};

struct SRxSendCtrlResult{    //��� ����
	unsigned int uiResult;
	SRxSendCtrlResult():
	uiResult(0)
	{

	}
};

struct SRxTSGCtrlResult{    //�������� ���
	int iResult;
	SRxTSGCtrlResult():
	iResult(0)
	{

	}
};

struct STxAcqStartRequest{   //�������� �䱸
	UINT	uiMode;
	UINT	uiStartLOBId;
	STxAcqStartRequest() :
	uiMode(0),
		uiStartLOBId(0)
	{
	}
};

struct STxAcqStartRslt{   //�������� ���
	UINT	uiMode;
	UINT	uiResult;
	STxAcqStartRslt() :
	uiMode(0),
		uiResult(0)
	{
	}
};

struct STxChCorrectRslt{   //�������� ���	
	UINT	uiResult;
	STxChCorrectRslt() :
	uiResult(0)
	{
	}
};

//struct STxAcqTraskData_old{   //���� ���� ����
//	unsigned char	aucTaskID[LENGTH_OF_TASK_ID];
//	unsigned int	uiNBDRBandWidth;
//	int				iRxThresholdValue;
//	unsigned int	uiAcquisitionTime;
//	unsigned int	uiNumOfAcqPuls;
//	unsigned int	uiFreq;
//
//	STxAcqTraskData() :
//	uiNBDRBandWidth(0),
//		iRxThresholdValue(0),
//		uiAcquisitionTime(0),
//		uiNumOfAcqPuls(0),
//		uiFreq(0)
//	{
//	}
//};

struct STxAcqTraskData{   //���� ���� ����
	unsigned char	aucTaskID[LENGTH_OF_TASK_ID];
	unsigned int	uiNBDRBandWidth;	
	unsigned int	uiAcquisitionTime;
	unsigned int	uiNumOfAcqPuls;
	unsigned int	uiFreq;
	int				iRxThresholdValue1;
	int				iRxThresholdValue2;
	int				iRxThresholdValue3;
	int				iRadarEqGain1;
	int				iRadarEqGain2;
	int				iRadarEqGain3;
	unsigned int	uiRvcUseStat1;
	unsigned int	uiRvcUseStat2;
	unsigned int	uiRvcUseStat3;

	STxAcqTraskData() :
	uiNBDRBandWidth(0),		
		uiAcquisitionTime(0),
		uiNumOfAcqPuls(0),
		uiFreq(0),
		iRxThresholdValue1(0),
		iRxThresholdValue2(0),
		iRxThresholdValue3(0),
		iRadarEqGain1(0),
		iRadarEqGain2(0),
		iRadarEqGain3(0),
		uiRvcUseStat1(0),
		uiRvcUseStat2(0),
		uiRvcUseStat3(0)
	{
	}
};

struct STxAcqTraskRequest{   //���� ���� �䱸
	unsigned int	uiNumOfTask;
	STxAcqTraskData		stTaskData[RX_MAX_TASK];

	STxAcqTraskRequest() :	
	uiNumOfTask(0)
	{
	}
};



typedef struct
{ 
	char aucTaskID[LENGTH_OF_TASK_ID];
	unsigned int uiLOBID;
	unsigned int uiABTID;
	unsigned int uiAETID;

	int			 tiContactTime;				// 32��Ʈ time_t �� �����ؾ� ��. 
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
	int		iPRIPatternPeriod;							// [us]
	float	fMeanPRI;											// [1ns]
	float	fMaxPRI;
	float	fMinPRI;
	float	fPRIJitterRatio;							// [%]
	int		iPRIPositionCount;
	int 	iPRISeq[MAX_FREQ_PRI_STEP];

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

}  SRxDFLOBData;


struct STxReqRadarSysVal{				//���̴���Ž �ý��ۺ��� �����䱸
	unsigned int	uiMinPDWForLOB;
	unsigned int	uiAlarmMinCollecting;
	unsigned int	uiDensityParameter;
	unsigned int	uireserved_1;
	unsigned int	uireserved_2;
	unsigned int	uireserved_3;
	unsigned int	uireserved_4;
	unsigned int	uireserved_5;
	unsigned int	uireserved_6;	

	STxReqRadarSysVal() :
	uiMinPDWForLOB(0),
		uiAlarmMinCollecting(0),
		uiDensityParameter(0),
		uireserved_1(0),
		uireserved_2(0),
		uireserved_3(0),
		uireserved_4(0),
		uireserved_5(0),
		uireserved_6(0)
	{
	}
};


struct STxSystemVariable{				//�ý��� ��������
	unsigned int	uiCollectorID;
	unsigned int	uiRxThresholdMode;
	unsigned int	uiCWDecisionPW;
	unsigned int	uiCWChoppinginterval;
	unsigned int	uiSignalCheckTime;
	unsigned int	uiSignalCheckPulseNumber;
	unsigned int	uiFilterType;
	unsigned int	uilsCWFilterApply;
	unsigned int	uilsFreqFilterApply;
	unsigned int	uiFilterMinFreq;
	unsigned int	uiFilterMaxFreq;
	unsigned int	uilsPAFilterApply;
	int				iFilterMinPA;
	int				iFilterMaxPA;
	unsigned int	uilsPWFilterApply;
	unsigned int	uiFilterMinPW;
	unsigned int	uiFilterMaxPW;

	STxSystemVariable() :
	uiCollectorID(0),
		uiRxThresholdMode(0),
		uiCWDecisionPW(0),
		uiCWChoppinginterval(0),
		uiSignalCheckTime(0),
		uiSignalCheckPulseNumber(0),
		uiFilterType(0),
		uilsCWFilterApply(0),
		uilsFreqFilterApply(0),
		uiFilterMinFreq(0),
		uiFilterMaxFreq(0),
		uilsPAFilterApply(0),
		iFilterMinPA(0),
		iFilterMaxPA(0),
		uilsPWFilterApply(0),
		uiFilterMinPW(0),
		uiFilterMaxPW(0)	
	{
	}
};

struct STxUHFSetCalFreq{   //���׳��й�� �������ļ� ����
	unsigned int	uiCal_Freq;

	STxUHFSetCalFreq() :	
	uiCal_Freq(0)
	{
	}
};

struct STxUHFSetCalFreqReq{   //���׳��й�� �������ļ� ������ ����
	unsigned int	uiCal_Freq_Req;

	STxUHFSetCalFreqReq() :	
	uiCal_Freq_Req(0)
	{
	}
};

struct STxUHFSetAntMode{   //���׳��й�� ������� ����
	unsigned char	ucCalAntMode;

	STxUHFSetAntMode() :	
	ucCalAntMode(0)
	{
	}
};


struct STxUHFSetAntModeReq{   //���׳��й�� ������� ����
	unsigned char	ucCalAntMode_Req;

	STxUHFSetAntModeReq() :	
	ucCalAntMode_Req(0)
	{
	}
};


typedef struct {									//��������
	unsigned char aucTaskID[LENGTH_OF_TASK_ID];
	unsigned int uiNBDRBandwidth;
	int iRxThresholdValue;
	unsigned int uiAcquisitionTime;
	unsigned int uiNumOfAcqPulse;
	unsigned int uiMode;
} _PDW_COLECTSET;

typedef struct {									//�������� ����
	unsigned char aucTaskID[LENGTH_OF_TASK_ID];
	unsigned int uiColectSetAck;	
} _PDW_COLECTSET_ACK;

typedef struct {						//�����Ϸ� ���� ��û
	unsigned int uiDoneStatus;			//1:��û
} _PDW_COLECTDONE_STAT_REQ;

typedef struct {						//�����Ϸ� ���� ���
	unsigned int uiDoneStatus;			//0:������, 1: �ð�, 2:����, 0xF: ����
	unsigned int iNumOfPDW;
	unsigned int iNoiseLevel_1;
	unsigned int iNoiseLevel_2;
	unsigned int iNoiseLevel_3;
	unsigned int iNoiseLevel_4;
	unsigned int iNoiseLevel_5;
} _PDW_COLECTDONE_STAT;

typedef struct {
	unsigned char aucTaskID[LENGTH_OF_TASK_ID];
	int iPDWCnt;			
} _PDW_DATA_Req;

typedef struct {
	long long int llTOA;//PDW������ ����	
	int iFreq;			//�������ļ�
	int iPA;			//��ȣ����
	int iPulseType;		//��ȣ����
	int iPW;			//�޽���	
	int iPFTag;			//��ó������
	int iph[5];
} PDW_DATA;

typedef struct {
	unsigned char aucTaskID[LENGTH_OF_TASK_ID];
	int iCollectorID;
	UINT count;
	PDW_DATA stPDW[RX_MAX_PDW];
} RX_STR_PDWDATA ;

typedef struct {
	unsigned int iProhibitMinFreq;	
	unsigned int iProhibitMaxFreq;		
} InhibitFreq_DATA;

typedef struct {
	unsigned int	 uiNumOfProhibitFreq;	
	InhibitFreq_DATA stInhbtFreq[RX_MAX_PDW];
} RX_STR_InhibitDATA ;


typedef struct {
	long long int llTOA;
	//unsigned int uiPDWID;
	int iFreq;
	int iPulseType;
	int iPA;
	int iPW;
	int iPFTag;
	int iAOA;
} PDW;

typedef struct {
	unsigned char aucTaskID[LENGTH_OF_TASK_ID];
	unsigned int iIsStorePDW;
	int iCollectorID;
	//ENUM_BANDWIDTH enBandWidth; //�߰�

	UINT count;
	PDW stPDW[RX_MAX_PDW];

} STR_PDWDATA_LIST ;



typedef struct {	
	int iFreq;			
	float fph[5];
} _CH_CORRECT;

typedef struct stCheckDataLIst
{
	int		    iCnt; 
	float       fphase[5];
}STCheckDataLIst;

typedef struct stAutoFreqDataLIst
{
	int		    index; 
	int         freq;
}STAutoFreqDataLIst;

typedef struct ST_DensityData{
	unsigned char	aucTaskID[LENGTH_OF_TASK_ID];	
	unsigned int	uiFreq;
	short			fDensity_1;
	short			fDensity_2;
	short			fDensity_3;

	ST_DensityData()
	{
		memset(aucTaskID, 0, sizeof(aucTaskID));

		uiFreq = 0;
		fDensity_1 = 0;
		fDensity_2 = 0;
		fDensity_3 = 0;
	}

} STR_DensityData ;


typedef struct ST_DensityDataOrg{
	unsigned int	uiCollectorID;
	unsigned char	aucTaskID[LENGTH_OF_TASK_ID];	
	unsigned int	uiFreq;
	short			fDensity;

	ST_DensityDataOrg()
	{
		memset(aucTaskID, 0, sizeof(aucTaskID));

		uiCollectorID = 0;
		uiFreq = 0;
		fDensity = 0;
	}

} STR_DensityDataOrg;

typedef struct ST_IQData{
	unsigned int	uiCollectorID;	
	unsigned int	uiReqIQInfo;  //1: ����, 2: ����

	ST_IQData()
	{
		uiCollectorID = 0;
		uiReqIQInfo = 0;
	}

} STR_IQData;
#pragma pack(pop)
#endif