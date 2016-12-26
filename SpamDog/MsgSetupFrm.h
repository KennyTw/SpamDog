#if !defined(AFX_MSGSETUPFRM_H__9398C3F5_C8EE_437A_A25F_AFE32336DE5D__INCLUDED_)
#define AFX_MSGSETUPFRM_H__9398C3F5_C8EE_437A_A25F_AFE32336DE5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgSetupFrm.h : header file
//
#include "../UI/ColorBox.h"

/////////////////////////////////////////////////////////////////////////////
// CMsgSetupFrm dialog

class CMsgSetupFrm : public CDialog
{
// Construction
public:
	CMsgSetupFrm(CWnd* pParent = NULL);   // standard constructor

	COLORREF SysOkMailColor,SpamMailColor,UnknownColor,VirusColor,TextColor; 

 

// Dialog Data
	//{{AFX_DATA(CMsgSetupFrm)
	enum { IDD = IDD_MSGSETUPFRM_DIALOG };
	CButton	m_ShowMsg;
	CColorBox	m_ColorBoxXX;
	CColorBox	m_ColorBoxVR;
	CColorBox	m_ColorText;
	CColorBox	m_ColorBoxSPAM;
	CColorBox	m_ColorBoxOK;
	CStatic	m_SpeedValue;
	CStatic	m_DelayValue;
	CButton	m_ShowTrashMsg;
	CSliderCtrl	m_MsgSpeed;
	CSliderCtrl	m_MsgDelayTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgSetupFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void GetIniColor();
	void GetIni();

protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgSetupFrm)
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawSliderMsgdelay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderMsgspeed(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGSETUPFRM_H__9398C3F5_C8EE_437A_A25F_AFE32336DE5D__INCLUDED_)
