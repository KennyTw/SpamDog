// LiveUpdateDlg.h : header file
//

#if !defined(AFX_LIVEUPDATEDLG_H__747FB09A_7CF2_4E19_A453_428EAA06D8F3__INCLUDED_)
#define AFX_LIVEUPDATEDLG_H__747FB09A_7CF2_4E19_A453_428EAA06D8F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WM_CLOSESPAMDOG WM_USER+1222
/////////////////////////////////////////////////////////////////////////////
// CLiveUpdateDlg dialog

class CLiveUpdateDlg : public CDialog
{
// Construction
private:
	char CallType;
	int TimeCounter;
	NOTIFYICONDATA tnd;

#ifdef _CONSOLEDBG
	HANDLE hConsole;    
#endif

	void OnOK();
	void OnCancel();

	
	bool FirstStart;
	bool InProcess;
	
	
public:
	CLiveUpdateDlg(CWnd* pParent = NULL);	// standard constructor
	HWND MainWnd;
	DWORD MainPID;
	void AddLine(CString InStr);
	afx_msg void OnNotifyIcon(WPARAM wParam, LPARAM lParam); 
// Dialog Data
	//{{AFX_DATA(CLiveUpdateDlg)
	enum { IDD = IDD_LIVEUPDATE_DIALOG };
	CEdit	m_memo;
	CProgressCtrl	m_Progress;
	CStatic	m_info;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLiveUpdateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLiveUpdateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIVEUPDATEDLG_H__747FB09A_7CF2_4E19_A453_428EAA06D8F3__INCLUDED_)
