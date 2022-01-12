#ifndef __EXTERNALICD_H__
#define __EXTERNALICD_H__

#include "ExternalICDCommon.h"

#pragma pack(1)

#define SZ_MAX_DATA_LEN						65536						// 64K
#define SZ_MAX_MSG_LEN						(SZ_MAX_DATA_LEN+20)		// 64K+20(stLHdrNFrame+stMHdr)
#define SZ_MAX_LOGMSG_LEN					(SZ_MAX_DATA_LEN-4)			// 64K-4(usTotalCount+usCurrentNumber)

#define VOICE_FRAME_COUNT					2							// 1ȸ ����(40ms ��)��  �����ϴ� ������ ����
#define VOICE_FRAME_DATA_COMP_SIZE			70							// ������ �� ������ ũ��(���ڵ���) = 160 ����  x ���� �� 2����Ʈ
#define VOICE_FRAME_DATA_UNCOMP_SIZE		640							// ������ �� ������ ũ��(���ڵ���) = 160 ���� x 20ms x ���� �� 2����Ʈ

/*********************************************************************
*
*                           �ܺ� �������̽�
*
*                              ��Ŷ ����
*
*********************************************************************/
// ��ũ ��� �� LSize�� ���Ե��� �ʴ� ����
#if defined(BIG_ENDIAN)
	typedef struct
	{
		UINT		ACID				: 4	;	// �װ��� �ĺ� ��ȣ
		UINT		E					: 1	;	// ��ȣȭ ����
		UINT		C					: 1	;	// ���� ����
		UINT		A					: 1	;	// ACK ����
		UINT		R					: 1	;	// ������ �ʿ� ����
		UINT		F					: 1	;	// Frame ���� ����
		UINT		LSize               : 23;	// ��ü ũ��
	} stLHdrFlag;	

	typedef union
	{
		UINT			uiData;
		stLHdrFlag		stData;
	}unLHdrFlag;
#else
	typedef struct
	{
		UINT		LSize               : 23;	// ��ü ũ��
		UINT		F					: 1	;	// Frame ���� ����
		UINT		R					: 1	;	// ������ �ʿ� ����
		UINT		A					: 1	;	// ACK ����
		UINT		C					: 1	;	// ���� ����
		UINT		E					: 1	;	// ��ȣȭ ����
		UINT		ACID				: 4	;	// �װ��� �ĺ� ��ȣ
	} stLHdrFlag;

	typedef union
	{
		UINT			uiData;
		stLHdrFlag		stData;
	}unLHdrFlag;
#endif	//	#if defined(BIG_ENDIAN)

// ��ũ ��� �� LSize�� ���ԵǴ� ����
typedef struct
{
	UINT		SeqNum;	// �޽��� �Ϸ� ��ȣ
} stLHdrRest;

// ��ũ���(������1)
#if defined(BIG_ENDIAN)	
	typedef struct
	{
		unLHdrFlag LHdrFlag;				//
		UINT	   uiSeqNum;				// �޽��� �Ϸ� ��ȣ
	} stLHdr1Frame;
#else
	typedef struct
	{
		unLHdrFlag LHdrFlag;				//
		UINT	   uiSeqNum;				// �޽��� �Ϸ� ��ȣ
	} stLHdr1Frame;
#endif	//	#if defined(BIG_ENDIAN)

// ��ũ���(������N)
#if defined(BIG_ENDIAN)
	typedef struct
	{
		unLHdrFlag	LHdrFlag;				//
		UINT		uiSeqNum;				// �޽��� �Ϸ� ��ȣ
		USHORT		usTotalFrameCount;		// ��ü Frame ��(Option)
		USHORT		usFrameNum;				// Frame ��ȣ(Option)
	} stLHdrNFrame;	
#else
	typedef struct
	{
		unLHdrFlag	LHdrFlag;				//
		UINT		uiSeqNum;				// �޽��� �Ϸ� ��ȣ
		USHORT		usTotalFrameCount;		// ��ü Frame ��(Option)
		USHORT		usFrameNum;				// Frame ��ȣ(Option)
	} stLHdrNFrame;	
#endif	//	#if defined(BIG_ENDIAN)

