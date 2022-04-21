#include "stdafx.h"
#include "RadarAnalysisMngr.h"
#include "ThreadTask/DFMsgDefnt.h"






// 운용-레이더분석 OPCODE
#define OPCODE_OV_BD_REQINIT		0x01020100		// 운용-레이더분석 초기화 요구
#define OPCODE_OV_BD_REQSWVER		0x01020200		// 운용-레이더분석 SW버전 요구
#define OPCODE_OV_BD_REQPBIT		0x01020300		// 운용-레이더분석 초기자체점검 요구
#define OPCODE_OV_BD_REQIBIT		0x01020400		// 운용-레이더분석 운용자자체점검 요구
#define OPCODE_OV_BD_REQCHCALIB		0x01020500		// 운용-레이더분석 채널 보정 요구
#define OPCODE_OV_BD_REQCHCALIBCHECK	0x01020600	// 운용-레이더분석 채널 보정 필요 상태 확인 요구

#define OPCODE_OV_BD_REQSTARTACQ	0x01021100		// 운용-레이더분석 수집시작 요구
#define OPCODE_OV_BD_SETRESET		0x01021200		// 운용-레이더분석 장비 리셋 요구
#define OPCODE_OV_BD_CONTROLRECVR	0x01021300		// 운용-레이더분석 수신기 제어
#define OPCODE_OV_BD_REQSYSTEMSET	0x01021400		// 운용-레이더분석 레이더방탐 시스템변수 설정 요구
#define OPCODE_OV_BD_REQCHANGEEMIT	0x01021600		// 운용-레이더분석 레이더/위협 변경 요구

#define OPCODE_OV_BD_REQACQTASK		0x01022100		// 운용-레이더분석 수집 과제 요구
#define OPCODE_OV_BD_REQREJFREQSET	0x01022200		// 운용-레이더분석 수신배제주파수 설정 요구
#define OPCODE_OV_BD_SETSYSTEM		0x01022300		// 운용-레이더분석 시스테 설정

#define OPCODE_OV_BD_REQIQDATA		0x01023500		// 운용-레이더분석 IQ수집 시작/중지
#define OPCODE_OV_BD_SENDERROR		0x0102F100		// 운용-레이더분석 에러 전송

// 레이더분석-운용 OPCODE
#define OPCODE_BD_OV_RSTINIT		0x02010100		// 레이더분석-운용 초기화 결과
#define OPCODE_BD_OV_RSTSWVER		0x02010200		// 레이더분석-운용 SW버전 결과
#define OPCODE_BD_OV_RSTPBIT		0x02010300		// 레이더분석-운용 초기자체점검 결과
#define OPCODE_BD_OV_RSTIBIT		0x02010400		// 레이더분석-운용 운용자자체점검 결과
#define OPCODE_BD_OV_STATECHCALIB	0x02010500		// 레이더분석-운용 채널 진행 상태
#define OPCODE_BD_OV_REQCHCALIBCHECK	0x02010600	// 레이더분석-운용 채널 보정 필요 상태 결과

#define OPCODE_BD_OV_RSTSTARTACQ	0x02011100		// 레이더분석-운용 수집시작 결과
#define OPCODE_BD_OV_RSTRESETEQUIP	0x02011200		// 레이더분석-운용 장비 리셋 결과
#define OPCODE_BD_OV_RSTRECVCONTROL	0x02011300		// 레이더분석-운용 수신기 제어 결과
#define OPCODE_BD_OV_RSTSYSTEMSET	0x02011400		// 레이더분석-운용 레이더방탐 시스템변수 설정 결과
#define OPCODE_BD_OV_RSTCHANGEEMIT	0x02011600		// 레이더분석-운용 레이더/위협 변경 결과
#define OPCODE_BD_OV_STATETASK		0x02011700		// 레이더분석-운용 과제 수행 상태

#define OPCODE_BD_OV_RSTACQTASK		0x02012100		// 레이더분석-운용 수집과제 수신결과
#define OPCODE_BD_OV_SENDLOB		0x02013100		// 레이더분석-운용 LOB 전송
#define OPCODE_BD_OV_SENDBEAM		0x02013300		// 레이더분석-운용 빔 전송
#define OPCODE_BD_OV_SENDDENSITY	0x02013600		// 레이더분석-운용 혼잡도 전송

#define OPCODE_BD_OV_SENDERROR		0x0201F100		// 레이더분석-운용 에러 전송

// 레이더분석-레이더방탐 OPCODE
#define OPCODE_BD_TF_REQINIT		0x02820100		// 레이더분석-레이더방탐 초기화 요구
#define OPCODE_BD_TF_REQSWVER		0x02820200		// 레이더분석-레이더방탐 SW버전 요구
#define OPCODE_BD_TF_REQPBIT		0x02820300		// 레이더분석-레이더방탐 초기자체점검 요구
#define OPCODE_BD_TF_REQIBIT		0x02820400		// 레이더분석-레이더방탐 운용자자체점검 요구
#define OPCODE_BD_TF_REQCHCALIB		0x02820500		// 레이더분석-레이더방탐 채널 보정 요구
#define OPCODE_BD_TF_REQCHCALIBCHECK	0x02820600		// 레이더분석-레이더방탐 채널 보정 필요 상태 확인 요구

#define OPCODE_BD_TF_REQSTARTACQ	0x02821100		// 레이더분석-레이더방탐 수집시작 요구
#define OPCODE_BD_TF_SETRESET		0x02821200		// 레이더분석-레이더방탐 장비 리셋 요구
#define OPCODE_BD_TF_CONTROLRECVR	0x02821300		// 레이더분석-레이더방탐 수신기 제어
#define OPCODE_BD_TF_REQSYSTEMSET	0x02821400		// 레이더분석-레이더방탐 레이더방탐 시스템변수 설정 요구

#define OPCODE_BD_TF_REQACQTASK		0x02822100		// 레이더분석-레이더방탐 수집 과제 요구
#define OPCODE_BD_TF_REQREJFREQSET	0x02822200		// 레이더분석-레이더방탐 수신배제주파수 설정 요구
#define OPCODE_BD_TF_SETSYSTEM		0x02822300		// 레이더분석-레이더방탐 시스테 설정
#define OPCODE_BD_TF_REQNEXTTASK	0x02822400		// 레이더분석-레이더방탐 다음 수집 과제 요구
#define OPCODE_BD_TF_REQIQDATA		0x02823500		// 레이더분석-레이더방탐 IQ 시작/중지 요구

#define OPCODE_BD_TF_SENDERROR		0x0282F100		// 레이더분석-레이더방탐 에러 전송

// 레이더방탐-레이더분석 OPCODE
#define OPCODE_TF_BD_RSTINIT		0x82020100		// 레이더방탐-레이더분석 초기화 결과
#define OPCODE_TF_BD_RSTSWVER		0x82020200		// 레이더방탐-레이더분석 SW버전 결과
#define OPCODE_TF_BD_RSTPBIT		0x82020300		// 레이더방탐-레이더분석 초기자체점검 결과
#define OPCODE_TF_BD_RSTIBIT		0x82020400		// 레이더방탐-레이더분석 운용자자체점검 결과
#define OPCODE_TF_BD_STATECHCALIB	0x82020500		// 레이더방탐-레이더분석 채널 진행 상태
#define OPCODE_TF_BD_REQCHCALIBCHECK	0x82020600		// 레이더방탐-레이더분석 채널 보정 필요 상태 결과

#define OPCODE_TF_BD_RSTSTARTACQ	0x82021100		// 레이더방탐-레이더분석 수집시작 결과
#define OPCODE_TF_BD_RSTRESETEQUIP	0x82021200		// 레이더방탐-레이더분석 장비 리셋 결과
#define OPCODE_TF_BD_RSTRECVCONTROL	0x82021300		// 레이더방탐-레이더분석 수신기 제어 결과
#define OPCODE_TF_BD_RSTSYSTEMSET	0x82021400		// 레이더방탐-레이더분석 레이더방탐 시스템변수 설정 결과
#define OPCODE_TF_BD_STATETASK		0x82021700		// 레이더방탐-레이더분석 과제 수행 상태
#define OPCODE_TF_BD_RSTACQTASK		0x82022100		// 레이더방탐-레이더분석 수집과제 수신결과

#define OPCODE_TF_BD_SENDLOB		0x82023100		// 레이더방탐-레이더분석 LOB 전송
#define OPCODE_TF_BD_SENDPDW		0x82023200		// 레이더방탐-레이더분석 PDW 전송
#define OPCODE_TF_BD_SENDDESITY		0x82023600		// 레이더방탐-레이더분석 혼잡도 전송

#define OPCODE_TF_BD_SENDERROR		0x8202F100		// 레이더방탐-레이더분석 에러 전송

#define TASK_RQADDCNT		5
#define SEND_DP_1			0
#define SEND_DP_2			1
#define SEND_DP_3			2
#define RCV_TIME_PERID		1000

