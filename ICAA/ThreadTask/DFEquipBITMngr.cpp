#include "stdafx.h"
#include "DFEquipBITMngr.h"
#include "..\MsgQueueThread.h"



CDFEquipBITMngr* CDFEquipBITMngr::m_pInstance=nullptr;

CDFEquipBITMngr::CDFEquipBITMngr()
:m_hCommIF_DFEquipBITMngr(m_hCommIF)
{
	//�ʱ�ȭ ���
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_INIT_SYS_RESULT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//SW���� ���
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_SW_VERSION_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//�ʱ���ü���� ���	
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_PBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//�������ü���� ���				
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_IBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	////ä�κ��� �������				
	//m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_CHNEL_CORECT, DEVICECODE_BRA, DEVICECODE_TRD), this);				
}

CDFEquipBITMngr::~CDFEquipBITMngr()
{
	//�ʱ�ȭ ��� 
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_INIT_SYS_RESULT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//SW���� ���
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_SW_VERSION_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//�ʱ���ü���� ���	
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_PBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//�������ü���� ���				
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


void CDFEquipBITMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE���� ��츦 �����Ͽ� ó���ϴ� ���� ���� �ʿ�
	// �ϴܿ� ����

	TRACE("[Log] CDFEquipBITMngr ProcessMsg ȣ��.\n");

	UEL_BITMAP_OPCode stOpCode= UEL_BITMAP_OPCode() ;
	stOpCode.w32 = i_stMsg.uiOpcode;


	switch(stOpCode.bData.uchCMDCode)
	{
	//�ʱ�ȭ ���
	case CMDCODE_DF_TX_INIT_SYS_RESULT:
		{

		}
		break;
	//SW���� ���
	case CMDCODE_DF_TX_SW_VERSION_RSLT:
		{

		}
		break;
	//�ʱ���ü���� ���	
	case CMDCODE_DF_TX_PBIT_RSLT:
		{

		}
		break;
	//�������ü���� ���				
	case CMDCODE_DF_TX_IBIT_RSLT:
		{

		}
		break;
	
	default:
		break;
	}
}