// OPCODE
	#if defined(BIG_ENDIAN)
	typedef union
	{
		UCHAR				ucData;
		struct
		{
			UCHAR   Encrytion       : 1 ;		// ���࿩��
			UCHAR   Class			: 4	;		// �޽��� �켱����
			CHAR   O				: 4	;		// ����� ����			
		}BIT_FIELD;
	}  unCO;
#else
	typedef union
	{
		UCHAR				ucData;
		struct
		{
			UCHAR   O				: 4	;		// ����� ����
			UCHAR   Class			: 3	;		// �޽��� �켱����
			UCHAR   Encrytion       : 1 ;		// ���࿩��
		}BIT_FIELD;
	} unCO; 
#endif	//	#if defined(BIG_ENDIAN)

#if defined(BIG_ENDIAN)
	typedef struct
	{
		UCHAR   Src;						// �۽����
		UCHAR   Dest;						// �������
		UCHAR   CmdCode;					// ���� ���
		unCO	CO;
	} stOpcode;	
#else
	typedef struct
	{
		unCO	CO;
		UCHAR   CmdCode;					// ���� ���
		UCHAR   Dest;						// �������
		UCHAR   Src;						// �۽����		
	} stOpcode;
#endif	//	#if defined(BIG_ENDIAN)

// �ӹ� ���
typedef struct
{
	stOpcode	Opcode;				// Opcode
	USHORT		usMSize;			// ������ ���� ũ��
	UCHAR		ucSourcdeOprID;		// SOURCE ����� �����ʵ�
	UCHAR		ucDestinationOprID; // Destination ����� �����ʵ�
	//USHORT		usOperatorID;		// ����� �ĺ� ID
} stMHdr;

// ��ũ������(������N)
typedef struct
{
	stLHdrNFrame	LHdr;					// ��ũ ���
	stMHdr			MHdr;					// �ӹ� ���
	char			Data[SZ_MAX_DATA_LEN];	// ������
} stMsgNFrame;

typedef union
{
	stMsgNFrame  Msg;
	char		 Data[SZ_MAX_MSG_LEN];
} unMsgNFrame;

// ��ũ������(������1)
typedef struct
{
	stLHdr1Frame	LHdr;					// ��ũ ���
	stMHdr			MHdr;					// �ӹ� ���
	char			Data[SZ_MAX_DATA_LEN];	// ������
} stMsg1Frame;

typedef union
{
	stMsg1Frame  Msg;
	char		 Data[SZ_MAX_MSG_LEN];
} unMsg1Frame;

// �ӹ�������
typedef struct
{
	stMHdr		MHdr;					// �ӹ� ���
	char		Data[SZ_MAX_DATA_LEN];	// ������
} stMsgMission;

typedef union
{
	stMsgMission  Msg;
	char		  Data[SZ_MAX_MSG_LEN];
} unMsgMission;

/*********************************************************************
*
*                           �ܺ� �������̽�
*
*                              OPCODE
*
*********************************************************************/
// GMEI->AIE
#define EOPCODE_GMEI_ReqInit						0x01		// �ʱ�ȭ �䱸
#define EOPCODE_GMEI_AckInitResult					0x02		// �ʱ�ȭ ��� ����
#define EOPCODE_GMEI_ReqPBIT						0x03		// �ʱ���ü���� �䱸
#define EOPCODE_GMEI_AckPBITResult					0x04		// �ʱ���ü���� ��� ����
#define EOPCODE_GMEI_ReqCBIT						0x05		// ������ü���� �䱸
#define EOPCODE_GMEI_ReqIBIT						0x06		// ����� ��ü���� �䱸
#define EOPCODE_GMEI_AckIBITResult					0x07		// ����� ��ü���� ��� ����
#define EOPCODE_GMEI_ReqFISINTPathCheck				0x08		// FISINT ������� �䱸
#define EOPCODE_GMEI_AckFISINTPathCheckResult		0x09		// FISINT ������� ��� ����
#define EOPCODE_GMEI_ReqPowerControl				0x12	    // �ӹ������������䱸	
#define EOPCODE_GMEI_AckPowerControlResult			0x13		// �ӹ�������������� ����	
#define EOPCODE_GMEI_AckPowerStatus					0x14		// �ӹ�����������º��� ����	
#define EOPCODE_GMEI_LogDataRequest					0x30		// ���嵥���� �䱸
#define EOPCODE_GMEI_UpwardVoice					0x40		// ��������	

