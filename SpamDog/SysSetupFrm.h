#if !defined(AFX_SYSSETUPFRM_H__981C3CBE_F8DE_46F6_B12A_9AAE8C5822E7__INCLUDED_)
#define AFX_SYSSETUPFRM_H__981C3CBE_F8DE_46F6_B12A_9AAE8C5822E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSetupFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysSetupFrm dialog

class CSysSetupFrm : public CDialog
{
// Construction
public:
	CSysSetupFrm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysSetupFrm)
	enum { IDD = IDD_SYSSETUPFRM_DIALOG };
	CComboBox	m_ApVersion;
	CStatic	m_UnknownValue;
	CStatic	m_SpeedValue;
	CStatic	m_DelayValue;
	CButton	m_ShowTrashMsg;
	CButton	m_BootStart;
	CButton	m_NullContent;
	CButton	m_NullSender;
	CButton	m_NullSubject;
	CButton	m_SpamAddress;
	CButton	m_SpamSubject;
	CSliderCtrl	m_UnknownRate;
	CSliderCtrl	m_MsgSpeed;
	CSliderCtrl	m_MsgDelayTime;
	CComboBox	m_ProfList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysSetupFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void GetIni();
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysSetupFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawSliderMsgdelay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderMsgspeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderUnknownrate(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETUPFRM_H__981C3CBE_F8DE_46F6_B12A_9AAE8C5822E7__INCLUDED_)
