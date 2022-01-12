//
// Ping.h
//

//#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include <winsock2.h>
#include <windows.h>

struct ICMPheader
{
	unsigned char	byType;
	unsigned char	byCode;
	unsigned short	nChecksum;
	unsigned short	nId;
	unsigned short	nSequence;
};

struct IPheader
{
    unsigned char	byVerLen;        
    unsigned char	byTos;       
    unsigned short	nTotalLength;   
    unsigned short	nId;            
    unsigned short	nOffset;        
    unsigned char	byTtl;          
    unsigned char	byProtocol;     
    unsigned short	nChecksum;      
    unsigned int	nSrcAddr;       
    unsigned int	nDestAddr;      
};

using namespace std;


class CPingICMP
{
public:
	
	/**
	* ���� ��� ��ü���� ����� ������ �����ϱ� ���� ������ �ڵ�
	*/
	bool Ping(UINT nRetries,LPCSTR pstrHost,HWND hWnd, int iBITKind);	
	unsigned short CalcChecksum (char *pBuffer, int nLen);
	bool ValidateChecksum (char *pBuffer, int nLen);
	bool Initialize ();
	bool UnInitialize ();
	bool ResolveIP (char *pszRemoteHost, char **pszIPAddress);
};