#define EQ_CHK_TIMEOUT		20
#define EQ_CHK_PERID		 1

CRadarAnalysisMngr* CRadarAnalysisMngr::uniqueInstance=nullptr;

void CALLBACK ReqTaskTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired);

void CALLBACK ReqInitEquipStatusProc(PVOID lpParam, BOOLEAN TimerOrWaitFired);
void CALLBACK ReqSWVersionProc(PVOID lpParam, BOOLEAN TimerOrWaitFired);
void CALLBACK ReqPBITProc(PVOID lpParam, BOOLEAN TimerOrWaitFired);
void CALLBACK ReqIBITProc(PVOID lpParam, BOOLEAN TimerOrWaitFired);
void (CALLBACK *FUNCEQUIPCHECKTIMER[EQ_CHK_NUM])(PVOID,BOOLEAN) = {ReqInitEquipStatusProc, ReqSWVersionProc, ReqPBITProc, ReqIBITProc};


CRadarAnalysisMngr::CRadarAnalysisMngr()
:m_hCommIF_RAMngr(m_hCommIF)
{
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQINIT, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQSWVER, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQPBIT, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQIBIT, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQCHCALIB, this);		
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQCHCALIBCHECK, this);		
				
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQSTARTACQ, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_SETRESET, this);		
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_CONTROLRECVR, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQSYSTEMSET, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQCHANGEEMIT, this);

	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQACQTASK, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQREJFREQSET, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_SETSYSTEM, this);

	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQIQDATA, this);
	m_hCommIF.RegisterOpcode(OPCODE_OV_BD_SENDERROR, this);
			
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTINIT, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTSWVER, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTPBIT, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTIBIT, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_STATECHCALIB, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_REQCHCALIBCHECK, this);

	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTSTARTACQ, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTRESETEQUIP, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTRECVCONTROL, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTSYSTEMSET, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_STATETASK, this);

	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_SENDLOB, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_SENDPDW, this);
	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_SENDDESITY, this);

	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_SENDERROR, this);

	m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTACQTASK, this);

	m_iTaskCnt = 0;
	m_iNextTaskCnt = 0;
	m_TaskAcqCnt = 0;
	m_hRqTaskTimerQueue = NULL;
	m_hRqTaskTimer = NULL;	
	m_bTaskStopFlag = false;
	m_bAsqTotalFlag = false;
	m_bDensityTotalFlag = false;

	m_bAsqIQFlag = false;

	m_iEQChkInitAckCnt = 0;
	m_iEQChkSWAckCnt = 0;
	m_iEQChkPBITAckCnt = 0;
	m_iEQChkIBITAckCnt = 0;

	for(int i=0; i < RCV_DP_NUM; i++)
	{
		//m_bRevAsqDataFromDP[i] = false;	
		m_bAsqDoneFlag[i] = false;

		m_bDensityFlag[i] = false;
		m_bEquipInit[i] = false;
		m_bEquipSW[i] = false;
		m_bEquipPBIT[i] = false;
		m_bEquipIBIT[i] = false;		
	}

	for(int i=0; i < EQ_CHK_NUM; i++)
	{
		m_hEQCHKTimerQueue[i] = NULL;
		m_hEQCHKTimer[i] = NULL;
	}

	m_OVIP = GetLastIP();
	m_isTaskStartFlag = false;
	m_isTaskAsqStartFlag = false;
	m_CHCALCurrent = 5;
	m_CHCALCurrentReslt = 5;
	// 시작시 한번만 호출하면 됩니다.

	RadarAnlAlgotirhm::RadarAnlAlgotirhm::Init( NULL, false, true );
    m_lOpInitID=RadarAnlAlgotirhm::RadarAnlAlgotirhm::GetOPInitID();

}

CRadarAnalysisMngr::~CRadarAnalysisMngr()
{
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQINIT, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQSWVER, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQPBIT, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQIBIT, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQCHCALIB, this);		
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQCHCALIBCHECK, this);	

	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQSTARTACQ, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_SETRESET, this);		
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_CONTROLRECVR, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQSYSTEMSET, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQCHANGEEMIT, this);

	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQACQTASK, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQREJFREQSET, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_SETSYSTEM, this);
	
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQIQDATA, this);
	m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_SENDERROR, this);

	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTINIT, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTSWVER, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTPBIT, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTIBIT, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_STATECHCALIB, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_REQCHCALIBCHECK, this);

	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTSTARTACQ, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTRESETEQUIP, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTRECVCONTROL, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTSYSTEMSET, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_STATETASK, this);

	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_SENDLOB, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_SENDPDW, this);
	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_SENDDESITY, this);

	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_SENDERROR, this);

	m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTACQTASK, this);

	// 프로그램 종료시 한번만 호출하면 됩니다.
	RadarAnlAlgotirhm::RadarAnlAlgotirhm::Close();
}

CRadarAnalysisMngr* CRadarAnalysisMngr::GetInstance()
{
	if (uniqueInstance == nullptr)
	{
		uniqueInstance = new CRadarAnalysisMngr();		
	}

	return uniqueInstance;
}

void CRadarAnalysisMngr::Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData)
{
	TRACE("[Log] Message is received.\n");

	// 수신단에서 Msg 데이터 형성
	STMsg stInstanceMsg;

	stInstanceMsg.uiOpcode = i_uiOpcode;
	stInstanceMsg.usMSize = i_usSize;
	stInstanceMsg.ucRevOperID = i_ucRevOperID;
	stInstanceMsg.ucLinkID = i_ucLink;
	stInstanceMsg.ucOperatorID = ucOpertorID;

	memcpy(stInstanceMsg.buf, i_pvData, i_usSize);

	// 쓰레드 처리를 위해 Msg Queue에 집어넣기
	PushMsg(stInstanceMsg); 

	m_cond.LIGSetEvent();
}

void CRadarAnalysisMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE별로 경우를 나열하여 처리하는 로직 구현 필요
	// 하단에 구현
	bool bRtnSend = false;

	STR_LOGMESSAGE stMsg;

	switch(i_stMsg.uiOpcode)
	{
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// 운용 To 레이더분석
	case OPCODE_OV_BD_REQINIT:		// 운용-레이더분석 초기화 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 초기화 요구\n");

			for(int i = 0; i< 1; i++)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQINIT, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더분석-레이더방탐#%d 초기화 요구\n", i);
				}
				else
				{
					TRACE("[송신 실패]레이더분석-레이더방탐#%d 초기화 요구\n", i);
				}
			}

			//if(m_hEQCHKTimerQueue[EQ_INT] == NULL)
			//{	//DTEC_NullPointCheck
			//	m_hEQCHKTimerQueue[EQ_INT] = CreateTimerQueue();
			//	CreateTimerQueueTimer(&m_hEQCHKTimer[EQ_INT], m_hEQCHKTimerQueue[EQ_INT], (WAITORTIMERCALLBACK)FUNCEQUIPCHECKTIMER[EQ_INT], this, 1000, 1000, WT_EXECUTEDEFAULT);				
			//}
			//else
			//{
			//	TRACE("EQUIP INIT STATUS CHECK 타이머가 이미 생성되어 있습니다.");
			//}
		}
		break;

	case OPCODE_OV_BD_REQSWVER:		// 운용-레이더분석 SW버전 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 SW버전 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSWVER, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐#%d SW버전 요구\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐#%d SW버전 요구\n", i_stMsg.ucLinkID);
			}
			
			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSWVER, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더분석-레이더방탐#%d SW버전 요구\n", i);
				}
				else
				{
					TRACE("[송신 실패]레이더분석-레이더방탐#%d SW버전 요구\n", i);
				}
			}*/

			//if(m_hEQCHKTimerQueue[EQ_SW] == NULL)
			//{	//DTEC_NullPointCheck
			//	m_hEQCHKTimerQueue[EQ_SW] = CreateTimerQueue();
			//	CreateTimerQueueTimer(&m_hEQCHKTimer[EQ_SW], m_hEQCHKTimerQueue[EQ_SW], (WAITORTIMERCALLBACK)FUNCEQUIPCHECKTIMER[EQ_SW], this, 1000, 1000, WT_EXECUTEDEFAULT);				
			//}
			//else
			//{
			//	TRACE("EQUIP SW VERSION CHECK 타이머가 이미 생성되어 있습니다.");
			//}
		}
		break;

	case OPCODE_OV_BD_REQPBIT:		// 운용-레이더분석 초기자체점검 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 초기자체점검 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQPBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐#%d 초기자체점검 요구\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐#%d 초기자체점검 요구\n", i_stMsg.ucLinkID);
			}

			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQPBIT, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[송신]레이더분석-레이더방탐#%d 초기자체점검 요구\n", i);
			}
			else
			{
			TRACE("[송신 실패]레이더분석-레이더방탐#%d 초기자체점검 요구\n", i);
			}
			}*/

			//if(m_hEQCHKTimerQueue[EQ_PBT] == NULL)
			//{	//DTEC_NullPointCheck
			//	m_hEQCHKTimerQueue[EQ_PBT] = CreateTimerQueue();
			//	CreateTimerQueueTimer(&m_hEQCHKTimer[EQ_PBT], m_hEQCHKTimerQueue[EQ_PBT], (WAITORTIMERCALLBACK)FUNCEQUIPCHECKTIMER[EQ_PBT], this, 1000, 1000, WT_EXECUTEDEFAULT);				
			//}
			//else
			//{
			//	TRACE("EQUIP PBIT CHECK 타이머가 이미 생성되어 있습니다.");
			//}
		}
		break;

	case OPCODE_OV_BD_REQIBIT:		// 운용-레이더분석 운용자자체점검 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 운용자자체점검 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQIBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐#%d 운용자자체점검 요구\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐#%d 운용자자체점검 요구\n", i_stMsg.ucLinkID);
			}

			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQIBIT, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[송신]레이더분석-레이더방탐#%d 운용자자체점검 요구\n", i);
			}
			else
			{
			TRACE("[송신 실패]레이더분석-레이더방탐#%d 운용자자체점검 요구\n", i);
			}
			}*/

			//if(m_hEQCHKTimerQueue[EQ_IBT] == NULL)
			//{	//DTEC_NullPointCheck
			//	m_hEQCHKTimerQueue[EQ_IBT] = CreateTimerQueue();
			//	CreateTimerQueueTimer(&m_hEQCHKTimer[EQ_IBT], m_hEQCHKTimerQueue[EQ_IBT], (WAITORTIMERCALLBACK)FUNCEQUIPCHECKTIMER[EQ_IBT], this, 1000, 1000, WT_EXECUTEDEFAULT);				
			//}
			//else
			//{
			//	TRACE("EQUIP 1BIT CHECK 타이머가 이미 생성되어 있습니다.");
			//}
		}
		break;
	
	case OPCODE_OV_BD_REQCHCALIB:		// 운용-레이더분석 채널 보정 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 채널 보정 요구\n");

			if(m_CHCALCurrent != i_stMsg.ucLinkID)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQCHCALIB, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	
			}

			m_CHCALCurrent = i_stMsg.ucLinkID;	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐#%d 채널 보정 요구\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐#%d 채널 보정 요구\n", i_stMsg.ucLinkID);
			}			
		}
		break;

	case OPCODE_OV_BD_REQCHCALIBCHECK:		// 운용-레이더분석 채널 보정 필요 상태 확인 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 채널 보정 필요 상태 확인 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQCHCALIBCHECK, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐#%d 채널 보정 필요 상태 확인 요구\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐#%d 채널 보정 필요 상태 확인 요구\n", i_stMsg.ucLinkID);
			}

			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQCHCALIBCHECK, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[송신]레이더분석-레이더방탐#%d 채널 보정 필요 상태 확인 요구\n", i);
			}
			else
			{
			TRACE("[송신 실패]레이더분석-레이더방탐#%d 채널 보정 필요 상태 확인 요구\n", i);
			}
			}*/
		}
		break;

	case OPCODE_OV_BD_REQSTARTACQ:	// 운용-레이더분석 수집시작 요구
		{
			STxAcqStartRequest stAcqStartReq;
			memcpy(&stAcqStartReq, i_stMsg.buf, i_stMsg.usMSize);
			STxAcqStartRslt acqStartRslt;
			//m_bIsTaskStop = false;
			if(stAcqStartReq.uiMode == ACQ_START)
			{
				m_bTaskStopFlag = false;
			}
			else if(stAcqStartReq.uiMode == ACQ_STOP)
			{
				m_bTaskStopFlag = true;
				StopTaskTimer();
			}

			TRACE("[수신]운용-레이더분석 수집시작 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSTARTACQ, i_stMsg.usMSize, SEND_DP_1, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐#1 수집시작 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐#1 수집시작 요구\n");
			}

			/*bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSTARTACQ, i_stMsg.usMSize, SEND_DP_2, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[송신]레이더분석-레이더방탐#2 수집시작 요구\n");
			}
			else
			{
			TRACE("[송신 실패]레이더분석-레이더방탐#2 수집시작 요구\n");
			}

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSTARTACQ, i_stMsg.usMSize, SEND_DP_3, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[송신]레이더분석-레이더방탐#3 수집시작 요구\n");
			}
			else
			{
			TRACE("[송신 실패]레이더분석-레이더방탐#3 수집시작 요구\n");
			}*/
		}
		break;

	case OPCODE_OV_BD_SETRESET:		// 운용-레이더분석 장비 리셋 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 장비 리셋 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETRESET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 장비 리셋 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 장비 리셋 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_CONTROLRECVR:		// 운용-레이더분석 수신기 제어
		{
			TRACE("[수신]레이더분석-레이더방탐 수신기 제어\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_CONTROLRECVR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 수신기 제어\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 수신기 제어\n");
			}
		}
		break;
	
	case OPCODE_OV_BD_REQSYSTEMSET:		// 운용-레이더분석 시스템변수 설정 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 시스템변수 설정 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSYSTEMSET, i_stMsg.usMSize, SEND_DP_1, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 시스템변수 설정 요구_수신국#1

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐#1 시스템변수 설정 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐#1 시스템변수 설정 요구\n");
			}

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSYSTEMSET, i_stMsg.usMSize, SEND_DP_2, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 시스템변수 설정 요구_수신국#2

			//if(bRtnSend == true)
			//{
			//	TRACE("[송신]레이더분석-레이더방탐#2 시스템변수 설정 요구\n");
			//}
			//else
			//{
			//	TRACE("[송신 실패]레이더분석-레이더방탐#2 시스템변수 설정 요구\n");
			//}

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSYSTEMSET, i_stMsg.usMSize, SEND_DP_3, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 시스템변수 설정 요구_수신국#3

			//if(bRtnSend == true)
			//{
			//	TRACE("[송신]레이더분석-레이더방탐#3 시스템변수 설정 요구\n");
			//}
			//else
			//{
			//	TRACE("[송신 실패]레이더분석-레이더방탐#3 시스템변수 설정 요구\n");
			//}
		}
		break;

	case OPCODE_OV_BD_REQCHANGEEMIT:	// 운용-레이더분석 레이더/위협 변경 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 레이더/위협 변경 요구\n");

			// 레이더분석-레이더방탐 레이더/위협 변경 요구 관련 ICD 메시지가 없음
		}
		break;

	case OPCODE_OV_BD_REQACQTASK:	// 운용-레이더분석 수집 과제 요구
		{		
			TRACE("[수신]운용-레이더분석 수집 과제 요구\n");

			m_isTaskStartFlag = false;
			m_isTaskAsqStartFlag = false;
			m_listTaskData.clear();
			m_iNextTaskCnt = 0;
			//수집과제 수신 결과 전송 (방탐->분석)
			ProcessTaskMsg(i_stMsg.buf);

			RadarAnlAlgotirhm::RadarAnlAlgotirhm::SWInit();

			iter = m_listTaskData.begin();
			m_stCurTaskData = m_listTaskData.front();

			if(m_listTaskData.size() > 1)
			{
				iter++;
				m_iNextTaskCnt++;
			}		
			STxAcqTraskRequest AcqtaskReq; 
			AcqtaskReq.uiNumOfTask = 1;
			memcpy(AcqtaskReq.stTaskData, &m_stCurTaskData, sizeof(m_stCurTaskData));
		
			STMsg stMsg;
			memcpy(stMsg.buf, &AcqtaskReq, sizeof(AcqtaskReq));
			//if(m_stCurTaskData.uiRvcUseStat1 == TK_USE)
			//{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQACQTASK, sizeof(STxAcqTraskRequest), SEND_DP_1, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, stMsg.buf);	// 수신국#1

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더분석-레이더방탐#1 수집 과제 요구\n");
				}
				else
				{
					TRACE("[송신 실패]레이더분석-레이더방탐#1 수집 과제 요구\n");
				}
			//}

			//if(m_stCurTaskData.uiRvcUseStat2 == TK_USE)
			//{
				//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQACQTASK, sizeof(STxAcqTraskRequest), SEND_DP_2, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, stMsg.buf);	// 수신국#2

				//if(bRtnSend == true)
				//{
				//	TRACE("[송신]레이더분석-레이더방탐#2 수집 과제 요구\n");
				//}
				//else
				//{
				//	TRACE("[송신 실패]레이더분석-레이더방탐#2 수집 과제 요구\n");
				//}
			//}

			//if(m_stCurTaskData.uiRvcUseStat3 == TK_USE)
			//{
				//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQACQTASK, sizeof(STxAcqTraskRequest), SEND_DP_3, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, stMsg.buf);	// 수신국#3

				//if(bRtnSend == true)
				//{
				//	TRACE("[송신]레이더분석-레이더방탐#3 수집 과제 요구\n");
				//}
				//else
				//{
				//	TRACE("[송신 실패]레이더분석-레이더방탐#3 수집 과제 요구\n");
				//}
			//}
			//timer 
			ClearAsqDoneFlag();
			StartTaskFlagCheckTimer();
			
		}
		break;

	case OPCODE_OV_BD_REQREJFREQSET:	// 운용-레이더분석 수신배제주파수 설정 요구
		{
			TRACE("[수신]운용-레이더분석 수신배제주파수 설정 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQREJFREQSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 수신배제주파수 설정 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 수신배제주파수 설정 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_SETSYSTEM:	// 운용-레이더분석 시스템 설정
		{
			TRACE("[수신]운용-레이더분석 시스템 설정\n");

			STxSystemVariable stSystemVal;
			memcpy(&stSystemVal, i_stMsg.buf, i_stMsg.usMSize);

			stSystemVal.uiCWDecisionPW = 245000;
			stSystemVal.uiCWChoppinginterval = 245000;

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETSYSTEM, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETSYSTEM, sizeof(stSystemVal), i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, (void*)&stSystemVal); 


			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐#%d 시스템 설정\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐#%d 시스템 설정\n", i_stMsg.ucLinkID);
			}


			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETSYSTEM, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[송신]레이더분석-레이더방탐#%d 시스템 설정\n", i);
			}
			else
			{
			TRACE("[송신 실패]레이더분석-레이더방탐#%d 시스템 설정\n", i);
			}
			}*/
		}
		break;

	case OPCODE_OV_BD_REQIQDATA:
		{
			TRACE("[수신]운용-레이더분석 IQ데이터 시작/중지 전송\n");
			STR_IQData stReqIQData;

			memcpy(&stReqIQData, i_stMsg.buf, i_stMsg.usMSize);

			if(stReqIQData.uiReqIQInfo == IQ_START) //시작
			{
				m_bAsqIQFlag = true;
			}
			else if (stReqIQData.uiReqIQInfo == IQ_STOP) //종료
			{
				m_bAsqIQFlag = false;
			}

			for(int i = 0; i< 1; i++)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQIQDATA, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더분석-레이더방탐 IQ데이터#%d 시작/중지 전송\n", i);
				}
				else
				{
					TRACE("[송신 실패]레이더분석-레이더방탐 IQ데이터#%d 시작/중지 전송\n", i);
				}
			}
		}
		break;

	case OPCODE_OV_BD_SENDERROR:	// 운용-레이더분석 에러 전송
		{
			TRACE("[수신]운용-레이더분석 에러 전송\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SENDERROR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 에러 전송\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 에러 전송\n");
			}
		}
		break;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// 레이더방탐 To 레이더분석
	case OPCODE_TF_BD_RSTINIT:	// 레이더방탐-레이더분석 초기화 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 초기화 결과\n");

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //수신국#1
			{			
				memcpy(&m_stInitReqRslt_1, i_stMsg.buf, i_stMsg.usMSize);
				//m_bRevAsqDataFromDP[SEND_DP_1] = true;
				TRACE("**************수신국#1에 초기화 결과 수신===================\n");
				m_bEquipInit[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //수신국#2
			//{
			//	memcpy(&m_stInitReqRslt_2, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_2] = true;
			//	TRACE("**************수신국#2에 초기화 결과 수신===================\n");
			//	m_bEquipInit[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //수신국#3
			//{
			//	memcpy(&m_stInitReqRslt_3, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_3] = true;
			//	TRACE("**************수신국#3에 초기화 결과 수신===================\n");
			//	m_bEquipInit[SEND_DP_3] = true;
			//}

			if(m_bEquipInit[SEND_DP_1] /*&& m_bEquipInit[SEND_DP_2] && m_bEquipInit[SEND_DP_3]*/)
			{
				TRACE("**************수신국#1 ~ #3에 초기화 결과 수신===================\n");	
				m_iEQChkInitAckCnt = 0;
				StopEquipCheckAckTimer(EQ_INT);
				ClearEquipInitFlag(EQ_INT);

				////초기화 결과 전송 분석=>운용
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_1);	

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더분석-운용 초기화#1 결과\n");
				}
				else
				{
					TRACE("[송신 실패]레이더분석-운용 초기화#1 결과\n");
				}

				/*bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_2);	

				if(bRtnSend == true)
				{
				TRACE("[송신]레이더분석-운용 초기화#2 결과\n");
				}
				else
				{
				TRACE("[송신 실패]레이더분석-운용 초기화#2 결과\n");
				}

				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_3);	

				if(bRtnSend == true)
				{
				TRACE("[송신]레이더분석-운용 초기화#3 결과\n");
				}
				else
				{
				TRACE("[송신 실패]레이더분석-운용 초기화#3 결과\n");
				}*/
			}	

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);				
		}
		break;

	case OPCODE_TF_BD_RSTSWVER:	// 레이더방탐-레이더분석 SW버전 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 SW버전 결과\n");

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //수신국#1
			{			
				memcpy(&m_stSWVerRslt_1, i_stMsg.buf, i_stMsg.usMSize);
				//m_bRevAsqDataFromDP[SEND_DP_1] = true;
				TRACE("**************수신국#1에 SW버전 결과 수신===================\n");
				m_bEquipSW[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //수신국#2
			//{
			//	memcpy(&m_stSWVerRslt_2, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_2] = true;
			//	TRACE("**************수신국#2에 SW버전 결과 수신===================\n");
			//	m_bEquipSW[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //수신국#3
			//{
			//	memcpy(&m_stSWVerRslt_3, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_3] = true;
			//	TRACE("**************수신국#3에 SW버전 결과 수신===================\n");
			//	m_bEquipSW[SEND_DP_3] = true;
			//}

			if(m_bEquipSW[SEND_DP_1] /*&& m_bEquipSW[SEND_DP_2] && m_bEquipSW[SEND_DP_3]*/)
			{
				TRACE("**************수신국#1 ~ #3에 SW버전 결과 수신===================\n");	
				m_iEQChkSWAckCnt = 0;
				StopEquipCheckAckTimer(EQ_SW);
				ClearEquipInitFlag(EQ_SW);

				////초기화 결과 전송 분석=>운용
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_1);	

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더분석-운용 SW버전#1 결과\n");
				}
				else
				{
					TRACE("[송신 실패]레이더분석-운용 SW버전#1 결과\n");
				}

				/*bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_2);	

				if(bRtnSend == true)
				{
				TRACE("[송신]레이더분석-운용 SW버전#2 결과\n");
				}
				else
				{
				TRACE("[송신 실패]레이더분석-운용 SW버전#2 결과\n");
				}

				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_3);	

				if(bRtnSend == true)
				{
				TRACE("[송신]레이더분석-운용 SW버전#3 결과\n");
				}
				else
				{
				TRACE("[송신 실패]레이더분석-운용 SW버전#3 결과\n");
				}*/
			}	

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	

			/*if(bRtnSend == true)
			{
			TRACE("[송신]레이더분석-운용 SW버전 결과\n");
			}
			else
			{
			TRACE("[송신 실패]레이더분석-운용 SW버전 결과\n");
			}*/
		}
		break;

	case OPCODE_TF_BD_RSTPBIT:	// 레이더방탐-레이더분석 초기자체점검 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 초기자체점검 결과\n");

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //수신국#1
			{			
				memcpy(&m_stPBITRslt_1, i_stMsg.buf, i_stMsg.usMSize);
				//m_bRevAsqDataFromDP[SEND_DP_1] = true;
				TRACE("**************수신국#1에 초기자체점검 결과 수신===================\n");
				m_bEquipPBIT[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //수신국#2
			//{
			//	memcpy(&m_stPBITRslt_2, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_2] = true;
			//	TRACE("**************수신국#2에 초기자체점검 결과 수신===================\n");
			//	m_bEquipPBIT[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //수신국#3
			//{
			//	memcpy(&m_stPBITRslt_3, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_3] = true;
			//	TRACE("**************수신국#3에 초기자체점검 결과 수신===================\n");
			//	m_bEquipPBIT[SEND_DP_3] = true;
			//}

			if(m_bEquipPBIT[SEND_DP_1] /*&& m_bEquipPBIT[SEND_DP_2] && m_bEquipPBIT[SEND_DP_3]*/)
			{
				TRACE("**************수신국#1 ~ #3에 초기자체점검 결과 수신===================\n");	
				m_iEQChkPBITAckCnt = 0;
				StopEquipCheckAckTimer(EQ_PBT);
				ClearEquipInitFlag(EQ_PBT);

				////초기화 결과 전송 분석=>운용
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_1);	

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더분석-운용 초기자체점검#1 결과\n");
				}
				else
				{
					TRACE("[송신 실패]레이더분석-운용 초기자체점검#1 결과\n");
				}

				/*bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_2);	

				if(bRtnSend == true)
				{
				TRACE("[송신]레이더분석-운용 초기자체점검#2 결과\n");
				}
				else
				{
				TRACE("[송신 실패]레이더분석-운용 초기자체점검#2 결과\n");
				}

				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_3);	

				if(bRtnSend == true)
				{
				TRACE("[송신]레이더분석-운용 초기자체점검#3 결과\n");
				}
				else
				{
				TRACE("[송신 실패]레이더분석-운용 초기자체점검#3 결과\n");
				}*/
			}	

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	

			/*if(bRtnSend == true)
			{
			TRACE("[송신]레이더분석-운용 초기자체점검 결과\n");
			}
			else
			{
			TRACE("[송신 실패]레이더분석-운용 초기자체점검 결과\n");
			}*/
		}
		break;

	case OPCODE_TF_BD_RSTIBIT:	// 레이더방탐-레이더분석 운용자자체점검 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 운용자자체점검 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTIBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 운용자자체점검 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 운용자자체점검 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_STATECHCALIB:	// 레이더방탐-레이더분석 채널 보정 진행 상태
		{
			TRACE("[수신]레이더방탐-레이더분석 채널 보정 진행 상태\n");
			
			int ChannelStatus = 0;
			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_STATECHCALIB, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	

			memcpy(&ChannelStatus, i_stMsg.buf, i_stMsg.usMSize);

			if(ChannelStatus == 100)
			{
				m_CHCALCurrent = 5;
				m_CHCALCurrentReslt = 5;
			}
			
			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용%d 채널 보정 진행 상태\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용%d 채널 보정 진행 상태\n", i_stMsg.ucLinkID);
			}
		}
		break;

	case OPCODE_TF_BD_REQCHCALIBCHECK  :	// 레이더방탐-레이더분석 채널 보정 필요 상태 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 채널 보정 필요 상태 결과\n");

			int ChannelRslt = 0;			

			memcpy(&ChannelRslt, i_stMsg.buf, i_stMsg.usMSize);

			if(m_CHCALCurrentReslt != i_stMsg.ucLinkID)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_REQCHCALIBCHECK, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	
			}
			
			m_CHCALCurrentReslt = i_stMsg.ucLinkID;

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 채널 보정 필요 상태 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 채널 보정 필요 상태 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSTARTACQ:	// 레이더방탐-레이더분석 수집시작 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 수집시작 결과\n");

			if(m_isTaskAsqStartFlag == false)
			{
				m_isTaskAsqStartFlag = true;
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSTARTACQ, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	
			}

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 수집시작 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 수집시작 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTRESETEQUIP:	// 레이더방탐-레이더분석 장비 리셋 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 장비 리셋 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTRESETEQUIP, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 장비 리셋 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 장비 리셋 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTRECVCONTROL:	// 레이더방탐-레이더분석 수신기 제어 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 수신기 제어 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTRECVCONTROL, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 수신기 제어 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 수신기 제어 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSYSTEMSET:	// 레이더방탐-레이더분석 레이더방탐 시스템변수 설정 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 레이더방탐 시스템변수 설정 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSYSTEMSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 레이더방탐 시스템변수 설정 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 레이더방탐 시스템변수 설정 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_STATETASK:	// 레이더방탐-레이더분석 과제 수행 상태
		{
			TRACE("[수신]레이더방탐-레이더분석 과제 수행 상태\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_STATETASK, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 과제 수행 상태\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 과제 수행 상태\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTACQTASK:
		{
			TRACE("[수신]레이더방탐-레이더분석 수집과제 수신 결과\n");

		    if(m_isTaskStartFlag == false)
			{
				m_isTaskStartFlag = true;
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTACQTASK, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	
			}

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 수집과제 수신 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 수집과제 수신 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_SENDLOB:	// 레이더방탐-레이더분석 LOB 전송
		{
			TRACE("[수신]레이더방탐-레이더분석 LOB 전송\n");			

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //수신국#1
			{
				//m_bRevAsqDataFromDP[SEND_DP_1] = true;
				TRACE("**************수신국#1에 자료 LOB 수신===================\n");
				m_bAsqDoneFlag[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //수신국#2
			//{
			//	//m_bRevAsqDataFromDP[SEND_DP_2] = true;
			//	TRACE("**************수신국#2에 자료 LOB 수신===================\n");
			//	m_bAsqDoneFlag[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //수신국#3
			//{
			//	//m_bRevAsqDataFromDP[SEND_DP_3] = true;
			//	TRACE("**************수신국#3에 자료 LOB 수신===================\n");
			//	m_bAsqDoneFlag[SEND_DP_3] = true;
			//}

			if(m_bAsqDoneFlag[SEND_DP_1] /*&& m_bAsqDoneFlag[SEND_DP_2] && m_bAsqDoneFlag[SEND_DP_3]*/)
			{
				TRACE("**************수신국#1 ~ #3에 자료 LOB 수신===================\n");			
				m_bAsqTotalFlag = true;
			}		

			// 알고리즘 수행 후 BEAM 생성하여 BEAM 정보 전송하는 루틴 추가
			///////////////////////////////////////////////////////////////////////////////
			//조철희 수석님의 LOB데이타에서 빔 추출 알고리즘 호출			

			STR_LOBDATA stLOBData;

			stLOBData.stLOBHeader.iNumOfLOB = i_stMsg.usMSize / sizeof(SRxLOBData);
			memcpy(&stLOBData.stLOBData, i_stMsg.buf, i_stMsg.usMSize);

			//TRACE("**************[수신]레이더분석-레이더방탐 LOB 수신 개수%d============\n", stLOBData.stLOBHeader.iNumOfLOB);
			sprintf( stMsg.szContents, "레이더분석-레이더방탐 LOB 수신 개수 : %d", stLOBData.stLOBHeader.iNumOfLOB );
			::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enRECV, (LPARAM) & stMsg.szContents[0] );
			
			//STR_ABTDATA *pABTData, txABTData;
			RadarAnlAlgotirhm::RadarAnlAlgotirhm::Start(&stLOBData);
			//RadarAnlAlgotirhm::RadarAnlAlgotirhm::UpdateCEDEOBLibrary();

			STR_LOBDATA stResLOBData;
			STR_ABTDATA stResABTData;

			RadarAnlAlgotirhm::RadarAnlAlgotirhm::GetLOBData( & stResLOBData );			
			RadarAnlAlgotirhm::RadarAnlAlgotirhm::GetABTData( & stResABTData );

			//memcpy(&txABTData, pABTData, sizeof(txABTData));
			///////////////////////////////////////////////////////////////////////////////
			//LOB 데이터 전송
			for(int i=0; i< stResLOBData.stLOBHeader.iNumOfLOB; i++) {
				sprintf( stMsg.szContents, " 위협 관리 : LOB#[%d], ABT#[%d]", stLOBData.stLOBData[i].uiLOBID, stLOBData.stLOBData[i].uiABTID );
				::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enRECV, (LPARAM) & stMsg.szContents[0] );

				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDLOB, sizeof(SRxLOBData), i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, &stResLOBData.stLOBData[i]);	
            }

			if(bRtnSend == true)
			{
				//TRACE("[송신]레이더분석-운용 LOB 전송\n");
                sprintf( stMsg.szContents, "레이더분석-운용 LOB 전송[%d]", stLOBData.stLOBHeader.iNumOfLOB );
                ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND, (LPARAM) & stMsg.szContents[0] );
			}
			else
			{
				//TRACE("[송신 실패]레이더분석-운용 LOB 전송\n");
                sprintf( stMsg.szContents, "레이더분석-운용 LOB 전송" );
                ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND_FAIL, (LPARAM) & stMsg.szContents[0] );
			}

			TRACE("LOB 개수  ============================ %d\n", stResLOBData.stLOBHeader.iNumOfLOB);
			//빔 데이터 전송
			for(int i=0; i< stResABTData.stABTHeader.iNumOfABT; i++) {
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDBEAM, sizeof(SRxABTData), i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, &stResABTData.stABTData[i]);	
            }

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 빔 전송\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 빔 전송\n");
			}

			TRACE("빔 개수  ============================ %d\n", stResABTData.stABTHeader.iNumOfABT);

			//다음 과제 전송
			//방탐정보 수신국 3군데서 받은 후 다음 과제 전송
			if( true || m_bAsqTotalFlag == true || (m_bAsqDoneFlag[SEND_DP_1] /*&& m_bAsqDoneFlag[SEND_DP_2] && m_bAsqDoneFlag[SEND_DP_3]*/))
			{
				ClearAsqDoneFlag();
				TRACE("===*************88 과제 수집 완료로 인한 다음과제 요청***********==========\n");
				StopTaskTimer();
				m_TaskAcqCnt = 0;
				if(m_bAsqIQFlag == false)
				{
					ReqPDWNextTask();
				}
				else
				{
					ReqCurTaskFromIQ();
				}				
				m_bAsqTotalFlag = false;
			}			
		}
		break;

	case OPCODE_TF_BD_SENDPDW:	// 레이더방탐-레이더분석 PDW 전송
		{
			TRACE("[수신]레이더방탐-레이더분석 PDW 전송\n");

			// 레이더분석-운용 PDW 전송은 ICD 메시지 정의 안되어 있음
		}
		break;

	case OPCODE_TF_BD_SENDDESITY:	// 레이더방탐-레이더분석 혼잡도 전송
		{
			TRACE("[수신]레이더방탐-레이더분석 혼잡도 수신\n");

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //수신국#1
			{
				memcpy(&m_stDensityDataOrg_1, i_stMsg.buf, i_stMsg.usMSize);
				TRACE("**************수신국#1에 자료 혼잡도 수신===================\n");
				m_bDensityFlag[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //수신국#2
			//{
			//	memcpy(&m_stDensityDataOrg_2, i_stMsg.buf, i_stMsg.usMSize);
			//	TRACE("**************수신국#2에 자료 혼잡도 수신===================\n");
			//	m_bDensityFlag[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //수신국#3
			//{
			//	memcpy(&m_stDensityDataOrg_3, i_stMsg.buf, i_stMsg.usMSize);
			//	TRACE("**************수신국#3에 자료 혼잡도 수신===================\n");
			//	m_bDensityFlag[SEND_DP_3] = true;
			//}

			if(m_bDensityFlag[SEND_DP_1] /*&& m_bDensityFlag[SEND_DP_2] && m_bDensityFlag[SEND_DP_3]*/)
			{
				// 레이더분석-운용 혼잡도 전송은 ICD 메시지 정의 안되어 있음
				m_bDensityTotalFlag = true;
				ClearDensityFlag();
				STR_DensityData stDensityTotalData;
				memcpy(stDensityTotalData.aucTaskID, m_stCurTaskData.aucTaskID, sizeof(stDensityTotalData.aucTaskID));
				stDensityTotalData.uiFreq = m_stCurTaskData.uiFreq;
				stDensityTotalData.fDensity_1 = m_stDensityDataOrg_1.fDensity;
				stDensityTotalData.fDensity_2 = m_stDensityDataOrg_2.fDensity;
				stDensityTotalData.fDensity_3 = m_stDensityDataOrg_3.fDensity;
				TRACE("************** 혼잡도 freq : %d,  1 : %d, 2 : %d, 3 : %d===================\n", stDensityTotalData.uiFreq, stDensityTotalData.fDensity_1, stDensityTotalData.fDensity_2, stDensityTotalData.fDensity_3);
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDDENSITY, sizeof(STR_DensityData), SEND_DP_1, Equip_Rev_0V, m_OVIP, &stDensityTotalData);	
				//bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDDENSITY, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더분석-운용 혼잡도 전송\n");
				}
				else
				{
					TRACE("[송신 실패]레이더분석-운용 혼잡도 전송\n");
				}	
			}								
		}
		break;

	case OPCODE_TF_BD_SENDERROR:	// 레이더방탐-레이더분석 에러 전송
		{
			TRACE("[수신]레이더방탐-레이더분석 에러 전송\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDERROR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 에러 전송\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 에러 전송\n");
			}
		}
		break;

	default:
		break;
	}
}

