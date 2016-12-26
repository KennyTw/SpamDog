#if !defined(AFX_SCANSETUPFRM_H__EE41CDA6_AAB0_4E97_BA6E_DC660F5BBEED__INCLUDED_)
#define AFX_SCANSETUPFRM_H__EE41CDA6_AAB0_4E97_BA6E_DC660F5BBEED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanSetupFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScanSetupFrm dialog

class CScanSetupFrm : public CDialog
{
// Construction
public:
	CScanSetupFrm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScanSetupFrm)
	enum { IDD = IDD_SCANSETUPFRM_DIALOG };
	CStatic	m_AutoShowReportText;
	CEdit	m_AutoShowReportFreq;
	CButton	m_AutoShowReport;
	CButton	m_AutoDel;
	CStatic	m_MaxAttSizeValue;
	CSliderCtrl	m_MaxAttSize;
	CButton	m_NeedReport;
	CButton	m_IpStrIdent;
	CButton	m_IpIdent;
	CEdit	m_Rbl2;
	CEdit	m_Rbl1;
	CEdit	m_IpKey;
	CEdit	m_DNS;
	CButton	m_AuthEx;
	CEdit	m_TracerExpire;
	CButton	m_MailTracer;
	CStatic	m_UnknownValue;
	CButton	m_NullSender;
	CButton	m_NullSubject;
	CButton	m_SpamAddress;
	CButton	m_SpamSubject;
	CSliderCtrl	m_UnknownRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanSetupFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void GetIni();

protected:

	// Generated message map functions
	//{{AFX_MSG(CScanSetupFrm)
	afx_msg void OnCustomdrawSliderUnknownrate(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnIpIdentCheck();
	afx_msg void OnCustomdrawSliderMaxattsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckNeedreport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANSETUPFRM_H__EE41CDA6_AAB0_4E97_BA6E_DC660F5BBEED__INCLUDED_)
