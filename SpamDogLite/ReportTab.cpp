// ReportTab.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "ReportTab.h"

#include "MailReportFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportTab

CReportTab::CReportTab()
{
	memset(&m_tabPages,0,sizeof(m_tabPages));

	m_tabPages[0]  = new CMailReportFrm(); 
	m_tabPages[1]  = new CMailReportFrm(); 

	m_nNumberOfPages = sizeof(m_tabPages) / sizeof(m_tabPages[0]);
}

CReportTab::~CReportTab()
{
}


BEGIN_MESSAGE_MAP(CReportTab, CTabCtrl)
	//{{AFX_MSG_MAP(CReportTab)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CReportTab::JoinTabDialog()
{

	m_tabCurrent = 0;

	m_tabPages[0]->Create(IDD_MAILREPORTFRM_DIALOG,this);
	m_tabPages[0]->ShowWindow(SW_SHOW);

	m_tabPages[1]->Create(IDD_MAILREPORTFRM_DIALOG,this);
	m_tabPages[1]->ShowWindow(SW_HIDE);

	 

	SetRectangle();


}

void CReportTab::SetRectangle()
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

void CReportTab::SetActivePage(int nindex)
{

	if (nindex < m_nNumberOfPages)
	{
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=nindex;
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();

		this->SetCurSel(nindex);
		this->SetCurFocus(nindex);
	}

}
/*
void CReportTab::OnLButtonDown(UINT nFlags, CPoint point) 
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
*/

/////////////////////////////////////////////////////////////////////////////
// CReportTab message handlers

void CReportTab::OnLButtonDown(UINT nFlags, CPoint point) 
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