unsigned char CRadarAnalysisMngr::GetLastIP()
{
	unsigned char ucLastIP = NULL;

	//////// 자신의 IP 획득 //////////////////////////////////////////////////////////////////
	char name[255] = {0,};
	memset(name, NULL, sizeof(name));

	PHOSTENT hostinfo = PHOSTENT();
	memset(&hostinfo, NULL, sizeof(PHOSTENT));

	CString strIP = _T("");
	if ( gethostname(name, sizeof(name)) == NULL )
	{
		hostinfo = gethostbyname(name);
		if ( hostinfo != NULL )
		{
			strIP = inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[0]);
		}
	}

	int iOffset = NULL;
	int iIp = NULL;
	iOffset = strIP.Find(".", 0);
	iOffset = strIP.Find(".", iOffset+1);
	iOffset = strIP.Find(".", iOffset+1);
	strIP = strIP.Mid(iOffset+1, strIP.GetLength()-iOffset-1);
	iIp	= _ttoi(strIP);

	if ( iIp > 0 && iIp < 65536 )
	{
		ucLastIP = (unsigned char)iIp;
	}	 
	//////////////////////////////////////////////////////////////////////////////////////////

	return ucLastIP;
}

bool CRadarAnalysisMngr::ProcessTaskMsg(void *i_pvData)
{
	//UINT nTaskNum = 0;
	bool bRtnSend = false;
	STxAcqTraskRequest AcqtaskReq; 
	memcpy(&AcqtaskReq, i_pvData, sizeof(AcqtaskReq));

	//nTaskNum = ENDIAN32(AcqtaskReq.uiNumOfTask); 

	m_iTaskCnt = AcqtaskReq.uiNumOfTask; 

	STxAcqTraskData *pTaskData;
	pTaskData = AcqtaskReq.stTaskData;

	STxAcqTraskData stTaskMngrData;

	for(int i = 0; i <m_iTaskCnt; ++i )
	{		
		memcpy(stTaskMngrData.aucTaskID, pTaskData->aucTaskID, sizeof(stTaskMngrData.aucTaskID)); 						
		stTaskMngrData.iRxThresholdValue1 = (pTaskData->iRxThresholdValue1 + 110) * 4;
		stTaskMngrData.iRxThresholdValue2 = (pTaskData->iRxThresholdValue2 + 110) * 4;
		stTaskMngrData.iRxThresholdValue3 = (pTaskData->iRxThresholdValue3 + 110) * 4;
		stTaskMngrData.iRadarEqGain1 = pTaskData->iRadarEqGain1;
		stTaskMngrData.iRadarEqGain2 = pTaskData->iRadarEqGain2;
		stTaskMngrData.iRadarEqGain3 = pTaskData->iRadarEqGain3;
		stTaskMngrData.uiRvcUseStat1 = pTaskData->uiRvcUseStat1;
		stTaskMngrData.uiRvcUseStat2 = pTaskData->uiRvcUseStat2;
		stTaskMngrData.uiRvcUseStat3 = pTaskData->uiRvcUseStat3;		

		stTaskMngrData.uiAcquisitionTime = pTaskData->uiAcquisitionTime;
		stTaskMngrData.uiNBDRBandWidth = pTaskData->uiNBDRBandWidth;
		stTaskMngrData.uiNumOfAcqPuls = pTaskData->uiNumOfAcqPuls + TASK_RQADDCNT;
		stTaskMngrData.uiFreq = pTaskData->uiFreq;
		//m_Freq = stTaskMngrData.uiFreq / 1000;
		//m_uiLastCurFreq = m_Freq;
		m_listTaskData.push_back(stTaskMngrData);				

		TRACE("\n ================= PDW 개수 : %d,  설정시간 : %d ====================\n", pTaskData->uiNumOfAcqPuls, pTaskData->uiAcquisitionTime); 
		++ pTaskData;
	}		

	return bRtnSend;
}

