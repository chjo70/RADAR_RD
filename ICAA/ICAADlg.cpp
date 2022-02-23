
// ICAADlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ICAA.h"
#include "ICAADlg.h"

#include "ThreadTask\\DFTaskMngr.h"
#include "ADSBReceivedProcessMngr.h"

#include "RadarAnlAlgorithm.h"

#include "afxdialogex.h"

#include <sstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

#define     MAX_LOGCOUNT        (1000)

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CICAADlg 대화 상자




CICAADlg::CICAADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CICAADlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iTotalNumOfLog = 1;
	m_bFlagLogShow = true;	
}

CICAADlg::~CICAADlg()
{
	
}

void CICAADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_LoglistCtrl);
	DDX_Control(pDX, IDC_BTN_OPERATOR_STATUS, m_CButtonOperatorStatus);
	DDX_Control(pDX, IDC_BTN_RADARDR_STATUS, m_CButtonRadarDRStatus);
}

BEGIN_MESSAGE_MAP(CICAADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SAVELOG, &CICAADlg::OnBnClickedBtnSavelog)
	ON_BN_CLICKED(IDC_BTN_CLEARLOG, &CICAADlg::OnBnClickedBtnClearlog)
	ON_BN_CLICKED(IDC_BUTTON1, &CICAADlg::OnBnClickedButton1)

	ON_MESSAGE(UWM_USER_LOG_MSG, OnLOGMessage )
	ON_MESSAGE(UWM_USER_STAT_MSG, OnStatus )
	ON_BN_CLICKED(IDC_BTN_LOADCEDEOB, &CICAADlg::OnBnClickedBtnLoadcedeob)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CICAADlg 메시지 처리기

BOOL CICAADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// 최상단 메뉴 틀 제공
	g_DlgHandle = GetSafeHwnd();


	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	CRect rt;

	EnableRadarDRStatus( FALSE );
	EnableOperatorStatus( FALSE );

	m_LoglistCtrl.GetWindowRect(&rt);
	m_LoglistCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_LoglistCtrl.InsertColumn(0, TEXT("순번"), LVCFMT_LEFT, (int)(rt.Width()*0.1));
	m_LoglistCtrl.InsertColumn(1, TEXT("항목"), LVCFMT_LEFT, (int)(rt.Width()*0.1));
	m_LoglistCtrl.InsertColumn(2, TEXT("내용"), LVCFMT_LEFT, (int)(rt.Width()*0.8));

	this->SetWindowText("레이더 분석(관리/식별)");

	m_pTheICAAMngr = new CICAAMngr;


	//CICAAMngr::GetInstance();
	CADSBReceivedProcessMngr::GetInstance();

	Log( enSYSTEM , "프로그램이 시작되었습니다." );

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CICAADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CICAADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CICAADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CICAADlg::OnBnClickedBtnSavelog()
{
	// 현재 리스트에 등록되어 있는 로그를 저장함
	int iItemCnt = m_LoglistCtrl.GetItemCount();

	if(iItemCnt > 0)
	{
		stringstream ssTempSaveLog = stringstream();

		CStdioFile cSaveLogFile;
		CString cstrOneColume;
		int iStrLen = 0;

		cSaveLogFile.Open("Log.txt", CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::typeText);

		for(int i=0; i < iItemCnt; i++)
		{
			cstrOneColume = m_LoglistCtrl.GetItemText(i, 0);
			ssTempSaveLog << cstrOneColume << "\t";
			cstrOneColume = "";

			cstrOneColume = m_LoglistCtrl.GetItemText(i, 1);
			ssTempSaveLog << cstrOneColume << "\t";
			cstrOneColume = "";

			cstrOneColume = m_LoglistCtrl.GetItemText(i, 2);
			ssTempSaveLog << cstrOneColume << "\n";
			cstrOneColume = "";

			cSaveLogFile.WriteString(ssTempSaveLog.str().c_str());
			ssTempSaveLog.str("");
		}

		cSaveLogFile.Close();

		stringstream ssTemp = stringstream("");

		ssTemp << m_iTotalNumOfLog;

		m_LoglistCtrl.InsertItem(m_iTotalNumOfLog, ssTemp.str().c_str());
		m_LoglistCtrl.SetItem(m_iTotalNumOfLog, 1, LVIF_TEXT, TEXT("일반"), NULL, NULL, NULL, NULL);
		m_LoglistCtrl.SetItem(m_iTotalNumOfLog, 2, LVIF_TEXT, TEXT("파일이 저장되었습니다."), NULL, NULL, NULL, NULL);

		m_iTotalNumOfLog++;

		ssTemp.str("");
	}
	else
	{
		AfxMessageBox("저장할 수 있는 로그가 존재하지 않습니다.");
	}
}


