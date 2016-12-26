#if !defined(AFX_OTHERSETUPFRM_H__2C1E639D_9A8E_48FE_9686_8258972778BC__INCLUDED_)
#define AFX_OTHERSETUPFRM_H__2C1E639D_9A8E_48FE_9686_8258972778BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OtherSetupFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COtherSetupFrm dialog

class COtherSetupFrm : public CDialog
{
// Construction
public:
	COtherSetupFrm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COtherSetupFrm)
	enum { IDD = IDD_OTHERSETUPFRM_DIALOG };
	CButton	m_SysAutoDel;
	CButton	m_AutoDel;
	CEdit	m_SysDelDay;
	CEdit	m_ArrangeFolder;
	CButton	m_StartWithAP;
	CComboBox	m_ApVersion;
	CButton	m_BootStart;
	CComboBox	m_ProfList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COtherSetupFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void GetIni();

protected:

	// Generated message map functions
	//{{AFX_MSG(COtherSetupFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OTHERSETUPFRM_H__2C1E639D_9A8E_48FE_9686_8258972778BC__INCLUDED_)