// AIE->GMEI
#define EOPCODE_GMEI_AckInit						0x01		// �ʱ�ȭ �䱸 ����
#define EOPCODE_GMEI_SndInitResult					0x02		// �ʱ�ȭ ���
#define EOPCODE_GMEI_AckPBIT						0x03		// �ʱ���ü���� �䱸 ����
#define EOPCODE_GMEI_SndPBITResult					0x04		// �ʱ���ü���� ���
#define EOPCODE_GMEI_SndCBITResult					0x05		// ������ü���� ���
#define EOPCODE_GMEI_AckIBIT						0x06		// ����� ��ü���� �䱸����
#define EOPCODE_GMEI_SndIBITResult					0x07		// ����� ��ü���� ���
#define EOPCODE_GMEI_AckFISINTPathCheck				0x08		// FISINT ������� �䱸 ����
#define EOPCODE_GMEI_SndFISINTPathCheckResult		0x09		// FISINT ������� ���
#define EOPCODE_GMEI_SndZeroizeResult				0x11		// �ڷ���� �˸�
#define EOPCODE_GMEI_AckPowerControl				0x12		// �ӹ������������䱸����
#define EOPCODE_GMEI_SndPowerControlResult			0x13		// �ӹ��������������
#define EOPCODE_GMEI_SndPowerStatus					0x14		// �ӹ�����������º���
#define EOPCODE_GMEI_SndNavigationInfo				0x20		// �׹���������
#define EOPCODE_GMEI_SndTimeInfo					0x21		// �ð���������
#define EOPCODE_GMEI_LogData						0x30		// ���嵥���� ����
#define EOPCODE_GMEI_CreatedDataSize				0x31		// ���������ͷ� ����
#define EOPCODE_GMEI_DownwardVoice					0x41		// ��������
#define EOPCODE_AIE_GMEI_WOWState					0x50		// WOW����


/*********************************************************************
*
*                           �ܺ� �������̽�
*
*                             �ʵ尪 ����
*
*********************************************************************/
// ��/���� ��� �ڵ�
#define EQUIP_AIR_GND								0x80	// �װ�/���� ����
#define EQUIP_AIR_ACE								0x20	// �װ� COMINT ���(ACE)
#define EQUIP_AIR_AEE								0x10	// �װ� ELINT ���(AEE)
#define EQUIP_AIR_AFE								0x08	// �װ� FISINT ���(AFE)
#define EQUIP_AIR_PDE								0x04	// ȭ��Ž�����(PDE)
#define EQUIP_AIR_AIE								0x02	// �װ��������(AIE)
#define EQUIP_AIR_ADE								0x01	// �װ������͸�ũ���(ADE)
#define EQUIP_GND_GMEO								0x08	// �����ӹ����-���(GMEO)
#define EQUIP_GND_GMEI								0x04	// �����ӹ����-����(GMEI)
#define EQUIP_GND_GMED								0x02	// �����ӹ����-DL(GMED)
#define EQUIP_GND_GDE								0x01	// �������͸�ũ���(GDE)
#define EQUIP_BROADSTING							0x00	// ����(Broadsting)

#define EQUIP_INTERNAL_AIPU							0x21	// �װ��ӹ�������ġ
#define EQUIP_INTERNAL_PDU							0x22	// �����й���ġ
#define EQUIP_INTERNAL_IFBD_CTRL					0x23	// ���տ����� - ����
#define EQUIP_INTERNAL_IFBD_IDPU					0x24	// ���տ����� - ��ȣ��񿬵�
#define EQUIP_INTERNAL_SBC_HEARTBEAT				0x25	// SBC�� ����ȭ
#define EQUIP_INTERNAL_SBC_LOG						0x26	// SBC�� ����

// �װ��� ID
#define AIRCRAFT_ID_1								0x01
#define AIRCRAFT_ID_2								0x02