void CRadarAnalysisMngr::ReqPDWNextTask()
{
	if(m_bAsqIQFlag == false)
	{
		bool bRtnSend = false;
		m_bAsqTotalFlag = false;
		if(m_bTaskStopFlag == true)
			return;

		if(m_iTaskCnt ==  m_iNextTaskCnt)
		{
			iter = m_listTaskData.begin();		
			m_iNextTaskCnt = 0;
		}

		if(m_iTaskCnt != 1)
		{
			m_stCurTaskData = *(iter);		

			iter++;
			m_iNextTaskCnt ++;
		}

		TRACE("ReqPDWNextTask m_iNextTaskCnt = %d \n", m_iNextTaskCnt);
		//3군데 보내고 
		STxAcqTraskRequest AcqtaskReq; 
		AcqtaskReq.uiNumOfTask = 1;
		memcpy(AcqtaskReq.stTaskData, &m_stCurTaskData, sizeof(m_stCurTaskData));

		STMsg stMsg;
		memcpy(stMsg.buf, &AcqtaskReq, sizeof(AcqtaskReq));
		TRACE("\n ================= NEXT과제 ID  %s, Freq %d ====================\n", m_stCurTaskData.aucTaskID, m_stCurTaskData.uiFreq); 

		//if(m_stCurTaskData.uiRvcUseStat1 == TK_USE)
		//{
		bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_1, Equip_Rev_BD, m_OVIP, stMsg.buf);	// 수신국#1-Bypass

		if(bRtnSend == true)
		{
			TRACE("[송신]레이더분석-레이더방탐#1 수집 NEXT 과제 요구\n");
		}
		else
		{
			TRACE("[송신 실패]레이더분석-레이더방탐#1 수집 NEXT 과제 요구\n");
		}
		//}

		//if(m_stCurTaskData.uiRvcUseStat2 == TK_USE)
		//{
		//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_2, Equip_Rev_BD, m_OVIP, stMsg.buf);	// 수신국#2

		//if(bRtnSend == true)
		//{
		//	TRACE("[송신]레이더분석-레이더방탐#2 수집 NEXT 과제 요구\n");
		//}
		//else
		//{
		//	TRACE("[송신 실패]레이더분석-레이더방탐#2 수집 NEXT 과제 요구\n");
		//}
		//}

		//if(m_stCurTaskData.uiRvcUseStat3 == TK_USE)
		//{
		//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_3, Equip_Rev_BD, m_OVIP, stMsg.buf);	// 수신국#3

		//if(bRtnSend == true)
		//{
		//	TRACE("[송신]레이더분석-레이더방탐#3 수집 NEXT 과제 요구\n");
		//}
		//else
		//{
		//	TRACE("[송신 실패]레이더분석-레이더방탐#3 수집 NEXT 과제 요구\n");
		//}
		//}

		ClearAsqDoneFlag();
		StartTaskFlagCheckTimer();
	}	
}

