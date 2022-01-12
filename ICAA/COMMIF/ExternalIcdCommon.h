#ifndef __EXTERNALICDCOMMON_H__
#define __EXTERNALICDCOMMON_H__

#pragma pack(1)
#if defined(BIG_ENDIAN)
	typedef union
	{
		UINT				uiTData;
		struct
		{
			UINT		iYear				: 6	;	// �� (��: 2013�� -> 13)
			UINT		iMonth				: 4	;	// ��
			UINT		iDayOfMonth			: 5	;	// ��
			UINT		iHour				: 5	;	// �� (24�� ����, ��:���� 1��-> 13)
			UINT		iMinute				: 6	;	// ��
			UINT		iSecond				: 6	;	// ��
		}BIT_FIELD;	
	}unTime;	
#else
	typedef union
	{
		UINT				uiTData;
		struct
		{
			UINT		iSecond				: 6	;	// ��
			UINT		iMinute				: 6	;	// ��
			UINT		iHour				: 5	;	// �� (24�� ����, ��:���� 1��-> 13)
			UINT		iDayOfMonth			: 5	;	// ��
			UINT		iMonth				: 4	;	// ��
			UINT		iYear				: 6	;	// �� (��: 2013�� -> 13)
		}BIT_FIELD;	
	}unTime;	
#endif	//	#if defined(BIG_ENDIAN)

	
#pragma pack()

#endif // #ifndef __EXTERNALICDCOMMON_H__
