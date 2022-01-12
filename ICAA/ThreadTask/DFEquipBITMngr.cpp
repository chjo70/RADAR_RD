#include "stdafx.h"
#include "DFEquipBITMngr.h"
#include "..\MsgQueueThread.h"



CDFEquipBITMngr* CDFEquipBITMngr::m_pInstance=nullptr;

CDFEquipBITMngr::CDFEquipBITMngr()
:m_hCommIF_DFEquipBITMngr(m_hCommIF)
{
	//초기화 결과
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_INIT_SYS_RESULT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//SW버전 결과
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_SW_VERSION_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//초기자체점검 결과	
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_PBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//운용자자체점검 결과				
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_IBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	////채널보정 진행상태				
	//m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_CHNEL_CORECT, DEVICECODE_BRA, DEVICECODE_TRD), this);				
}

CDFEquipBITMngr::~CDFEquipBITMngr()
{
	//초기화 결과 
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_INIT_SYS_RESULT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//SW버전 결과
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_SW_VERSION_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//초기자체점검 결과	
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_PBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//운용자자체점검 결과				
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_IBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);
}

CDFEquipBITMngr* CDFEquipBITMngr::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CDFEquipBITMngr();		
	}

	return m_pInstance;
}

void CDFEquipBITMngr::Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData)
{
	TRACE("[Log] DFTaskMngr received.\n");

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


void CDFEquipBITMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE별로 경우를 나열하여 처리하는 로직 구현 필요
	// 하단에 구현

	TRACE("[Log] CDFEquipBITMngr ProcessMsg 호출.\n");

	UEL_BITMAP_OPCode stOpCode= UEL_BITMAP_OPCode() ;
	stOpCode.w32 = i_stMsg.uiOpcode;


	switch(stOpCode.bData.uchCMDCode)
	{
	//초기화 결과
	case CMDCODE_DF_TX_INIT_SYS_RESULT:
		{

		}
		break;
	//SW버전 결과
	case CMDCODE_DF_TX_SW_VERSION_RSLT:
		{

		}
		break;
	//초기자체점검 결과	
	case CMDCODE_DF_TX_PBIT_RSLT:
		{

		}
		break;
	//운용자자체점검 결과				
	case CMDCODE_DF_TX_IBIT_RSLT:
		{

		}
		break;
	
	default:
		break;
	}
}
