#pragma once

#include "MsgQueueThread.h"
#include "CommIF\CommonMngr.h"
#include "CommIF\CGRCommIF.h"
#include "RadarAnlAlgorithm.h"
#include "ThreadTask/DFMsgDefnt.h"

#define RCV_DP_NUM    3
#define EQ_CHK_NUM    4

enum EQUIP_CHECK_TYPE 
{
	EQ_INT = 0,
	EQ_SW,
	EQ_PBT,
	EQ_IBT
}; 

enum TASK_USE_STAT 
{
	TK_NOUSE = 0,
	TK_USE
}; 

enum REQ_IQ 
{
	IQ_START = 1,
	IQ_STOP
}; 

struct STMsg;

class CRadarAnalysisMngr : public CMsgQueueThread, public CCommonMngr
{
private:
    unsigned int m_uiOpInitID;

public:
	CRadarAnalysisMngr();
	~CRadarAnalysisMngr(void);

	static CRadarAnalysisMngr* GetInstance();

	void Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData);
	void ProcessMsg(STMsg& i_stMsg);

	CGRCommIF& m_hCommIF_RAMngr;
	static CRadarAnalysisMngr* uniqueInstance;

	int m_OVIP;
	unsigned char GetLastIP();

	int m_iTaskCnt;
	int m_iNextTaskCnt;

	HANDLE m_hRqTaskTimer;
	HANDLE m_hRqTaskTimerQueue;

	HANDLE m_hEQCHKTimer[EQ_CHK_NUM];
	HANDLE m_hEQCHKTimerQueue[EQ_CHK_NUM];

	int	m_TaskAcqCnt;
	STxAcqTraskData m_stCurTaskData;
	std::list<STxAcqTraskData>::iterator iter;
	std::list<STxAcqTraskData> m_listTaskData; //과제정보 관리
	bool ProcessTaskMsg(void *i_pvData);
	void ReqPDWNextTask();
	void ReqCurTaskFromIQ();
	void StartTaskFlagCheckTimer();
	void StopTaskTimer();
	void TaskAsqReqCheck();	
	void ClearAsqDoneFlag();
	void ClearDensityFlag();
	void ClearEquipInitFlag(int i_nEqType);

    inline unsigned int GetOpInitID() { return m_uiOpInitID; }
	
	//bool m_bRevAsqDataFromDP[RCV_DP_NUM];
	bool m_bDensityFlag[RCV_DP_NUM];
	bool m_bAsqDoneFlag[RCV_DP_NUM];
	bool m_bEquipInit[RCV_DP_NUM];
	bool m_bEquipSW[RCV_DP_NUM];
	bool m_bEquipPBIT[RCV_DP_NUM];
	bool m_bEquipIBIT[RCV_DP_NUM];
	bool m_bAsqTotalFlag;
	bool m_bDensityTotalFlag;
	bool m_bTaskStopFlag;
	bool m_bAsqIQFlag;

	void TimeoutEquipCheckAckCnt(int i_nEqType);
	void StopEquipCheckAckTimer(int i_nEqType);
	void SendDensityDataFromTimeOut();

	SRxInitReqRslt m_stInitReqRslt_1;
	SRxInitReqRslt m_stInitReqRslt_2;
	SRxInitReqRslt m_stInitReqRslt_3;

	SRxInitReqRslt m_stSWVerRslt_1;
	SRxInitReqRslt m_stSWVerRslt_2;
	SRxInitReqRslt m_stSWVerRslt_3;

	SRxBITReqRslt m_stPBITRslt_1;
	SRxBITReqRslt m_stPBITRslt_2;
	SRxBITReqRslt m_stPBITRslt_3;

	STR_DensityDataOrg m_stDensityDataOrg_1;
	STR_DensityDataOrg m_stDensityDataOrg_2;
	STR_DensityDataOrg m_stDensityDataOrg_3;
	
	int	m_iEQChkInitAckCnt;
	int	m_iEQChkSWAckCnt;
	int	m_iEQChkPBITAckCnt;
	int	m_iEQChkIBITAckCnt;

	bool m_isTaskStartFlag;
	bool m_isTaskAsqStartFlag;

	int m_CHCALCurrent;
	int m_CHCALCurrentReslt;

	void SendDummyMsg();
};