void CRadarAnalysisMngr::ReqCurTaskFromIQ()
{
	bool bRtnSend = false;
	m_bAsqTotalFlag = false;
	if(m_bTaskStopFlag == true)
		return;

	//3군데 보내고 
	STxAcqTraskRequest AcqtaskReq; 
	AcqtaskReq.uiNumOfTask = 1;
	memcpy(AcqtaskReq.stTaskData, &m_stCurTaskData, sizeof(m_stCurTaskData));

	STMsg stMsg;
	memcpy(stMsg.buf, &AcqtaskReq, sizeof(AcqtaskReq));
	TRACE("\n ================= NEXT과제 ID  %s, Freq %d ====================\n", m_stCurTaskData.aucTaskID, m_stCurTaskData.uiFreq); 

	bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_1, Equip_Rev_BD, m_OVIP, stMsg.buf);	// 수신국#1-Bypass

	if(bRtnSend == true)
	{
		TRACE("[송신]레이더분석-레이더방탐#1 수집 NEXT 과제 요구\n");
	}
	else
	{
		TRACE("[송신 실패]레이더분석-레이더방탐#1 수집 NEXT 과제 요구\n");
	}
	
	//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_2, Equip_Rev_BD, m_OVIP, stMsg.buf);	// 수신국#2

	//if(bRtnSend == true)
	//{
	//	TRACE("[송신]레이더분석-레이더방탐#2 수집 NEXT 과제 요구\n");
	//}
	//else
	//{
	//	TRACE("[송신 실패]레이더분석-레이더방탐#2 수집 NEXT 과제 요구\n");
	//}
	
	//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_3, Equip_Rev_BD, m_OVIP, stMsg.buf);	// 수신국#3

	//if(bRtnSend == true)
	//{
	//	TRACE("[송신]레이더분석-레이더방탐#3 수집 NEXT 과제 요구\n");
	//}
	//else
	//{
	//	TRACE("[송신 실패]레이더분석-레이더방탐#3 수집 NEXT 과제 요구\n");
	//}

	ClearAsqDoneFlag();
	StartTaskFlagCheckTimer();
}