void CICAADlg::OnBnClickedBtnClearlog()
{
	int iResult = AfxMessageBox("로그를 정말로 지우시겠습니까?", MB_OKCANCEL);

	if(iResult == 1)
	{
		m_bFlagLogShow = false;
		m_iTotalNumOfLog = 0;
		m_LoglistCtrl.DeleteAllItems();
		m_bFlagLogShow = true;
	}
	else
	{

	}
	
}


void CICAADlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDFTaskMngr::GetInstance()->SendDummyMsg();

	//CRadarAnalysisMngr::GetInstance()->SendDummyMsg();
}



LRESULT CICAADlg::OnLOGMessage( WPARAM wParam, LPARAM lParam )
{
	// 메시지를 받아서 처리하는 함수
	enENUM_ITEM enItemType = (enENUM_ITEM ) wParam;
	char *pszContents = (char *) lParam;
	
	TRACE( "\n" );
	TRACE( pszContents );
	Log( enItemType, pszContents );

	//AfxMessageBox(msg->GetString());
	return 0;
}

void CICAADlg::Log( enENUM_ITEM enItemType , char *pszContents )
{
	STR_LOGMESSAGE stMsg;

	stMsg.enItemType = enItemType;
	strcpy( stMsg.szContents, pszContents );

	Log( & stMsg );
}

/**
 * @brief     Log
 * @param     STR_LOGMESSAGE * pMsg
 * @return    void
 * @exception
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2022-01-23, 13:18
 * @warning
 */
void CICAADlg::Log( STR_LOGMESSAGE *pMsg )
{
	int nItemNum = m_LoglistCtrl.GetItemCount();

	char szBuffer[100];

    if( nItemNum > MAX_LOGCOUNT ) {
        m_LoglistCtrl.DeleteAllItems();
        nItemNum = m_LoglistCtrl.GetItemCount();
    }

	sprintf( szBuffer, "%d" , m_iTotalNumOfLog );
	m_LoglistCtrl.InsertItem( nItemNum, szBuffer );

	sprintf( szBuffer, "%s" , g_stLogItemType[ pMsg->enItemType ] );
	m_LoglistCtrl.SetItemText( nItemNum, 1, szBuffer );
	m_LoglistCtrl.SetItemText( nItemNum, 2, pMsg->szContents );

	m_LoglistCtrl.SendMessage( WM_VSCROLL, SB_BOTTOM );

	++ m_iTotalNumOfLog;

}

/**
 * @brief     EnableRadarDRStatus
 * @param     BOOL bEnable
 * @return    void
 * @exception
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2022-01-23, 13:18
 * @warning
 */
void CICAADlg::EnableRadarDRStatus( BOOL bEnable )
{
	if( bEnable == TRUE ) {
		m_CButtonRadarDRStatus.SetWindowTextA( "레이더 방탐 연결" );
		m_CButtonRadarDRStatus.SetFaceColor(RGB(0,0,255),true);
	}
	else {
		m_CButtonRadarDRStatus.SetFaceColor(RGB(255,0,0),true);
		m_CButtonRadarDRStatus.SetWindowTextA( "레이더 방탐 미연결" );
	}

}

/**
 * @brief     EnableOperatorStatus
 * @param     BOOL bEnable
 * @return    void
 * @exception
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2022-01-23, 13:18
 * @warning
 */
void CICAADlg::EnableOperatorStatus( BOOL bEnable )
{
	if( bEnable == TRUE ) {
		m_CButtonOperatorStatus.SetWindowTextA( "운용 소프트웨어 연결" );
		m_CButtonOperatorStatus.SetFaceColor(RGB(0,0,255),true);
	}
	else {
		m_CButtonOperatorStatus.SetWindowTextA( "운용 소프트웨어 미연결" );
		m_CButtonOperatorStatus.SetFaceColor(RGB(255,0,0),true);
	}

}

/**
 * @brief     OnStatus
 * @param     WPARAM wParam
 * @param     LPARAM lParam
 * @return    LRESULT
 * @exception
 * @author    조철희 (churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2022-01-23, 13:18
 * @warning
 */
LRESULT CICAADlg::OnStatus( WPARAM wParam, LPARAM lParam )
{
	// 메시지를 받아서 처리하는 함수
	enENUM_STATUS_UNIT enStatusUnit = (enENUM_STATUS_UNIT ) wParam;
	BOOL bEnable = (BOOL ) lParam;

	switch( enStatusUnit ) {
	case enOperator :
		EnableOperatorStatus( bEnable );
		break;
	case enRADARDR :
		EnableRadarDRStatus( bEnable );
		break;

	default:
		break;
	}

	return 0;
}

void CICAADlg::OnBnClickedBtnLoadcedeob()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	RadarAnlAlgotirhm::RadarAnlAlgotirhm::UpdateCEDEOBLibrary();
}


void CICAADlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete m_pTheICAAMngr;

}
