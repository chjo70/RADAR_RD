
// ICAADlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "COMMIF/CGRCommIF.h"
#include "ICAAMngr.h"
#include "ThreadTask/DFTaskMngr.h"
#include "ThreadTask/DFEquipBITMngr.h"
#include "RadarAnalysisMngr.h"
#include "afxwin.h"

// CICAADlg 대화 상자

class CICAADlg : public CDialogEx
{
private:
    LONG m_lOpInitID;

// 생성입니다.
public:
	CICAADlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CICAADlg();

	void EnableRadarDRStatus( BOOL bEnable );
	void EnableOperatorStatus( BOOL bEnable );
	void Log( STR_LOGMESSAGE *pMsg );
	void Log( enENUM_ITEM enItemType , char *pszContents );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ICAA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_LoglistCtrl;

	int m_iTotalNumOfLog;
	bool m_bFlagLogShow;

public:
	afx_msg LRESULT OnLOGMessage( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStatus( WPARAM wParam, LPARAM lParam );

	afx_msg void OnBnClickedBtnSavelog();
	afx_msg void OnBnClickedBtnClearlog();

	CICAAMngr *m_pTheICAAMngr;
	CDFTaskMngr m_DFTaskMngr;
	CDFEquipBITMngr m_DFEquipBITMngr;
	CRadarAnalysisMngr m_RadarAnlyMngr;
	afx_msg void OnBnClickedButton1();
	CMFCButton m_CButtonOperatorStatus;
	CMFCButton m_CButtonRadarDRStatus;
	afx_msg void OnBnClickedBtnLoadcedeob();
	afx_msg void OnDestroy();
};
