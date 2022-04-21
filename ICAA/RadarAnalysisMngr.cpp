#include "stdafx.h"
#include "RadarAnalysisMngr.h"
#include "ThreadTask/DFMsgDefnt.h"






// ���-���̴��м� OPCODE
#define OPCODE_OV_BD_REQINIT		0x01020100		// ���-���̴��м� �ʱ�ȭ �䱸
#define OPCODE_OV_BD_REQSWVER		0x01020200		// ���-���̴��м� SW���� �䱸
#define OPCODE_OV_BD_REQPBIT		0x01020300		// ���-���̴��м� �ʱ���ü���� �䱸
#define OPCODE_OV_BD_REQIBIT		0x01020400		// ���-���̴��м� �������ü���� �䱸
#define OPCODE_OV_BD_REQCHCALIB		0x01020500		// ���-���̴��м� ä�� ���� �䱸
#define OPCODE_OV_BD_REQCHCALIBCHECK	0x01020600	// ���-���̴��м� ä�� ���� �ʿ� ���� Ȯ�� �䱸

#define OPCODE_OV_BD_REQSTARTACQ	0x01021100		// ���-���̴��м� �������� �䱸
#define OPCODE_OV_BD_SETRESET		0x01021200		// ���-���̴��м� ��� ���� �䱸
#define OPCODE_OV_BD_CONTROLRECVR	0x01021300		// ���-���̴��м� ���ű� ����
#define OPCODE_OV_BD_REQSYSTEMSET	0x01021400		// ���-���̴��м� ���̴���Ž �ý��ۺ��� ���� �䱸
#define OPCODE_OV_BD_REQCHANGEEMIT	0x01021600		// ���-���̴��м� ���̴�/���� ���� �䱸

#define OPCODE_OV_BD_REQACQTASK		0x01022100		// ���-���̴��м� ���� ���� �䱸
#define OPCODE_OV_BD_REQREJFREQSET	0x01022200		// ���-���̴��м� ���Ź������ļ� ���� �䱸
#define OPCODE_OV_BD_SETSYSTEM		0x01022300		// ���-���̴��м� �ý��� ����

#define OPCODE_OV_BD_REQIQDATA		0x01023500		// ���-���̴��м� IQ���� ����/����
#define OPCODE_OV_BD_SENDERROR		0x0102F100		// ���-���̴��м� ���� ����

// ���̴��м�-��� OPCODE
#define OPCODE_BD_OV_RSTINIT		0x02010100		// ���̴��м�-��� �ʱ�ȭ ���
#define OPCODE_BD_OV_RSTSWVER		0x02010200		// ���̴��м�-��� SW���� ���
#define OPCODE_BD_OV_RSTPBIT		0x02010300		// ���̴��м�-��� �ʱ���ü���� ���
#define OPCODE_BD_OV_RSTIBIT		0x02010400		// ���̴��м�-��� �������ü���� ���
#define OPCODE_BD_OV_STATECHCALIB	0x02010500		// ���̴��м�-��� ä�� ���� ����
#define OPCODE_BD_OV_REQCHCALIBCHECK	0x02010600	// ���̴��м�-��� ä�� ���� �ʿ� ���� ���

#define OPCODE_BD_OV_RSTSTARTACQ	0x02011100		// ���̴��м�-��� �������� ���
#define OPCODE_BD_OV_RSTRESETEQUIP	0x02011200		// ���̴��м�-��� ��� ���� ���
#define OPCODE_BD_OV_RSTRECVCONTROL	0x02011300		// ���̴��м�-��� ���ű� ���� ���
#define OPCODE_BD_OV_RSTSYSTEMSET	0x02011400		// ���̴��м�-��� ���̴���Ž �ý��ۺ��� ���� ���
#define OPCODE_BD_OV_RSTCHANGEEMIT	0x02011600		// ���̴��м�-��� ���̴�/���� ���� ���
#define OPCODE_BD_OV_STATETASK		0x02011700		// ���̴��м�-��� ���� ���� ����

#define OPCODE_BD_OV_RSTACQTASK		0x02012100		// ���̴��м�-��� �������� ���Ű��
#define OPCODE_BD_OV_SENDLOB		0x02013100		// ���̴��м�-��� LOB ����
#define OPCODE_BD_OV_SENDBEAM		0x02013300		// ���̴��м�-��� �� ����
#define OPCODE_BD_OV_SENDDENSITY	0x02013600		// ���̴��м�-��� ȥ�⵵ ����

#define OPCODE_BD_OV_SENDERROR		0x0201F100		// ���̴��м�-��� ���� ����

// ���̴��м�-���̴���Ž OPCODE
#define OPCODE_BD_TF_REQINIT		0x02820100		// ���̴��м�-���̴���Ž �ʱ�ȭ �䱸
#define OPCODE_BD_TF_REQSWVER		0x02820200		// ���̴��м�-���̴���Ž SW���� �䱸
#define OPCODE_BD_TF_REQPBIT		0x02820300		// ���̴��м�-���̴���Ž �ʱ���ü���� �䱸
#define OPCODE_BD_TF_REQIBIT		0x02820400		// ���̴��м�-���̴���Ž �������ü���� �䱸
#define OPCODE_BD_TF_REQCHCALIB		0x02820500		// ���̴��м�-���̴���Ž ä�� ���� �䱸
#define OPCODE_BD_TF_REQCHCALIBCHECK	0x02820600		// ���̴��м�-���̴���Ž ä�� ���� �ʿ� ���� Ȯ�� �䱸

#define OPCODE_BD_TF_REQSTARTACQ	0x02821100		// ���̴��м�-���̴���Ž �������� �䱸
#define OPCODE_BD_TF_SETRESET		0x02821200		// ���̴��м�-���̴���Ž ��� ���� �䱸
#define OPCODE_BD_TF_CONTROLRECVR	0x02821300		// ���̴��м�-���̴���Ž ���ű� ����
#define OPCODE_BD_TF_REQSYSTEMSET	0x02821400		// ���̴��м�-���̴���Ž ���̴���Ž �ý��ۺ��� ���� �䱸