void CRadarAnalysisMngr::StartTaskFlagCheckTimer()
{
	if(m_hRqTaskTimerQueue == NULL)
	{							
		m_hRqTaskTimerQueue = CreateTimerQueue();
		CreateTimerQueueTimer(&m_hRqTaskTimer, m_hRqTaskTimerQueue, (WAITORTIMERCALLBACK)ReqTaskTimer, this, 0, 1000, 0);
	}	
}

void CRadarAnalysisMngr::StopTaskTimer()
{
	if ( m_hRqTaskTimerQueue == NULL )
		return;

	BOOL bRtn = DeleteTimerQueueTimer(m_hRqTaskTimerQueue, m_hRqTaskTimer, NULL);

	if(bRtn)
	{
		m_hRqTaskTimerQueue = NULL;
	}
	else
	{
		if ( ERROR_IO_PENDING == GetLastError() || m_hRqTaskTimerQueue == NULL || m_hRqTaskTimer == NULL )
		{
			m_hRqTaskTimerQueue = NULL;
		}
		else
		{
			while(DeleteTimerQueueTimer(m_hRqTaskTimerQueue, m_hRqTaskTimer, NULL) == 0)
			{
				if ( ERROR_IO_PENDING == GetLastError() || m_hRqTaskTimerQueue == NULL || m_hRqTaskTimer == NULL )
				{
					m_hRqTaskTimer = NULL;
					break;
				}
			}			
		}
	}	
}

void CALLBACK ReqTaskTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CRadarAnalysisMngr* pThis = ((CRadarAnalysisMngr*)lpParam);

	pThis->TaskAsqReqCheck();
}

void CRadarAnalysisMngr::TaskAsqReqCheck()
{
	m_TaskAcqCnt++;

	if((m_TaskAcqCnt * RCV_TIME_PERID >= m_stCurTaskData.uiAcquisitionTime + (RCV_TIME_PERID *3)))
	{
		/*m_bRevAsqDataFromDP[SEND_DP_1] = true;
		m_bRevAsqDataFromDP[SEND_DP_2] = true;
		m_bRevAsqDataFromDP[SEND_DP_3] = true;*/

		m_TaskAcqCnt = 0;
		TRACE("===과제 수집시간 종료로 인한 다음과제 요청 m_bAsqTotalFlag %d==========\n", m_bAsqTotalFlag);

		if(m_bAsqTotalFlag == false || (m_bAsqDoneFlag[SEND_DP_1] == false /*&& m_bAsqDoneFlag[SEND_DP_2] == false && m_bAsqDoneFlag[SEND_DP_3] == false*/) )
		{
			if(m_bDensityTotalFlag == false)
				SendDensityDataFromTimeOut();

			TRACE("======== 타이머에 의한 다음 과제 시작=========\n");
			if(m_bAsqIQFlag == false)
			{
				ReqPDWNextTask();
			}
			else
			{
				ReqCurTaskFromIQ();
			}

			m_bDensityTotalFlag = false;
		}
	}
}

void CRadarAnalysisMngr::ClearEquipInitFlag(int i_nEqType)
{
	switch(i_nEqType)
	{
	case EQ_INT:
		{
			for(int i=0; i < 1; i++)
			{
				m_bEquipInit[i] = false;		
			}	
		}
		break;
	case EQ_SW:
		{
			for(int i=0; i < 1; i++)
			{
				m_bEquipSW[i] = false;		
			}	
		}
		break;
	case EQ_PBT:
		{
			for(int i=0; i < 1; i++)
			{
				m_bEquipPBIT[i] = false;		
			}	
		}
		break;
	case EQ_IBT:
		{
			for(int i=0; i < 1; i++)
			{
				m_bEquipIBIT[i] = false;		
			}	
		}
		break;
	default:
		break;
	}

	
}

void CRadarAnalysisMngr::ClearAsqDoneFlag()
{
	m_TaskAcqCnt = 0;
	for(int i=0; i < 1; i++)
	{
		m_bAsqDoneFlag[i] = false;		
	}
}

void CRadarAnalysisMngr::ClearDensityFlag()
{
	for(int i=0; i < 1; i++)
	{
		m_bDensityFlag[i] = false;		
	}
}



void CALLBACK ReqInitEquipStatusProc(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CRadarAnalysisMngr* pCThis = ((CRadarAnalysisMngr*)lpParam);

	pCThis->TimeoutEquipCheckAckCnt(EQ_INT);
}

void CALLBACK ReqSWVersionProc(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CRadarAnalysisMngr* pCThis = ((CRadarAnalysisMngr*)lpParam);

	pCThis->TimeoutEquipCheckAckCnt(EQ_SW);
}

void CALLBACK ReqPBITProc(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CRadarAnalysisMngr* pCThis = ((CRadarAnalysisMngr*)lpParam);

	pCThis->TimeoutEquipCheckAckCnt(EQ_PBT);
}

void CALLBACK ReqIBITProc(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CRadarAnalysisMngr* pCThis = ((CRadarAnalysisMngr*)lpParam);

	pCThis->TimeoutEquipCheckAckCnt(EQ_IBT);
}

