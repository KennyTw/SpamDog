// SetupTab.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "SetupTab.h"


#include "RuleSetupFrm.h"
#include "MsgSetupFrm.h"
#include "ScanSetupFrm.h"
#include "OtherSetupFrm.h"
#include "LiveUpdate.h"
#include "AboutThis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupTab

CSetupTab::CSetupTab()
{
	memset(&m_tabPages,0,sizeof(m_tabPages));

	m_tabPages[0]  = new CRuleSetupFrm(); 
	m_tabPages[1]  = new CMsgSetupFrm();
	m_tabPages[2]  = new CScanSetupFrm();
	m_tabPages[3]  = new CLiveUpdate();
	m_tabPages[4]  = new COtherSetupFrm();
	m_tabPages[5]  = new CAboutThis();


	m_nNumberOfPages = sizeof(m_tabPages) / sizeof(m_tabPages[0]);
}

CSetupTab::~CSetupTab()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++)
	{
		delete m_tabPages[nCount];
	}
}


BEGIN_MESSAGE_MAP(CSetupTab, CTabCtrl)
	//{{AFX_MSG_MAP(CSetupTab)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupTab message handlers
void CSetupTab::JoinTabDialog()
{

	m_tabCurrent = 0;

	m_tabPages[0]->Create(IDD_RULESETUPFRM_DIALOG,this);
	m_tabPages[0]->ShowWindow(SW_SHOW);

	m_tabPages[1]->Create(IDD_MSGSETUPFRM_DIALOG,this);
	m_tabPages[1]->ShowWindow(SW_HIDE);

	m_tabPages[2]->Create(IDD_SCANSETUPFRM_DIALOG,this);
	m_tabPages[2]->ShowWindow(SW_HIDE);

	m_tabPages[3]->Create(IDD_LIVEUPDATE_DIALOG,this);
	m_tabPages[3]->ShowWindow(SW_HIDE);

	m_tabPages[4]->Create(IDD_OTHERSETUPFRM_DIALOG,this);
	m_tabPages[4]->ShowWindow(SW_HIDE);

	m_tabPages[5]->Create(IDD_ABOUTTHIS_DIALOG,this);
	m_tabPages[5]->ShowWindow(SW_HIDE);

	SetRectangle();


}

void CSetupTab::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	//setupfrm->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);	 
	//syssetupfrm->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	//setupfrm->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++)
	{
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
	 
}

void CSetupTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurFocus() && GetCurFocus() < m_nNumberOfPages ){

		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}

}