#define OPCODE_BD_TF_REQACQTASK		0x02822100		// ���̴��м�-���̴���Ž ���� ���� �䱸
#define OPCODE_BD_TF_REQREJFREQSET	0x02822200		// ���̴��м�-���̴���Ž ���Ź������ļ� ���� �䱸
#define OPCODE_BD_TF_SETSYSTEM		0x02822300		// ���̴��м�-���̴���Ž �ý��� ����
#define OPCODE_BD_TF_REQNEXTTASK	0x02822400		// ���̴��м�-���̴���Ž ���� ���� ���� �䱸
#define OPCODE_BD_TF_REQIQDATA		0x02823500		// ���̴��м�-���̴���Ž IQ ����/���� �䱸

#define OPCODE_BD_TF_SENDERROR		0x0282F100		// ���̴��м�-���̴���Ž ���� ����

// ���̴���Ž-���̴��м� OPCODE
#define OPCODE_TF_BD_RSTINIT		0x82020100		// ���̴���Ž-���̴��м� �ʱ�ȭ ���
#define OPCODE_TF_BD_RSTSWVER		0x82020200		// ���̴���Ž-���̴��м� SW���� ���
#define OPCODE_TF_BD_RSTPBIT		0x82020300		// ���̴���Ž-���̴��м� �ʱ���ü���� ���
#define OPCODE_TF_BD_RSTIBIT		0x82020400		// ���̴���Ž-���̴��м� �������ü���� ���
#define OPCODE_TF_BD_STATECHCALIB	0x82020500		// ���̴���Ž-���̴��м� ä�� ���� ����
#define OPCODE_TF_BD_REQCHCALIBCHECK	0x82020600		// ���̴���Ž-���̴��м� ä�� ���� �ʿ� ���� ���

#define OPCODE_TF_BD_RSTSTARTACQ	0x82021100		// ���̴���Ž-���̴��м� �������� ���
#define OPCODE_TF_BD_RSTRESETEQUIP	0x82021200		// ���̴���Ž-���̴��м� ��� ���� ���
#define OPCODE_TF_BD_RSTRECVCONTROL	0x82021300		// ���̴���Ž-���̴��м� ���ű� ���� ���
#define OPCODE_TF_BD_RSTSYSTEMSET	0x82021400		// ���̴���Ž-���̴��м� ���̴���Ž �ý��ۺ��� ���� ���
#define OPCODE_TF_BD_STATETASK		0x82021700		// ���̴���Ž-���̴��м� ���� ���� ����
#define OPCODE_TF_BD_RSTACQTASK		0x82022100		// ���̴���Ž-���̴��м� �������� ���Ű��

#define OPCODE_TF_BD_SENDLOB		0x82023100		// ���̴���Ž-���̴��м� LOB ����
#define OPCODE_TF_BD_SENDPDW		0x82023200		// ���̴���Ž-���̴��м� PDW ����
#define OPCODE_TF_BD_SENDDESITY		0x82023600		// ���̴���Ž-���̴��м� ȥ�⵵ ����

#define OPCODE_TF_BD_SENDERROR		0x8202F100		// ���̴���Ž-���̴��м� ���� ����

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
	// ���۽� �ѹ��� ȣ���ϸ� �˴ϴ�.

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

	// ���α׷� ����� �ѹ��� ȣ���ϸ� �˴ϴ�.
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

	// ���Ŵܿ��� Msg ������ ����
	STMsg stInstanceMsg;

	stInstanceMsg.uiOpcode = i_uiOpcode;
	stInstanceMsg.usMSize = i_usSize;
	stInstanceMsg.ucRevOperID = i_ucRevOperID;
	stInstanceMsg.ucLinkID = i_ucLink;
	stInstanceMsg.ucOperatorID = ucOpertorID;

	memcpy(stInstanceMsg.buf, i_pvData, i_usSize);

	// ������ ó���� ���� Msg Queue�� ����ֱ�
	PushMsg(stInstanceMsg); 

	m_cond.LIGSetEvent();
}

void CRadarAnalysisMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE���� ��츦 �����Ͽ� ó���ϴ� ���� ���� �ʿ�
	// �ϴܿ� ����
	bool bRtnSend = false;

	STR_LOGMESSAGE stMsg;

	switch(i_stMsg.uiOpcode)
	{
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// ��� To ���̴��м�
	case OPCODE_OV_BD_REQINIT:		// ���-���̴��м� �ʱ�ȭ �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž �ʱ�ȭ �䱸\n");

			for(int i = 0; i< 1; i++)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQINIT, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

				if(bRtnSend == true)
				{
					TRACE("[�۽�]���̴��м�-���̴���Ž#%d �ʱ�ȭ �䱸\n", i);
				}
				else
				{
					TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d �ʱ�ȭ �䱸\n", i);
				}
			}

			//if(m_hEQCHKTimerQueue[EQ_INT] == NULL)
			//{	//DTEC_NullPointCheck
			//	m_hEQCHKTimerQueue[EQ_INT] = CreateTimerQueue();
			//	CreateTimerQueueTimer(&m_hEQCHKTimer[EQ_INT], m_hEQCHKTimerQueue[EQ_INT], (WAITORTIMERCALLBACK)FUNCEQUIPCHECKTIMER[EQ_INT], this, 1000, 1000, WT_EXECUTEDEFAULT);				
			//}
			//else
			//{
			//	TRACE("EQUIP INIT STATUS CHECK Ÿ�̸Ӱ� �̹� �����Ǿ� �ֽ��ϴ�.");
			//}
		}
		break;

	case OPCODE_OV_BD_REQSWVER:		// ���-���̴��м� SW���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž SW���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSWVER, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž#%d SW���� �䱸\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d SW���� �䱸\n", i_stMsg.ucLinkID);
			}
			
			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSWVER, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

				if(bRtnSend == true)
				{
					TRACE("[�۽�]���̴��м�-���̴���Ž#%d SW���� �䱸\n", i);
				}
				else
				{
					TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d SW���� �䱸\n", i);
				}
			}*/

			//if(m_hEQCHKTimerQueue[EQ_SW] == NULL)
			//{	//DTEC_NullPointCheck
			//	m_hEQCHKTimerQueue[EQ_SW] = CreateTimerQueue();
			//	CreateTimerQueueTimer(&m_hEQCHKTimer[EQ_SW], m_hEQCHKTimerQueue[EQ_SW], (WAITORTIMERCALLBACK)FUNCEQUIPCHECKTIMER[EQ_SW], this, 1000, 1000, WT_EXECUTEDEFAULT);				
			//}
			//else
			//{
			//	TRACE("EQUIP SW VERSION CHECK Ÿ�̸Ӱ� �̹� �����Ǿ� �ֽ��ϴ�.");
			//}
		}
		break;

	case OPCODE_OV_BD_REQPBIT:		// ���-���̴��м� �ʱ���ü���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž �ʱ���ü���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQPBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž#%d �ʱ���ü���� �䱸\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d �ʱ���ü���� �䱸\n", i_stMsg.ucLinkID);
			}

			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQPBIT, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[�۽�]���̴��м�-���̴���Ž#%d �ʱ���ü���� �䱸\n", i);
			}
			else
			{
			TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d �ʱ���ü���� �䱸\n", i);
			}
			}*/

			//if(m_hEQCHKTimerQueue[EQ_PBT] == NULL)
			//{	//DTEC_NullPointCheck
			//	m_hEQCHKTimerQueue[EQ_PBT] = CreateTimerQueue();
			//	CreateTimerQueueTimer(&m_hEQCHKTimer[EQ_PBT], m_hEQCHKTimerQueue[EQ_PBT], (WAITORTIMERCALLBACK)FUNCEQUIPCHECKTIMER[EQ_PBT], this, 1000, 1000, WT_EXECUTEDEFAULT);				
			//}
			//else
			//{
			//	TRACE("EQUIP PBIT CHECK Ÿ�̸Ӱ� �̹� �����Ǿ� �ֽ��ϴ�.");
			//}
		}
		break;

	case OPCODE_OV_BD_REQIBIT:		// ���-���̴��м� �������ü���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž �������ü���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQIBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž#%d �������ü���� �䱸\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d �������ü���� �䱸\n", i_stMsg.ucLinkID);
			}

			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQIBIT, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[�۽�]���̴��м�-���̴���Ž#%d �������ü���� �䱸\n", i);
			}
			else
			{
			TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d �������ü���� �䱸\n", i);
			}
			}*/

			//if(m_hEQCHKTimerQueue[EQ_IBT] == NULL)
			//{	//DTEC_NullPointCheck
			//	m_hEQCHKTimerQueue[EQ_IBT] = CreateTimerQueue();
			//	CreateTimerQueueTimer(&m_hEQCHKTimer[EQ_IBT], m_hEQCHKTimerQueue[EQ_IBT], (WAITORTIMERCALLBACK)FUNCEQUIPCHECKTIMER[EQ_IBT], this, 1000, 1000, WT_EXECUTEDEFAULT);				
			//}
			//else
			//{
			//	TRACE("EQUIP 1BIT CHECK Ÿ�̸Ӱ� �̹� �����Ǿ� �ֽ��ϴ�.");
			//}
		}
		break;
	
	case OPCODE_OV_BD_REQCHCALIB:		// ���-���̴��м� ä�� ���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž ä�� ���� �䱸\n");

			if(m_CHCALCurrent != i_stMsg.ucLinkID)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQCHCALIB, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	
			}

			m_CHCALCurrent = i_stMsg.ucLinkID;	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž#%d ä�� ���� �䱸\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d ä�� ���� �䱸\n", i_stMsg.ucLinkID);
			}			
		}
		break;

	case OPCODE_OV_BD_REQCHCALIBCHECK:		// ���-���̴��м� ä�� ���� �ʿ� ���� Ȯ�� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž ä�� ���� �ʿ� ���� Ȯ�� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQCHCALIBCHECK, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž#%d ä�� ���� �ʿ� ���� Ȯ�� �䱸\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d ä�� ���� �ʿ� ���� Ȯ�� �䱸\n", i_stMsg.ucLinkID);
			}

			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQCHCALIBCHECK, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[�۽�]���̴��м�-���̴���Ž#%d ä�� ���� �ʿ� ���� Ȯ�� �䱸\n", i);
			}
			else
			{
			TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d ä�� ���� �ʿ� ���� Ȯ�� �䱸\n", i);
			}
			}*/
		}
		break;

	case OPCODE_OV_BD_REQSTARTACQ:	// ���-���̴��м� �������� �䱸
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

			TRACE("[����]���-���̴��м� �������� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSTARTACQ, i_stMsg.usMSize, SEND_DP_1, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž#1 �������� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž#1 �������� �䱸\n");
			}

			/*bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSTARTACQ, i_stMsg.usMSize, SEND_DP_2, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[�۽�]���̴��м�-���̴���Ž#2 �������� �䱸\n");
			}
			else
			{
			TRACE("[�۽� ����]���̴��м�-���̴���Ž#2 �������� �䱸\n");
			}

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSTARTACQ, i_stMsg.usMSize, SEND_DP_3, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[�۽�]���̴��м�-���̴���Ž#3 �������� �䱸\n");
			}
			else
			{
			TRACE("[�۽� ����]���̴��м�-���̴���Ž#3 �������� �䱸\n");
			}*/
		}
		break;

	case OPCODE_OV_BD_SETRESET:		// ���-���̴��м� ��� ���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž ��� ���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETRESET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ��� ���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ��� ���� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_CONTROLRECVR:		// ���-���̴��м� ���ű� ����
		{
			TRACE("[����]���̴��м�-���̴���Ž ���ű� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_CONTROLRECVR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ���ű� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ���ű� ����\n");
			}
		}
		break;
	
	case OPCODE_OV_BD_REQSYSTEMSET:		// ���-���̴��м� �ý��ۺ��� ���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž �ý��ۺ��� ���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSYSTEMSET, i_stMsg.usMSize, SEND_DP_1, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �ý��ۺ��� ���� �䱸_���ű�#1

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž#1 �ý��ۺ��� ���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž#1 �ý��ۺ��� ���� �䱸\n");
			}

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSYSTEMSET, i_stMsg.usMSize, SEND_DP_2, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �ý��ۺ��� ���� �䱸_���ű�#2

			//if(bRtnSend == true)
			//{
			//	TRACE("[�۽�]���̴��м�-���̴���Ž#2 �ý��ۺ��� ���� �䱸\n");
			//}
			//else
			//{
			//	TRACE("[�۽� ����]���̴��м�-���̴���Ž#2 �ý��ۺ��� ���� �䱸\n");
			//}

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSYSTEMSET, i_stMsg.usMSize, SEND_DP_3, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �ý��ۺ��� ���� �䱸_���ű�#3

			//if(bRtnSend == true)
			//{
			//	TRACE("[�۽�]���̴��м�-���̴���Ž#3 �ý��ۺ��� ���� �䱸\n");
			//}
			//else
			//{
			//	TRACE("[�۽� ����]���̴��м�-���̴���Ž#3 �ý��ۺ��� ���� �䱸\n");
			//}
		}
		break;

	case OPCODE_OV_BD_REQCHANGEEMIT:	// ���-���̴��м� ���̴�/���� ���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž ���̴�/���� ���� �䱸\n");

			// ���̴��м�-���̴���Ž ���̴�/���� ���� �䱸 ���� ICD �޽����� ����
		}
		break;

	case OPCODE_OV_BD_REQACQTASK:	// ���-���̴��м� ���� ���� �䱸
		{		
			TRACE("[����]���-���̴��м� ���� ���� �䱸\n");

			m_isTaskStartFlag = false;
			m_isTaskAsqStartFlag = false;
			m_listTaskData.clear();
			m_iNextTaskCnt = 0;
			//�������� ���� ��� ���� (��Ž->�м�)
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
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQACQTASK, sizeof(STxAcqTraskRequest), SEND_DP_1, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, stMsg.buf);	// ���ű�#1

				if(bRtnSend == true)
				{
					TRACE("[�۽�]���̴��м�-���̴���Ž#1 ���� ���� �䱸\n");
				}
				else
				{
					TRACE("[�۽� ����]���̴��м�-���̴���Ž#1 ���� ���� �䱸\n");
				}
			//}

			//if(m_stCurTaskData.uiRvcUseStat2 == TK_USE)
			//{
				//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQACQTASK, sizeof(STxAcqTraskRequest), SEND_DP_2, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, stMsg.buf);	// ���ű�#2

				//if(bRtnSend == true)
				//{
				//	TRACE("[�۽�]���̴��м�-���̴���Ž#2 ���� ���� �䱸\n");
				//}
				//else
				//{
				//	TRACE("[�۽� ����]���̴��м�-���̴���Ž#2 ���� ���� �䱸\n");
				//}
			//}

			//if(m_stCurTaskData.uiRvcUseStat3 == TK_USE)
			//{
				//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQACQTASK, sizeof(STxAcqTraskRequest), SEND_DP_3, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, stMsg.buf);	// ���ű�#3

				//if(bRtnSend == true)
				//{
				//	TRACE("[�۽�]���̴��м�-���̴���Ž#3 ���� ���� �䱸\n");
				//}
				//else
				//{
				//	TRACE("[�۽� ����]���̴��м�-���̴���Ž#3 ���� ���� �䱸\n");
				//}
			//}
			//timer 
			ClearAsqDoneFlag();
			StartTaskFlagCheckTimer();
			
		}
		break;

	case OPCODE_OV_BD_REQREJFREQSET:	// ���-���̴��м� ���Ź������ļ� ���� �䱸
		{
			TRACE("[����]���-���̴��м� ���Ź������ļ� ���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQREJFREQSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ���Ź������ļ� ���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ���Ź������ļ� ���� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_SETSYSTEM:	// ���-���̴��м� �ý��� ����
		{
			TRACE("[����]���-���̴��м� �ý��� ����\n");

			STxSystemVariable stSystemVal;
			memcpy(&stSystemVal, i_stMsg.buf, i_stMsg.usMSize);

			stSystemVal.uiCWDecisionPW = 245000;
			stSystemVal.uiCWChoppinginterval = 245000;

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETSYSTEM, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETSYSTEM, sizeof(stSystemVal), i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, (void*)&stSystemVal); 


			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž#%d �ý��� ����\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d �ý��� ����\n", i_stMsg.ucLinkID);
			}


			/*for(int i = 0; i< RCV_DP_NUM; i++)
			{
			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETSYSTEM, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
			TRACE("[�۽�]���̴��м�-���̴���Ž#%d �ý��� ����\n", i);
			}
			else
			{
			TRACE("[�۽� ����]���̴��м�-���̴���Ž#%d �ý��� ����\n", i);
			}
			}*/
		}
		break;

	case OPCODE_OV_BD_REQIQDATA:
		{
			TRACE("[����]���-���̴��м� IQ������ ����/���� ����\n");
			STR_IQData stReqIQData;

			memcpy(&stReqIQData, i_stMsg.buf, i_stMsg.usMSize);

			if(stReqIQData.uiReqIQInfo == IQ_START) //����
			{
				m_bAsqIQFlag = true;
			}
			else if (stReqIQData.uiReqIQInfo == IQ_STOP) //����
			{
				m_bAsqIQFlag = false;
			}

			for(int i = 0; i< 1; i++)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQIQDATA, i_stMsg.usMSize, i, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

				if(bRtnSend == true)
				{
					TRACE("[�۽�]���̴��м�-���̴���Ž IQ������#%d ����/���� ����\n", i);
				}
				else
				{
					TRACE("[�۽� ����]���̴��м�-���̴���Ž IQ������#%d ����/���� ����\n", i);
				}
			}
		}
		break;

	case OPCODE_OV_BD_SENDERROR:	// ���-���̴��м� ���� ����
		{
			TRACE("[����]���-���̴��м� ���� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SENDERROR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ���� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ���� ����\n");
			}
		}
		break;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// ���̴���Ž To ���̴��м�
	case OPCODE_TF_BD_RSTINIT:	// ���̴���Ž-���̴��м� �ʱ�ȭ ���
		{
			TRACE("[����]���̴���Ž-���̴��м� �ʱ�ȭ ���\n");

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //���ű�#1
			{			
				memcpy(&m_stInitReqRslt_1, i_stMsg.buf, i_stMsg.usMSize);
				//m_bRevAsqDataFromDP[SEND_DP_1] = true;
				TRACE("**************���ű�#1�� �ʱ�ȭ ��� ����===================\n");
				m_bEquipInit[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //���ű�#2
			//{
			//	memcpy(&m_stInitReqRslt_2, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_2] = true;
			//	TRACE("**************���ű�#2�� �ʱ�ȭ ��� ����===================\n");
			//	m_bEquipInit[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //���ű�#3
			//{
			//	memcpy(&m_stInitReqRslt_3, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_3] = true;
			//	TRACE("**************���ű�#3�� �ʱ�ȭ ��� ����===================\n");
			//	m_bEquipInit[SEND_DP_3] = true;
			//}

			if(m_bEquipInit[SEND_DP_1] /*&& m_bEquipInit[SEND_DP_2] && m_bEquipInit[SEND_DP_3]*/)
			{
				TRACE("**************���ű�#1 ~ #3�� �ʱ�ȭ ��� ����===================\n");	
				m_iEQChkInitAckCnt = 0;
				StopEquipCheckAckTimer(EQ_INT);
				ClearEquipInitFlag(EQ_INT);

				////�ʱ�ȭ ��� ���� �м�=>���
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_1);	

				if(bRtnSend == true)
				{
					TRACE("[�۽�]���̴��м�-��� �ʱ�ȭ#1 ���\n");
				}
				else
				{
					TRACE("[�۽� ����]���̴��м�-��� �ʱ�ȭ#1 ���\n");
				}

				/*bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_2);	

				if(bRtnSend == true)
				{
				TRACE("[�۽�]���̴��м�-��� �ʱ�ȭ#2 ���\n");
				}
				else
				{
				TRACE("[�۽� ����]���̴��м�-��� �ʱ�ȭ#2 ���\n");
				}

				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_3);	

				if(bRtnSend == true)
				{
				TRACE("[�۽�]���̴��м�-��� �ʱ�ȭ#3 ���\n");
				}
				else
				{
				TRACE("[�۽� ����]���̴��м�-��� �ʱ�ȭ#3 ���\n");
				}*/
			}	

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);				
		}
		break;

	case OPCODE_TF_BD_RSTSWVER:	// ���̴���Ž-���̴��м� SW���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� SW���� ���\n");

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //���ű�#1
			{			
				memcpy(&m_stSWVerRslt_1, i_stMsg.buf, i_stMsg.usMSize);
				//m_bRevAsqDataFromDP[SEND_DP_1] = true;
				TRACE("**************���ű�#1�� SW���� ��� ����===================\n");
				m_bEquipSW[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //���ű�#2
			//{
			//	memcpy(&m_stSWVerRslt_2, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_2] = true;
			//	TRACE("**************���ű�#2�� SW���� ��� ����===================\n");
			//	m_bEquipSW[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //���ű�#3
			//{
			//	memcpy(&m_stSWVerRslt_3, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_3] = true;
			//	TRACE("**************���ű�#3�� SW���� ��� ����===================\n");
			//	m_bEquipSW[SEND_DP_3] = true;
			//}

			if(m_bEquipSW[SEND_DP_1] /*&& m_bEquipSW[SEND_DP_2] && m_bEquipSW[SEND_DP_3]*/)
			{
				TRACE("**************���ű�#1 ~ #3�� SW���� ��� ����===================\n");	
				m_iEQChkSWAckCnt = 0;
				StopEquipCheckAckTimer(EQ_SW);
				ClearEquipInitFlag(EQ_SW);

				////�ʱ�ȭ ��� ���� �м�=>���
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_1);	

				if(bRtnSend == true)
				{
					TRACE("[�۽�]���̴��м�-��� SW����#1 ���\n");
				}
				else
				{
					TRACE("[�۽� ����]���̴��м�-��� SW����#1 ���\n");
				}

				/*bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_2);	

				if(bRtnSend == true)
				{
				TRACE("[�۽�]���̴��м�-��� SW����#2 ���\n");
				}
				else
				{
				TRACE("[�۽� ����]���̴��м�-��� SW����#2 ���\n");
				}

				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_3);	

				if(bRtnSend == true)
				{
				TRACE("[�۽�]���̴��м�-��� SW����#3 ���\n");
				}
				else
				{
				TRACE("[�۽� ����]���̴��м�-��� SW����#3 ���\n");
				}*/
			}	

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	

			/*if(bRtnSend == true)
			{
			TRACE("[�۽�]���̴��м�-��� SW���� ���\n");
			}
			else
			{
			TRACE("[�۽� ����]���̴��м�-��� SW���� ���\n");
			}*/
		}
		break;

	case OPCODE_TF_BD_RSTPBIT:	// ���̴���Ž-���̴��м� �ʱ���ü���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� �ʱ���ü���� ���\n");

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //���ű�#1
			{			
				memcpy(&m_stPBITRslt_1, i_stMsg.buf, i_stMsg.usMSize);
				//m_bRevAsqDataFromDP[SEND_DP_1] = true;
				TRACE("**************���ű�#1�� �ʱ���ü���� ��� ����===================\n");
				m_bEquipPBIT[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //���ű�#2
			//{
			//	memcpy(&m_stPBITRslt_2, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_2] = true;
			//	TRACE("**************���ű�#2�� �ʱ���ü���� ��� ����===================\n");
			//	m_bEquipPBIT[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //���ű�#3
			//{
			//	memcpy(&m_stPBITRslt_3, i_stMsg.buf, i_stMsg.usMSize);
			//	//m_bRevAsqDataFromDP[SEND_DP_3] = true;
			//	TRACE("**************���ű�#3�� �ʱ���ü���� ��� ����===================\n");
			//	m_bEquipPBIT[SEND_DP_3] = true;
			//}

			if(m_bEquipPBIT[SEND_DP_1] /*&& m_bEquipPBIT[SEND_DP_2] && m_bEquipPBIT[SEND_DP_3]*/)
			{
				TRACE("**************���ű�#1 ~ #3�� �ʱ���ü���� ��� ����===================\n");	
				m_iEQChkPBITAckCnt = 0;
				StopEquipCheckAckTimer(EQ_PBT);
				ClearEquipInitFlag(EQ_PBT);

				////�ʱ�ȭ ��� ���� �м�=>���
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_1);	

				if(bRtnSend == true)
				{
					TRACE("[�۽�]���̴��м�-��� �ʱ���ü����#1 ���\n");
				}
				else
				{
					TRACE("[�۽� ����]���̴��м�-��� �ʱ���ü����#1 ���\n");
				}

				/*bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_2);	

				if(bRtnSend == true)
				{
				TRACE("[�۽�]���̴��м�-��� �ʱ���ü����#2 ���\n");
				}
				else
				{
				TRACE("[�۽� ����]���̴��м�-��� �ʱ���ü����#2 ���\n");
				}

				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_3);	

				if(bRtnSend == true)
				{
				TRACE("[�۽�]���̴��м�-��� �ʱ���ü����#3 ���\n");
				}
				else
				{
				TRACE("[�۽� ����]���̴��м�-��� �ʱ���ü����#3 ���\n");
				}*/
			}	

			//bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	

			/*if(bRtnSend == true)
			{
			TRACE("[�۽�]���̴��м�-��� �ʱ���ü���� ���\n");
			}
			else
			{
			TRACE("[�۽� ����]���̴��м�-��� �ʱ���ü���� ���\n");
			}*/
		}
		break;

	case OPCODE_TF_BD_RSTIBIT:	// ���̴���Ž-���̴��м� �������ü���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� �������ü���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTIBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� �������ü���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� �������ü���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_STATECHCALIB:	// ���̴���Ž-���̴��м� ä�� ���� ���� ����
		{
			TRACE("[����]���̴���Ž-���̴��м� ä�� ���� ���� ����\n");
			
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
				TRACE("[�۽�]���̴��м�-���%d ä�� ���� ���� ����\n", i_stMsg.ucLinkID);
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���%d ä�� ���� ���� ����\n", i_stMsg.ucLinkID);
			}
		}
		break;

	case OPCODE_TF_BD_REQCHCALIBCHECK  :	// ���̴���Ž-���̴��м� ä�� ���� �ʿ� ���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� ä�� ���� �ʿ� ���� ���\n");

			int ChannelRslt = 0;			

			memcpy(&ChannelRslt, i_stMsg.buf, i_stMsg.usMSize);

			if(m_CHCALCurrentReslt != i_stMsg.ucLinkID)
			{
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_REQCHCALIBCHECK, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	
			}
			
			m_CHCALCurrentReslt = i_stMsg.ucLinkID;

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ä�� ���� �ʿ� ���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ä�� ���� �ʿ� ���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSTARTACQ:	// ���̴���Ž-���̴��м� �������� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� �������� ���\n");

			if(m_isTaskAsqStartFlag == false)
			{
				m_isTaskAsqStartFlag = true;
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSTARTACQ, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	
			}

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� �������� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� �������� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTRESETEQUIP:	// ���̴���Ž-���̴��м� ��� ���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� ��� ���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTRESETEQUIP, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ��� ���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ��� ���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTRECVCONTROL:	// ���̴���Ž-���̴��м� ���ű� ���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� ���ű� ���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTRECVCONTROL, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ���ű� ���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ���ű� ���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSYSTEMSET:	// ���̴���Ž-���̴��м� ���̴���Ž �ý��ۺ��� ���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� ���̴���Ž �ý��ۺ��� ���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSYSTEMSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ���̴���Ž �ý��ۺ��� ���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ���̴���Ž �ý��ۺ��� ���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_STATETASK:	// ���̴���Ž-���̴��м� ���� ���� ����
		{
			TRACE("[����]���̴���Ž-���̴��м� ���� ���� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_STATETASK, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ���� ���� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ���� ���� ����\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTACQTASK:
		{
			TRACE("[����]���̴���Ž-���̴��м� �������� ���� ���\n");

		    if(m_isTaskStartFlag == false)
			{
				m_isTaskStartFlag = true;
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTACQTASK, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	
			}

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� �������� ���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� �������� ���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_SENDLOB:	// ���̴���Ž-���̴��м� LOB ����
		{
			TRACE("[����]���̴���Ž-���̴��м� LOB ����\n");			

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //���ű�#1
			{
				//m_bRevAsqDataFromDP[SEND_DP_1] = true;
				TRACE("**************���ű�#1�� �ڷ� LOB ����===================\n");
				m_bAsqDoneFlag[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //���ű�#2
			//{
			//	//m_bRevAsqDataFromDP[SEND_DP_2] = true;
			//	TRACE("**************���ű�#2�� �ڷ� LOB ����===================\n");
			//	m_bAsqDoneFlag[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //���ű�#3
			//{
			//	//m_bRevAsqDataFromDP[SEND_DP_3] = true;
			//	TRACE("**************���ű�#3�� �ڷ� LOB ����===================\n");
			//	m_bAsqDoneFlag[SEND_DP_3] = true;
			//}

			if(m_bAsqDoneFlag[SEND_DP_1] /*&& m_bAsqDoneFlag[SEND_DP_2] && m_bAsqDoneFlag[SEND_DP_3]*/)
			{
				TRACE("**************���ű�#1 ~ #3�� �ڷ� LOB ����===================\n");			
				m_bAsqTotalFlag = true;
			}		

			// �˰��� ���� �� BEAM �����Ͽ� BEAM ���� �����ϴ� ��ƾ �߰�
			///////////////////////////////////////////////////////////////////////////////
			//��ö�� �������� LOB����Ÿ���� �� ���� �˰��� ȣ��			

			STR_LOBDATA stLOBData;

			stLOBData.stLOBHeader.iNumOfLOB = i_stMsg.usMSize / sizeof(SRxLOBData);
			memcpy(&stLOBData.stLOBData, i_stMsg.buf, i_stMsg.usMSize);

			//TRACE("**************[����]���̴��м�-���̴���Ž LOB ���� ����%d============\n", stLOBData.stLOBHeader.iNumOfLOB);
			sprintf( stMsg.szContents, "���̴��м�-���̴���Ž LOB ���� ���� : %d", stLOBData.stLOBHeader.iNumOfLOB );
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
			//LOB ������ ����
			for(int i=0; i< stResLOBData.stLOBHeader.iNumOfLOB; i++) {
				sprintf( stMsg.szContents, " ���� ���� : LOB#[%d], ABT#[%d]", stLOBData.stLOBData[i].uiLOBID, stLOBData.stLOBData[i].uiABTID );
				::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enRECV, (LPARAM) & stMsg.szContents[0] );

				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDLOB, sizeof(SRxLOBData), i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, &stResLOBData.stLOBData[i]);	
            }

			if(bRtnSend == true)
			{
				//TRACE("[�۽�]���̴��м�-��� LOB ����\n");
                sprintf( stMsg.szContents, "���̴��м�-��� LOB ����[%d]", stLOBData.stLOBHeader.iNumOfLOB );
                ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND, (LPARAM) & stMsg.szContents[0] );
			}
			else
			{
				//TRACE("[�۽� ����]���̴��м�-��� LOB ����\n");
                sprintf( stMsg.szContents, "���̴��м�-��� LOB ����" );
                ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND_FAIL, (LPARAM) & stMsg.szContents[0] );
			}

			TRACE("LOB ����  ============================ %d\n", stResLOBData.stLOBHeader.iNumOfLOB);
			//�� ������ ����
			for(int i=0; i< stResABTData.stABTHeader.iNumOfABT; i++) {
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDBEAM, sizeof(SRxABTData), i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, &stResABTData.stABTData[i]);	
            }

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� �� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� �� ����\n");
			}

			TRACE("�� ����  ============================ %d\n", stResABTData.stABTHeader.iNumOfABT);

			//���� ���� ����
			//��Ž���� ���ű� 3������ ���� �� ���� ���� ����
			if( true || m_bAsqTotalFlag == true || (m_bAsqDoneFlag[SEND_DP_1] /*&& m_bAsqDoneFlag[SEND_DP_2] && m_bAsqDoneFlag[SEND_DP_3]*/))
			{
				ClearAsqDoneFlag();
				TRACE("===*************88 ���� ���� �Ϸ�� ���� �������� ��û***********==========\n");
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

	case OPCODE_TF_BD_SENDPDW:	// ���̴���Ž-���̴��м� PDW ����
		{
			TRACE("[����]���̴���Ž-���̴��м� PDW ����\n");

			// ���̴��м�-��� PDW ������ ICD �޽��� ���� �ȵǾ� ����
		}
		break;

	case OPCODE_TF_BD_SENDDESITY:	// ���̴���Ž-���̴��м� ȥ�⵵ ����
		{
			TRACE("[����]���̴���Ž-���̴��м� ȥ�⵵ ����\n");

			if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP1) //���ű�#1
			{
				memcpy(&m_stDensityDataOrg_1, i_stMsg.buf, i_stMsg.usMSize);
				TRACE("**************���ű�#1�� �ڷ� ȥ�⵵ ����===================\n");
				m_bDensityFlag[SEND_DP_1] = true;
			}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP2) //���ű�#2
			//{
			//	memcpy(&m_stDensityDataOrg_2, i_stMsg.buf, i_stMsg.usMSize);
			//	TRACE("**************���ű�#2�� �ڷ� ȥ�⵵ ����===================\n");
			//	m_bDensityFlag[SEND_DP_2] = true;
			//}
			//if(i_stMsg.ucOperatorID == SYSTEM_CLR_EQUIP3) //���ű�#3
			//{
			//	memcpy(&m_stDensityDataOrg_3, i_stMsg.buf, i_stMsg.usMSize);
			//	TRACE("**************���ű�#3�� �ڷ� ȥ�⵵ ����===================\n");
			//	m_bDensityFlag[SEND_DP_3] = true;
			//}

			if(m_bDensityFlag[SEND_DP_1] /*&& m_bDensityFlag[SEND_DP_2] && m_bDensityFlag[SEND_DP_3]*/)
			{
				// ���̴��м�-��� ȥ�⵵ ������ ICD �޽��� ���� �ȵǾ� ����
				m_bDensityTotalFlag = true;
				ClearDensityFlag();
				STR_DensityData stDensityTotalData;
				memcpy(stDensityTotalData.aucTaskID, m_stCurTaskData.aucTaskID, sizeof(stDensityTotalData.aucTaskID));
				stDensityTotalData.uiFreq = m_stCurTaskData.uiFreq;
				stDensityTotalData.fDensity_1 = m_stDensityDataOrg_1.fDensity;
				stDensityTotalData.fDensity_2 = m_stDensityDataOrg_2.fDensity;
				stDensityTotalData.fDensity_3 = m_stDensityDataOrg_3.fDensity;
				TRACE("************** ȥ�⵵ freq : %d,  1 : %d, 2 : %d, 3 : %d===================\n", stDensityTotalData.uiFreq, stDensityTotalData.fDensity_1, stDensityTotalData.fDensity_2, stDensityTotalData.fDensity_3);
				bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDDENSITY, sizeof(STR_DensityData), SEND_DP_1, Equip_Rev_0V, m_OVIP, &stDensityTotalData);	
				//bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDDENSITY, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

				if(bRtnSend == true)
				{
					TRACE("[�۽�]���̴��м�-��� ȥ�⵵ ����\n");
				}
				else
				{
					TRACE("[�۽� ����]���̴��м�-��� ȥ�⵵ ����\n");
				}	
			}								
		}
		break;

	case OPCODE_TF_BD_SENDERROR:	// ���̴���Ž-���̴��м� ���� ����
		{
			TRACE("[����]���̴���Ž-���̴��м� ���� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDERROR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ���� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ���� ����\n");
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

	//////// �ڽ��� IP ȹ�� //////////////////////////////////////////////////////////////////
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

		TRACE("\n ================= PDW ���� : %d,  �����ð� : %d ====================\n", pTaskData->uiNumOfAcqPuls, pTaskData->uiAcquisitionTime); 
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
		//3���� ������ 
		STxAcqTraskRequest AcqtaskReq; 
		AcqtaskReq.uiNumOfTask = 1;
		memcpy(AcqtaskReq.stTaskData, &m_stCurTaskData, sizeof(m_stCurTaskData));

		STMsg stMsg;
		memcpy(stMsg.buf, &AcqtaskReq, sizeof(AcqtaskReq));
		TRACE("\n ================= NEXT���� ID  %s, Freq %d ====================\n", m_stCurTaskData.aucTaskID, m_stCurTaskData.uiFreq); 

		//if(m_stCurTaskData.uiRvcUseStat1 == TK_USE)
		//{
		bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_1, Equip_Rev_BD, m_OVIP, stMsg.buf);	// ���ű�#1-Bypass

		if(bRtnSend == true)
		{
			TRACE("[�۽�]���̴��м�-���̴���Ž#1 ���� NEXT ���� �䱸\n");
		}
		else
		{
			TRACE("[�۽� ����]���̴��м�-���̴���Ž#1 ���� NEXT ���� �䱸\n");
		}
		//}

		//if(m_stCurTaskData.uiRvcUseStat2 == TK_USE)
		//{
		//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_2, Equip_Rev_BD, m_OVIP, stMsg.buf);	// ���ű�#2

		//if(bRtnSend == true)
		//{
		//	TRACE("[�۽�]���̴��м�-���̴���Ž#2 ���� NEXT ���� �䱸\n");
		//}
		//else
		//{
		//	TRACE("[�۽� ����]���̴��м�-���̴���Ž#2 ���� NEXT ���� �䱸\n");
		//}
		//}

		//if(m_stCurTaskData.uiRvcUseStat3 == TK_USE)
		//{
		//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_3, Equip_Rev_BD, m_OVIP, stMsg.buf);	// ���ű�#3

		//if(bRtnSend == true)
		//{
		//	TRACE("[�۽�]���̴��м�-���̴���Ž#3 ���� NEXT ���� �䱸\n");
		//}
		//else
		//{
		//	TRACE("[�۽� ����]���̴��м�-���̴���Ž#3 ���� NEXT ���� �䱸\n");
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

	//3���� ������ 
	STxAcqTraskRequest AcqtaskReq; 
	AcqtaskReq.uiNumOfTask = 1;
	memcpy(AcqtaskReq.stTaskData, &m_stCurTaskData, sizeof(m_stCurTaskData));

	STMsg stMsg;
	memcpy(stMsg.buf, &AcqtaskReq, sizeof(AcqtaskReq));
	TRACE("\n ================= NEXT���� ID  %s, Freq %d ====================\n", m_stCurTaskData.aucTaskID, m_stCurTaskData.uiFreq); 

	bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_1, Equip_Rev_BD, m_OVIP, stMsg.buf);	// ���ű�#1-Bypass

	if(bRtnSend == true)
	{
		TRACE("[�۽�]���̴��м�-���̴���Ž#1 ���� NEXT ���� �䱸\n");
	}
	else
	{
		TRACE("[�۽� ����]���̴��м�-���̴���Ž#1 ���� NEXT ���� �䱸\n");
	}
	
	//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_2, Equip_Rev_BD, m_OVIP, stMsg.buf);	// ���ű�#2

	//if(bRtnSend == true)
	//{
	//	TRACE("[�۽�]���̴��м�-���̴���Ž#2 ���� NEXT ���� �䱸\n");
	//}
	//else
	//{
	//	TRACE("[�۽� ����]���̴��м�-���̴���Ž#2 ���� NEXT ���� �䱸\n");
	//}
	
	//bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQNEXTTASK, sizeof(STxAcqTraskRequest), SEND_DP_3, Equip_Rev_BD, m_OVIP, stMsg.buf);	// ���ű�#3

	//if(bRtnSend == true)
	//{
	//	TRACE("[�۽�]���̴��м�-���̴���Ž#3 ���� NEXT ���� �䱸\n");
	//}
	//else
	//{
	//	TRACE("[�۽� ����]���̴��м�-���̴���Ž#3 ���� NEXT ���� �䱸\n");
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
		TRACE("===���� �����ð� ����� ���� �������� ��û m_bAsqTotalFlag %d==========\n", m_bAsqTotalFlag);

		if(m_bAsqTotalFlag == false || (m_bAsqDoneFlag[SEND_DP_1] == false /*&& m_bAsqDoneFlag[SEND_DP_2] == false && m_bAsqDoneFlag[SEND_DP_3] == false*/) )
		{
			if(m_bDensityTotalFlag == false)
				SendDensityDataFromTimeOut();

			TRACE("======== Ÿ�̸ӿ� ���� ���� ���� ����=========\n");
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
				//Ÿ�Ӿƿ����� ���� �ʱ�ȭ ��� ����
				StopEquipCheckAckTimer(EQ_INT);
				m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_1);	
				m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_2);	
				m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, sizeof(m_stInitReqRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stInitReqRslt_3);	

				TRACE("======== Ÿ�̸ӿ� ���� �ʱ�ȭ �䱸 ���� =========\n");
			}
		}
		break;
	case EQ_SW:
		{
			m_iEQChkSWAckCnt++;

			if((m_iEQChkSWAckCnt * EQ_CHK_PERID >= EQ_CHK_TIMEOUT) )
			{
				//Ÿ�Ӿƿ����� ���� SW ���� ��� ����
				StopEquipCheckAckTimer(EQ_SW);
				m_iEQChkSWAckCnt = 0;	

				m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_1);
				m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_2);	
				m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, sizeof(m_stSWVerRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stSWVerRslt_3);		

				TRACE("======== Ÿ�̸ӿ� ���� SW���� �䱸 ���� =========\n");
			}
		}
		break;
	case EQ_PBT:
		{
			m_iEQChkPBITAckCnt++;

			if((m_iEQChkPBITAckCnt * EQ_CHK_PERID >= EQ_CHK_TIMEOUT) )
			{
				//Ÿ�Ӿƿ����� ���� PBIT ��� ����
				StopEquipCheckAckTimer(EQ_PBT);
				m_iEQChkPBITAckCnt = 0;	

				m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_1), SEND_DP_1, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_1);
				m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_2), SEND_DP_2, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_2);
				m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, sizeof(m_stPBITRslt_3), SEND_DP_3, Equip_Rev_0V, m_OVIP, &m_stPBITRslt_3);

				TRACE("======== Ÿ�̸ӿ� ���� �ʱ���ü���� �䱸 ���� =========\n");
			}
		}
		break;
			
				
	case EQ_IBT:
		{
			m_iEQChkIBITAckCnt++;

			if((m_iEQChkIBITAckCnt * EQ_CHK_PERID >= EQ_CHK_TIMEOUT) )
			{
				//Ÿ�Ӿƿ����� ���� IBIT ��� ����
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

	TRACE("************** Ÿ�Ӿƿ��� ���� ȥ�⵵ freq : %d,  1 : %d, 2 : %d, 3 : %d===================\n", stDensityTotalData.uiFreq, stDensityTotalData.fDensity_1, stDensityTotalData.fDensity_2, stDensityTotalData.fDensity_3);

	if(bRtnSend == true)
	{
		TRACE("[�۽�]Ÿ�Ӿƿ��� ���� ���̴��м�-��� ȥ�⵵ ����\n");
	}
	else
	{
		TRACE("[�۽� ����]Ÿ�Ӿƿ��� ���� ���̴��м�-��� ȥ�⵵ ����\n");
	}		

	ClearDensityFlag();
}

void CRadarAnalysisMngr::SendDummyMsg()
{
	int rslt = 1;
	
	m_hCommIF.Send(OPCODE_BD_OV_REQCHCALIBCHECK, sizeof(rslt), SEND_DP_1, Equip_Rev_0V, m_OVIP, (void*)&rslt);	
}