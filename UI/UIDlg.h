// UIDlg.h : header file
//

#if !defined(AFX_UIDLG_H__D666A40C_3A3C_4ED4_A1C0_28D98CF4DF95__INCLUDED_)
#define AFX_UIDLG_H__D666A40C_3A3C_4ED4_A1C0_28D98CF4DF95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorBox.h"
#include "FlatButton.h"
#include "WPStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CUIDlg dialog

class CUIDlg : public CDialog
{
// Construction
public:
	CUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUIDlg)
	enum { IDD = IDD_UI_DIALOG };
	CWPStatic	m_static1;
	CFlatButton	m_btn;
	CColorBox	m_colorbox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL


private:
	CBrush* m_pEditBkBrush; 

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UIDLG_H__D666A40C_3A3C_4ED4_A1C0_28D98CF4DF95__INCLUDED_)
