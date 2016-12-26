// WebFrm.cpp : implementation file
//

#include "stdafx.h"
#include "spamdog.h"
#include "WebFrm.h"
#include <Mshtml.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebFrm dialog


CWebFrm::CWebFrm(CWnd* pParent /*=NULL*/)
	: CDialog(CWebFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebFrm)
	//}}AFX_DATA_INIT
}


void CWebFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebFrm)
	DDX_Control(pDX, IDC_EXPLORER1, m_Web);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebFrm, CDialog)
	//{{AFX_MSG_MAP(CWebFrm)
	 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebFrm message handlers

BOOL CWebFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CenterWindow(GetDesktopWindow());
	ShowWindow(SW_HIDE);
	

	//m_Web.Navigate("about:blank",NULL,NULL,NULL,NULL);
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

  
BEGIN_EVENTSINK_MAP(CWebFrm, CDialog)
    //{{AFX_EVENTSINK_MAP(CWebFrm)
	ON_EVENT(CWebFrm, IDC_EXPLORER1, 259 /* DocumentComplete */, OnDocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CWebFrm::OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	// TODO: Add your control notification handler code here

	HRESULT hr = S_OK;
	IWebBrowser2* pWB = NULL;

	hr = pDisp->QueryInterface(IID_IWebBrowser2, reinterpret_cast<void**>(&pWB));

	IDispatch* pDocDisp = NULL;
	hr = pWB->get_Document(&pDocDisp);

	if (pDocDisp)
	{
		VARIANT v;
		VariantInit(&v);

		IHTMLDocument2* pDoc = NULL;
		hr = pDocDisp->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<void **>(&pDoc));

		IHTMLElement* pElement = NULL;
		hr = pDoc->get_body(&pElement);

		IHTMLBodyElement* pBodyElement = NULL;
		hr = pElement->QueryInterface(IID_IHTMLBodyElement, (void**)&pBodyElement);

		if (pBodyElement)
		{
			pBodyElement->put_scroll(::SysAllocString(L"no"));
			pBodyElement->Release();
		}
		pElement->Release();
		pDoc->Release();
		pDocDisp->Release();
	}
	pWB->Release();


	ShowWindow(SW_SHOW);
	
}
