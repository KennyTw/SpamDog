#if !defined(AFX_LIVEUPDATE_H__285DC83D_3B62_4A15_AF01_03F268990E20__INCLUDED_)
#define AFX_LIVEUPDATE_H__285DC83D_3B62_4A15_AF01_03F268990E20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LiveUpdate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLiveUpdate dialog

class CLiveUpdate : public CDialog
{
// Construction
private:
	void GetIni();
	 

public:
	CLiveUpdate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLiveUpdate)
	enum { IDD = IDD_LIVEUPDATE_DIALOG };
	CStatic	m_DbCutValue;
	CSliderCtrl	m_DbCut;
	//CButton	m_AutoUpdate;
	//CButton	m_AutoBackup;
	//CEdit	m_FilePath;
	CStatic	m_status;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLiveUpdate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLiveUpdate)
	virtual BOOL OnInitDialog();
	//afx_msg void OnButton1();
	afx_msg void OnButton7();
	afx_msg void OnButton4();
	//afx_msg void OnButton6();
	//afx_msg void OnButton5();
	afx_msg void OnCustomdrawSliderCutdb(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIVEUPDATE_H__285DC83D_3B62_4A15_AF01_03F268990E20__INCLUDED_)
