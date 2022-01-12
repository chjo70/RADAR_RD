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

#define COMINT_MAX_CHANNEL_COUNT	5			// COMIT ��Ž �ִ� ä�� ��
#define COLSTAT_INTERVAL	1000
#define COLSTAT_START_INTERVAL 1000
#define PDWCONNSTAT_INTERVAL 3000

enum MODE_TYPE
{
	MODE_INIT_TYPE = 1, //���� ����
	MODE_CH_TYPE,       //ä�κ���
	MODE_CRT_TYPE,      //��Ȯ�� 
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

	std::list<STxAcqTraskData> m_listTaskData; //�������� ����
	std::list<STxAcqTraskData>::iterator iter;
	std::list<STAutoFreqDataLIst> m_listAutoFreqData; //ä�κ��� ���ļ� ����Ʈ ����

	std::list<INT> m_listDFFreqData; //��Ž��Ȯ�� ������ ���� ���ļ� ����Ʈ ����

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

	//���׳����������� �ε�
	//BOOL LoadDfCalRomDataPh();
	void SetDFCrectFreqList(); //��Ž��Ȯ�� ���ļ��� ������ ���ļ� 
	int GetIndexFreq(UINT iFreq); //��Ž��Ȯ���� ���� ���ļ� �ε��� �� ���
	int GetAOADataFromAlgrism(UINT iFreq, int i_idxFreq, float *fchMeasPhDiffData);
	int GetCloseDFFreq(UINT Freq);
	bool isRqTaskAcqSigFromPDW;

	//ä�κ���
	BOOL m_bStartChannelCorrect;
	bool m_bEndChannelCorrect;
	void StartChannelCorrect(int i_nModeType);
	void InitFileFromStartChCorrect();
	void GetAutoFreqChCorrect();
	bool m_SetReqInitFlag;
	void SendReqInitRslt();


	//PDW����
	void SendSptrCmdToPDW(int i_ModeType);
	void SetPDWAcqCollect(STxAcqTraskData stAcqTaskData, int i_ModeType);

	//���̴���Ž��ġ ����
	void SetFreqToRadarUnit(UINT Freq);

	//���̴��м� ���� ����
	void SendToRadarAnlys(int i_opCode, int i_cmd);

	void testTask();
	CString GetModulePath();
	void AlgrismInit();
};
