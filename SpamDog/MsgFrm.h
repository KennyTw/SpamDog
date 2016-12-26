#if !defined(AFX_MSGFRM_H__B46E2DDA_6D45_41AA_A15B_1519EA10A325__INCLUDED_)
#define AFX_MSGFRM_H__B46E2DDA_6D45_41AA_A15B_1519EA10A325__INCLUDED_
#pragma warning (disable:4786)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgFrm.h : header file
//
#include "../UI/FlatButton.h"
#include "../UI/WPStatic.h"
#include "SpamDog.h"
#include <list>

typedef struct _MsgObj 
{
  char MsgStr[255];
  COLORREF BgColor;
  COLORREF TextColor;  
  CTime MsgTime;
} MsgObj;

typedef  std::list<MsgObj> MsgQueue;

/////////////////////////////////////////////////////////////////////////////
// CMsgFrm dialog
typedef  std::list<int> MsgObjList;

class CMsgFrm : public CDialog
{
// Construction
private:
	CWPStatic m_MsgText;
	HICON hicon;

	//StrList MsgQueue;
	MsgQueue m_MsgQueue;

	//CBrush* m_pEditBkBrush;
	COLORREF mBgColor;
	COLORREF mTextColor;
	HMODULE hUser32 ;

	int LayerLevel;
	bool capmouse;
	bool msgenable ;
	long InProcess;
	char m_MsgType ;

	MsgObjList m_MsgObjList;
	CWnd* m_pParent;

	CFlatButton m_CloseBtn;

	void ShowQueueMsg();
public:
	CMsgFrm(CWnd* pParent = NULL,char MsgType=1);   // standard constructor

	//void SetEnable(bool enable);

	void ShowMsg(COLORREF BgColor,COLORREF TextColor,char *MsgStr );
	void ShowNewMsg(COLORREF BgColor,COLORREF TextColor,char *MsgStr);
	void SetNewContent(COLORREF BgColor,COLORREF TextColor,char *MsgStr);
	void CloseAlert(CWnd* obj);

// Dialog Data
	//{{AFX_DATA(CMsgFrm)
	enum { IDD = IDD_MSGFRM_DIALOG };
	CStatic	m_MsgTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgFrm)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGFRM_H__B46E2DDA_6D45_41AA_A15B_1519EA10A325__INCLUDED_)