void CRadarAnalysisMngr::TimeoutEquipCheckAckCnt(int i_nEqType)
{
	switch(i_nEqType)
	{
	case EQ_INT:
		{
			m_iEQChkInitAckCnt++;

			if((m_iEQChkInitAckCnt * EQ_CHK_PERID >= EQ_CHK_TIMEOUT) )
			{
				m_iEQChkInitAckCnt = 0;			
				//타임아웃으로 인한 초기화 결과 전송
				StopEquipCheckAckTimer(EQ_INT);
				m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_1);	
				m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_2);	
				m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_3);	

				TRACE("======== 타이머에 의한 초기화 요구 전송 =========\n");
			}
		}
		break;
	case EQ_SW:
		{
			m_iEQChkSWAckCnt++;

			if((m_iEQChkSWAckCnt * EQ_CHK_PERID >= EQ_CHK_TIMEOUT) )
			{
				//타임아웃으로 인한 SW 버전 결과 전송
				StopEquipCheckAckTimer(EQ_SW);
				m_iEQChkSWAckCnt = 0;	

				m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_1);
				m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_2);	
				m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_3);		

				TRACE("======== 타이머에 의한 SW버전 요구 전송 =========\n");
			}
		}
		break;
	case EQ_PBT:
		{
			m_iEQChkPBITAckCnt++;

			if((m_iEQChkPBITAckCnt * EQ_CHK_PERID >= EQ_CHK_TIMEOUT) )
			{
				//타임아웃으로 인한 PBIT 결과 전송
				StopEquipCheckAckTimer(EQ_PBT);
				m_iEQChkPBITAckCnt = 0;	

				m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_1);
				m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_2);
				m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_3);

				TRACE("======== 타이머에 의한 초기자체점검 요구 전송 =========\n");
			}
		}
		break;
			
				
	case EQ_IBT:
		{
			m_iEQChkIBITAckCnt++;

			if((m_iEQChkIBITAckCnt * EQ_CHK_PERID >= EQ_CHK_TIMEOUT) )
			{
				//타임아웃으로 인한 IBIT 결과 전송
				StopEquipCheckAckTimer(EQ_IBT);
				m_iEQChkIBITAckCnt = 0;			

			}
		}
		break;
	default:
		break;
	}
}

void CRadarAnalysisMngr::StopEquipCheckAckTimer(int i_nEqType)
{
	switch(i_nEqType)
	{
	case EQ_INT:
		{
			if (m_hEQCHKTimerQueue[EQ_INT] == NULL )
				return;

			if(m_hEQCHKTimerQueue[EQ_INT] != NULL)
			{
				BOOL bRtn = DeleteTimerQueueTimer(m_hEQCHKTimerQueue[EQ_INT], m_hEQCHKTimer[EQ_INT], NULL);

				if(bRtn)
				{
					m_hEQCHKTimerQueue[EQ_INT] = NULL;
				}
				else
				{
					if(ERROR_IO_PENDING == GetLastError())
					{
						TRACE("EQ_INT_ACK PENDING\n");

						m_hEQCHKTimerQueue[EQ_INT] = NULL;
					}
					else
					{
						while(0 == DeleteTimerQueueTimer(m_hEQCHKTimerQueue[EQ_INT], m_hEQCHKTimer[EQ_INT], NULL))	
						{
							TRACE("EQ_INT_ACK DeleteTimerQueue Error %d\n", GetLastError());

							if(ERROR_IO_PENDING == GetLastError())
							{
								m_hEQCHKTimerQueue[EQ_INT] = NULL;	
								break;
							}
						}
					}
				}
			}
		}
		break;
	case EQ_SW:
		{
			if (m_hEQCHKTimerQueue[EQ_SW] == NULL )
				return;

			if(m_hEQCHKTimerQueue[EQ_SW] != NULL)
			{
				BOOL bRtn = DeleteTimerQueueTimer(m_hEQCHKTimerQueue[EQ_SW], m_hEQCHKTimer[EQ_SW], NULL);

				if(bRtn)
				{
					m_hEQCHKTimerQueue[EQ_SW] = NULL;
				}
				else
				{
					if(ERROR_IO_PENDING == GetLastError())
					{
						TRACE("EQ_SW_ACK PENDING\n");

						m_hEQCHKTimerQueue[EQ_SW] = NULL;
					}
					else
					{
						while(0 == DeleteTimerQueueTimer(m_hEQCHKTimerQueue[EQ_SW], m_hEQCHKTimer[EQ_SW], NULL))	
						{
							TRACE("EQ_SW_ACK DeleteTimerQueue Error %d\n", GetLastError());

							if(ERROR_IO_PENDING == GetLastError())
							{
								m_hEQCHKTimerQueue[EQ_SW] = NULL;	
								break;
							}
						}
					}
				}
			}
		}
		break;
	case EQ_PBT:
		{
			if (m_hEQCHKTimerQueue[EQ_PBT] == NULL )
				return;

			if(m_hEQCHKTimerQueue[EQ_PBT] != NULL)
			{
				BOOL bRtn = DeleteTimerQueueTimer(m_hEQCHKTimerQueue[EQ_PBT], m_hEQCHKTimer[EQ_PBT], NULL);

				if(bRtn)
				{
					m_hEQCHKTimerQueue[EQ_PBT] = NULL;
				}
				else
				{
					if(ERROR_IO_PENDING == GetLastError())
					{
						TRACE("EQ_PBT_ACK PENDING\n");

						m_hEQCHKTimerQueue[EQ_PBT] = NULL;
					}
					else
					{
						while(0 == DeleteTimerQueueTimer(m_hEQCHKTimerQueue[EQ_PBT], m_hEQCHKTimer[EQ_PBT], NULL))	
						{
							TRACE("EQ_PBT_ACK DeleteTimerQueue Error %d\n", GetLastError());

							if(ERROR_IO_PENDING == GetLastError())
							{
								m_hEQCHKTimerQueue[EQ_PBT] = NULL;	
								break;
							}
						}
					}
				}
			}
		}
		break;
	case EQ_IBT:
		{
			if (m_hEQCHKTimerQueue[EQ_IBT] == NULL )
				return;

			if(m_hEQCHKTimerQueue[EQ_IBT] != NULL)
			{
				BOOL bRtn = DeleteTimerQueueTimer(m_hEQCHKTimerQueue[EQ_IBT], m_hEQCHKTimer[EQ_IBT], NULL);

				if(bRtn)
				{
					m_hEQCHKTimerQueue[EQ_IBT] = NULL;
				}
				else
				{
					if(ERROR_IO_PENDING == GetLastError())
					{
						TRACE("EQ_IBT_ACK PENDING\n");

						m_hEQCHKTimerQueue[EQ_IBT] = NULL;
					}
					else
					{
						while(0 == DeleteTimerQueueTimer(m_hEQCHKTimerQueue[EQ_IBT], m_hEQCHKTimer[EQ_IBT], NULL))	
						{
							TRACE("EQ_IBT_ACK DeleteTimerQueue Error %d\n", GetLastError());

							if(ERROR_IO_PENDING == GetLastError())
							{
								m_hEQCHKTimerQueue[EQ_IBT] = NULL;	
								break;
							}
						}
					}
				}
			}
		}
		break;
	default:
		{	//DTEC_Else
		}
		break;
	}
}

void CRadarAnalysisMngr::SendDensityDataFromTimeOut()
{
	bool bRtnSend = false;
	STR_DensityData stDensityTotalData;
	memcpy(stDensityTotalData.aucTaskID, m_stCurTaskData.aucTaskID, sizeof(stDensityTotalData.aucTaskID));
	stDensityTotalData.uiFreq = m_stCurTaskData.uiFreq;

	if(m_bDensityFlag[SEND_DP_1] == true)
		stDensityTotalData.fDensity_1 = m_stDensityDataOrg_1.fDensity;
	else
	{
		if(m_bAsqDoneFlag[SEND_DP_1] == false)
			stDensityTotalData.fDensity_1 = 0;
	}

	/*if(m_bDensityFlag[SEND_DP_2] == true)
	stDensityTotalData.fDensity_2 = m_stDensityDataOrg_2.fDensity;
	else
	{
	if(m_bAsqDoneFlag[SEND_DP_2] == false)
	stDensityTotalData.fDensity_2 = 0;
	}

	if(m_bDensityFlag[SEND_DP_3] == true)
	stDensityTotalData.fDensity_3 = m_stDensityDataOrg_3.fDensity;
	else
	{
	if(m_bAsqDoneFlag[SEND_DP_3] == false)
	stDensityTotalData.fDensity_3 = 0;
	}*/

	bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDDENSITY, sizeof(STR_DensityData), SEND_DP_1, Equip_Rev_0V, m_OVIP, &stDensityTotalData);	

	TRACE("************** 타임아웃에 의한 혼잡도 freq : %d,  1 : %d, 2 : %d, 3 : %d===================\n", stDensityTotalData.uiFreq, stDensityTotalData.fDensity_1, stDensityTotalData.fDensity_2, stDensityTotalData.fDensity_3);

	if(bRtnSend == true)
	{
		TRACE("[송신]타임아웃에 의한 레이더분석-운용 혼잡도 전송\n");
	}
	else
	{
		TRACE("[송신 실패]타임아웃에 의한 레이더분석-운용 혼잡도 전송\n");
	}		

	ClearDensityFlag();
}

void CRadarAnalysisMngr::SendDummyMsg()
{
	int rslt = 1;
	
	m_hCommIF.Send(OPCODE_BD_OV_REQCHCALIBCHECK, sizeof(rslt), SEND_DP_1, Equip_Rev_0V, m_OVIP, (void*)&rslt);	
}