// Ŭ����
#define CLASS_1										1		// Ŭ���� 1 : ������, �м����
#define CLASS_2										2		// Ŭ���� 2 : Telemetry, ȭ��
#define CLASS_3										3		// Ŭ���� 3 : ����, ���� ���
#define CLASS_4										4		// Ŭ���� 4 : ��û ������
#define CLASS_5										5		// Ŭ���� 5 : ����������(����Ʈ��)
#define CLASS_COUNT									5		// Ŭ���� ����

// ����� ����
#define OPR_NA										0		// ����ڱ��� : NA
#define OPR_TST										1		// ����ڱ��� : TST
#define OPR_DLO										2		// ����ڱ��� : DLO
#define OPR_ETC										3		// ����ڱ��� : ��Ÿ(muphiri �߰� ���� �ʿ�)

// ����� ID
#define OPRID_DEFAULT								0		// �����ID : �⺻

#define FLAG_ENCRY_YES								0x0		// ��ȣȭ ���� : �����Ϳ��� ��ȣȭ
#define FLAG_ENCRY_NO								0x1		// ��ȣȭ ���� : �����Ϳ��� ���ȣȭ

#define FLAG_COMP_YES								0x0		// ���� ���� : �����Ϳ��� ����
#define FLAG_COMP_NO								0x1		// ���� ���� : �����Ϳ��� �����

#define FLAG_ACK_NO									0x0		// ACK ���� : �޽���
#define FLAG_ACK_YES								0x1		// ACK ���� : �޽����� ���� ACK

#define FLAG_RETRY_YES								0x0		// ������ �ʿ� ���� : ������ �ʿ�
#define FLAG_RETRY_NO								0x1		// ������ �ʿ� ���� : ������ ���ʿ�

#define FLAG_FRAME_YES								0x0		// Frame ���� ���� : Frame ����
#define FLAG_FRAME_NO								0x1		// Frame ���� ���� : Frame �̺���

#define DLSTAT_LINK_OK_EQUIP_OK						0xFFFF	// �����͸�ũ ����:��ũ ����, ��� ����
#define DLSTAT_LINK_OK_EQUIP_FAIL					0xFF00	// �����͸�ũ ����:��ũ ����, ��� �̻�
#define DLSTAT_LINK_FAIL_EQUIP_OK					0x00FF	// �����͸�ũ ����:��ũ ����, ��� ����
#define DLSTAT_LINK_FAIL_EQUIP_FAIL					0x0000	// �����͸�ũ ����:��ũ ����, ��� �̻�

#define EQUIP_STATE_NOT_VALID						0x0000	// Not Valid(�������κ��� ��û�� ���� �ʾ� �������ʵ� ��ȿ���� ����)
#define EQUIP_STATE_ING								0x0001	// ������(�������� ���۽� �������� ����������� ������ ��)
#define EQUIP_STATE_NOT_RESPOND						0x0002	// �������(�������ʵ���ȿ���� ����)
#define EQUIP_STATE_COMPLETE						0x0003	// �Ϸ�

#define ZEROIZE_STATE_ING							0x0000	// ������
#define ZEROIZE_STATE_COMPLETE						0x0001	// �Ϸ�

#define BIT_RESULT_FAIL								0		// ��ü���˰�� ����
#define BIT_RESULT_OK								1		// ��ü���˰�� ������

#define FISINT_PATH_OK								0x00	// C/E ��� ����
#define FISINT_PATH_COK_EFAIL						0x0F	// C��� ����/E��� ������
#define FISINT_PATH_CFAIL_EOK						0xF0	// C��� ������/E��� ����
#define FISINT_PATH_CFAIL_EFAIL						0xFF	// CE ��� ������

#define MSG_STATUS_ING								0		// ������
#define MSG_STATUS_COMPLETE							1		// �Ϸ�

#define MSG_STATUS_AEE_NOTDOING				0		// �̼���
#define MSG_STATUS_AEE_ING					1		// ������
#define MSG_STATUS_AEE_COMPLETE				2		// �Ϸ�

#define RETRY_NO_OPCODE_GR_OPERATOR_AUDIO		0x04824000

#define	MAX_RETRY_CNT				3
#define RETRY_INTERVAL_TIME			(3000)

#pragma pack()

#endif // #ifndef __EXTERNALICD_H__