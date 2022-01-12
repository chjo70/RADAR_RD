#include "stdafx.h"
#include "ComplibFacade.h" 

CComplibFacade::CComplibFacade()
{
	m_uiBufferSize = compressBound(SZ_MAX_MSG_LEN * ZLIB_BUFFER_MARGIN);
	TRACE1("buffer size:%d\n", m_uiBufferSize);

	m_ucInflateBuffer = new UCHAR[m_uiBufferSize];
	if(m_ucInflateBuffer == NULL)
	{
		TRACE0("Memory Alloc Fail\n");
	}

	m_ucDeflateBuffer = new UCHAR[m_uiBufferSize];
	if(m_ucDeflateBuffer == NULL)
	{
		TRACE0("Memory Alloc Fail\n");
	}
}

CComplibFacade::~CComplibFacade()
{
	if(m_ucInflateBuffer!=NULL)
	{
		delete m_ucInflateBuffer;
		m_ucInflateBuffer = NULL;
	}

	if(m_ucDeflateBuffer!=NULL)
	{
		delete m_ucDeflateBuffer;
		m_ucDeflateBuffer = NULL;
	}
}

UCHAR* CComplibFacade::inflate(ULONG *ulResultLen, const UCHAR *ucSrcData, ULONG ulSrcLen)
{
	//TRACE0("CComplibFacade::inflate\n");
	if ( !ucSrcData )
	{
		TRACE1("[Src:0x%x]\n", ucSrcData);
		return NULL;
	}

	if ( ulSrcLen<=0 )
	{
		*(ulResultLen) = 0;
		return m_ucInflateBuffer;
	}

	// [주의] ulDestLen은 compress2 수행 후 받는 결과값(압축후 데이터 크기)이나 적절하게 큰 값을 넣어주지 않으면 에러 발생함.
	*(ulResultLen) = m_uiBufferSize;

	int result;
	result = compress2(m_ucInflateBuffer, ulResultLen, ucSrcData, ulSrcLen, Z_BEST_SPEED);

	if(result == Z_OK)		// if success
	{
		return m_ucInflateBuffer;
	}
	else if(result == Z_MEM_ERROR)	// if there was not enough memory
	{
		TRACE1("not enough memory:%d\n", result);
		return NULL;
	}
	else if(result == Z_BUF_ERROR)	// if there was not enough room in the output buffer
	{
		TRACE1("not enough room in the output buffer:%d\n", result);
		return NULL;
	}
	else if(result == Z_STREAM_ERROR)	// if the level parameter is invalid
	{
		TRACE1("level parameter is invalid:%d\n", result);
		return NULL;
	}
	else	// if unknown error
	{
		TRACE1("unknown error:%d\n", result);
		return NULL;
	}
	return NULL;
}

UCHAR* CComplibFacade::deflate(ULONG *ulResultLen, const UCHAR *ucSrcData, ULONG ulSrcLen)
{
	if (!ucSrcData )
	{
		TRACE1("Not valid source:%x", ucSrcData);
		return NULL;
	}

	if ( ulSrcLen<=0 )
	{
		*(ulResultLen) = 0;
		return m_ucDeflateBuffer;
	}

	// [주의] ulDestLen은 compress2 수행 후 받는 결과값(압축후 데이터 크기)이나 적절하게 큰 값을 넣어주지 않으면 에러 발생함.
	*(ulResultLen) = m_uiBufferSize;

	int result;
	result = uncompress(m_ucDeflateBuffer, ulResultLen, ucSrcData, ulSrcLen);

	if(result == Z_OK)		// if success
	{
		return m_ucDeflateBuffer;
	}
	else if(result == Z_MEM_ERROR)	// if there was not enough memory
	{
		TRACE1("not enough memory:%d\n", result);
		return NULL;
	}
	else if(result == Z_BUF_ERROR)	// if there was not enough room in the output buffer
	{
		TRACE1("not enough room in the output buffer:%d\n", result);
		return NULL;
	}
	else if(result == Z_DATA_ERROR)	// input data was corrupted or incomplete
	{
		TRACE1("input data was corrupted or incomplete:%d\n", result);
		return NULL;
	}
	else	// if unknown error
	{
		TRACE1("unknown error:%d\n", result);
		return NULL;
	}
	return NULL;
}
