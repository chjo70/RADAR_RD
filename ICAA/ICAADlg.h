
// ICAADlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "COMMIF/CGRCommIF.h"
#include "ICAAMngr.h"
#include "ThreadTask/DFTaskMngr.h"
#include "ThreadTask/DFEquipBITMngr.h"
#include "RadarAnalysisMngr.h"

// CICAADlg ��ȭ ����

class CICAADlg : public CDialogEx
{
// �����Դϴ�.
public:
	CICAADlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CICAADlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ICAA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_LoglistCtrl;

public:
	int m_iTotalNumOfLog;
	bool m_bFlagLogShow;

	afx_msg void OnBnClickedBtnSavelog();
	afx_msg void OnBnClickedBtnClearlog();

	CICAAMngr m_cICAAMngr;
	CDFTaskMngr m_DFTaskMngr;
	CDFEquipBITMngr m_DFEquipBITMngr;
	CRadarAnalysisMngr m_RadarAnlyMngr;
	afx_msg void OnBnClickedButton1();
};
