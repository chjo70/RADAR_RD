#pragma once

#include "..\MsgQueueThread.h"
#include "..\CommIF\CommonMngr.h"
#include "..\CommIF\CGRCommIF.h"

#include "..\_lib_AnD\RcvFuncTCP.h"
#include "..\RadarDirAlgorithm.h"
//#include "..\DF_Incheon2019Dlg.h"
#include "..\ThreadTask/DFMsgDefnt.h"
#include <iostream>
#include <fstream>
#include <list>
#include "math.h"

#define COMINT_MAX_CHANNEL_COUNT	5			// COMIT 방탐 최대 채널 수
#define COLSTAT_INTERVAL	1000
#define COLSTAT_START_INTERVAL 1000
#define PDWCONNSTAT_INTERVAL 3000

enum MODE_TYPE
{
	MODE_INIT_TYPE = 1, //최초 보정
	MODE_CH_TYPE,       //채널보정
	MODE_CRT_TYPE,      //정확도 
};

struct STMsg;

class CDFTaskMngr : public CMsgQueueThread, public CCommonMngr
{
public:
	CDFTaskMngr();
	~CDFTaskMngr(void);

	static CDFTaskMngr* GetInstance();	

	void Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData);
	void ProcessMsg(STMsg& i_stMsg);

	void SendDummyMsg();
	unsigned char GetLastIP();
	bool SetDataChanelCorrect(UINT iFreq, float* fch_t);

	static CDFTaskMngr* m_pInstance;
	CGRCommIF& m_hCommIF_DFTaskMngr;

	CRcvFuncTCP g_RcvFunc;
	CRcvFuncTCP *g_RcvTempFunc;

	int m_LinkInfo;
	int m_POSNIP;
	CString m_strFilePath_Chennel;
	int m_iMode;
	UINT m_Freq;

	int m_pdwCntZero;
	bool m_bisRetrySigz;
	CString m_strAntFullFileName;
	int m_iTaskCnt;
	int m_iNextTaskCnt;
	int m_iTotCoretFreqDataCnt;
	int m_iIncrCoretFreqCnt;
	int m_RetryCnt;
	int m_RetryAginCnt;
	bool m_bIsTaskStop;
	int m_iAOAOffset;

	STxSystemVariable m_stSystemVal;

	std::list<STxAcqTraskData> m_listTaskData; //과제정보 관리
	std::list<STxAcqTraskData>::iterator iter;
	std::list<STAutoFreqDataLIst> m_listAutoFreqData; //채널보정 주파수 리스트 정보

	std::list<INT> m_listDFFreqData; //방탐정확도 측정을 위한 주파수 리스트 정보

	STxAcqTraskData m_stCurTaskData;
	HANDLE m_hRetryColectStatTimer;
	HANDLE m_hRetryColectTimerQueue;

	HANDLE m_hPDWConStatTimer;
	HANDLE m_hPDWConTimerQueue;

	HANDLE m_hRqTaskRetryTimer;
	HANDLE m_hRqTaskRetryTimerQueue;
		
	void ReqTaskRetry();
	void RetryCollectStatusReq();
	void CheckPDWConnStatus();
	void StopReqTaskRetryTimer();
	void StopRetryCollectStatusTimer();
	void StopPDWConnStatusTimer();
	void RetrySignalSend();
	bool ProcessTaskMsg(void *i_pvData);
	void ProcessNextTaskMsg(void *i_pvData, bool isTaskStop);
	void ReqPDWNextTask(bool isTaskStop);
	void StartCheckPDWConTimer();

	//안테나보정데이터 로드
	//BOOL LoadDfCalRomDataPh();
	void SetDFCrectFreqList(); //방탐정확도 주파수와 근접한 주파수 
	int GetIndexFreq(UINT iFreq); //방탐정확도를 위한 주파수 인덱스 값 얻기
	int GetAOADataFromAlgrism(UINT iFreq, int i_idxFreq, float *fchMeasPhDiffData);
	int GetCloseDFFreq(UINT Freq);
	bool isRqTaskAcqSigFromPDW;

	//채널보정
	BOOL m_bStartChannelCorrect;
	bool m_bEndChannelCorrect;
	void StartChannelCorrect(int i_nModeType);
	void InitFileFromStartChCorrect();
	void GetAutoFreqChCorrect();
	bool m_SetReqInitFlag;
	void SendReqInitRslt();


	//PDW전송
	void SendSptrCmdToPDW(int i_ModeType);
	void SetPDWAcqCollect(STxAcqTraskData stAcqTaskData, int i_ModeType);

	//레이더방탐장치 제어
	void SetFreqToRadarUnit(UINT Freq);

	//레이더분석 데몬에 전송
	void SendToRadarAnlys(int i_opCode, int i_cmd);

	void testTask();
	CString GetModulePath();
	void AlgrismInit();
};
