#if !defined(AFX_FLATBUTTON_H__B80BD7CC_C5DF_4DE5_9F6A_DC0279DCC573__INCLUDED_)
#define AFX_FLATBUTTON_H__B80BD7CC_C5DF_4DE5_9F6A_DC0279DCC573__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlatButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlatButton window

class CFlatButton : public CButton
{
// Construction
public:
	CFlatButton();
	void SetBitMap(int MapResId);
	void SetButtonColor(COLORREF TextColor,	
						COLORREF BorderColor,
						COLORREF TextBgColor,
						COLORREF HoverTextColor,
						COLORREF HoverBgColor,
						COLORREF PressTextColor,
						COLORREF PressBgColor);

	void SetButtonTextColor(COLORREF TextColor,
							COLORREF HoverTextColor,
							COLORREF PressTextColor);

	void SetButtonBgColor(COLORREF TextBgColor,
						  COLORREF HoverBgColor,
						  COLORREF PressBgColor);

	void SetButtonBorderColor(COLORREF BorderColor);
	void SetNeedText(bool needtext);
	void SetRepaintParent(bool repaint);
	




// Attributes
private:
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	bool m_bMouseOnButton;
	bool m_bIsPressed;
	bool m_bneedtext;
	bool m_bRepaintParent;

	HBITMAP		BitmapIn;
	HBITMAP		hMask;

	COLORREF mTextColor;
	COLORREF mBorderColor;
	COLORREF mTextBgColor;
	COLORREF mHoverTextColor;
	COLORREF mHoverBgColor;
	COLORREF mPressTextColor;
	COLORREF mPressBgColor;

	int SaveIndex; 

 

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlatButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlatButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATBUTTON_H__B80BD7CC_C5DF_4DE5_9F6A_DC0279DCC573__INCLUDED_)
