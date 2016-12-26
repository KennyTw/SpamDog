// WizLearnTest.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "WizLearnTest.h"
#include "WizSheet.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_OLNOTIFY WM_USER+1 //OutLook Event to Form

/////////////////////////////////////////////////////////////////////////////
// CWizLearnTest property page

IMPLEMENT_DYNCREATE(CWizLearnTest, CPropertyPageEx)


void __stdcall MailOlNotify(LPVOID lpParam ,ULONG ulTableEvent,OlREGISTERNOTIFY* lpRegisterNotify ,OlMAILPROPS *mailpros)
{

	CWizLearnTest *WizFrm = (CWizLearnTest *) lpParam;

	
	
	if (ulTableEvent == TABLE_ROW_ADDED)
	{
		OutputDebugString("Notify TABLE_ROW_ADDED\n");
	}
	else if(ulTableEvent == TABLE_ROW_DELETED )
	{
	    OutputDebugString("Notify TABLE_ROW_DELETED\n");
	}

	OlMoveMail(mailpros,lpRegisterNotify->RegisterFolder,WizFrm->lpSpamFolder);

	//OlUnRegFolderNotify(&WizFrm->olNotify);
	//memset(&WizFrm->olNotify,0,sizeof(WizFrm->olNotify));
	//OlCloseFolder(WizFrm->lpTestFolder);
	//WizFrm->lpTestFolder = NULL;

	//((CWizSheet *) WizFrm->GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	//WizFrm->m_MainText.SetWindowText("測試成功!");
	//OlLogout();

	PostMessage(WizFrm->m_hWnd,WM_OLNOTIFY,1,ulTableEvent);
	
     

}

void __stdcall MailOeNotify(LPVOID lpParam ,ULONG ulTableEvent, OeMailObj* MailObj)
{

	CWizLearnTest *WizFrm = (CWizLearnTest *) lpParam;

	

	if (ulTableEvent == WM_NEWMSGS)
	{
		OutputDebugString("Notify WM_NEWMSGS\n");
	}
	else if(ulTableEvent == WM_DELETEMSGS )
	{
	    OutputDebugString("Notify WM_DELETEMSGS\n");
	}
     
	OeMoveMail(MailObj,WizFrm->m_pSpamBoxStoreFolder);
	
	//OeUnRegFolderNotify(&WizFrm->oeNotify);
	//memset(&WizFrm->olNotify,0,sizeof(WizFrm->oeNotify));

 
	
	//couldn't close folder when Notify In
	//OeCloseFolder(WizFrm->m_pTestFolder);
	//WizFrm->m_pTestFolder = NULL;

	//((CWizSheet *) WizFrm->GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);	
	//WizFrm->m_MainText.SetWindowText("測試成功!");

	PostMessage(WizFrm->m_hWnd,WM_OLNOTIFY,2,ulTableEvent);


	//OeLogout();

}

void CWizLearnTest::ApRelease()
{
	if (OutlookType == 1) //OutLook
	{
		if (olNotify.lpContentsTable != NULL)
		{
			OlUnRegFolderNotify(&olNotify);
			memset(&olNotify,0,sizeof(olNotify));
		}
	
	}
	else if (OutlookType == 2) //OutLook Express
	{
		if (oeNotify.RegisterFolder != NULL)
		{
			OeUnRegFolderNotify(&oeNotify);
			memset(&oeNotify,0,sizeof(oeNotify));
		}

	
	}



	if (lpLearnSpamFolder != NULL)
	{
		OlCloseFolder(lpLearnSpamFolder);
	}

	if (m_pLearnSpamBoxStoreFolder != NULL)
	{
		OeCloseFolder(m_pLearnSpamBoxStoreFolder);
	}

	if (lpSpamFolder != NULL)
	{
		OlCloseFolder(lpSpamFolder);
	}

	if (m_pSpamBoxStoreFolder != NULL)
	{
		OeCloseFolder(m_pSpamBoxStoreFolder);
	}

	if (bLogin == true)
	{
		if (OutlookType == 1) //OutLook
		{
			
			OlLogout();
			bLogin = false;
			
		}
		else if (OutlookType == 2) //OutLook Express
		{
		
			
			OeLogout();
			bLogin = false;
		}

	}
	 

	lpSpamFolder = NULL; 
	lpLearnOkFolder = NULL;
	lpLearnSpamFolder = NULL;
	lpUnknownFolder = NULL;
	lpWhiteFolder = NULL;
	lpSysMsgFolder = NULL;
	//lpTestFolder = NULL;

       
	m_pUnknownBoxStoreFolder = NULL;   
	m_pSpamBoxStoreFolder = NULL;      
	m_pLearnOkBoxStoreFolder = NULL;   
	m_pLearnSpamBoxStoreFolder = NULL; 
	m_pLearnWhiteBoxStoreFolder = NULL;
	m_pSysMsgStoreFolder = NULL;   
	//m_pTestFolder = NULL;

	memset(&oeNotify,0,sizeof(oeNotify));
	memset(&olNotify,0,sizeof(olNotify));
    
	OutlookType = 0;
}

CWizLearnTest::CWizLearnTest() : CPropertyPageEx(CWizLearnTest::IDD)
{
	//{{AFX_DATA_INIT(CWizLearnTest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
		m_psp.dwFlags &= ~PSP_HASHELP;

	lpSpamFolder = NULL; 
	lpLearnOkFolder = NULL;
	lpLearnSpamFolder = NULL;
	lpUnknownFolder = NULL;
	lpWhiteFolder = NULL;
	lpSysMsgFolder = NULL;
	//lpTestFolder = NULL;

       
	m_pUnknownBoxStoreFolder = NULL;   
	m_pSpamBoxStoreFolder = NULL;      
	m_pLearnOkBoxStoreFolder = NULL;   
	m_pLearnSpamBoxStoreFolder = NULL; 
	m_pLearnWhiteBoxStoreFolder = NULL;
	m_pSysMsgStoreFolder = NULL;   
	//m_pTestFolder = NULL;

	memset(&oeNotify,0,sizeof(oeNotify));
	memset(&olNotify,0,sizeof(olNotify));
    
	OutlookType = 0;
	bLogin = false;

	m_strHeaderTitle = _T("SpamDog 學習測試");
	m_strHeaderSubTitle = _T("請開啟設定的郵件程式 , 並進行測試確認"); 



}

CWizLearnTest::~CWizLearnTest()
{
	
	if (OutlookType == 1) //OutLook
	{
		if (olNotify.lpContentsTable != NULL)
		{
			OlUnRegFolderNotify(&olNotify);
			memset(&olNotify,0,sizeof(olNotify));
		}
	
	}
	else if (OutlookType == 2) //OutLook Express
	{
		if (oeNotify.RegisterFolder != NULL)
		{
			OeUnRegFolderNotify(&oeNotify);
			memset(&oeNotify,0,sizeof(oeNotify));
		}

	
	}
	
	if (lpLearnSpamFolder != NULL)
	{
		OlCloseFolder(lpLearnSpamFolder);
	}

	if (m_pLearnSpamBoxStoreFolder != NULL)
	{
		OeCloseFolder(m_pLearnSpamBoxStoreFolder);
	}

	if (lpSpamFolder != NULL)
	{
		OlCloseFolder(lpSpamFolder);
	}

	if (m_pSpamBoxStoreFolder != NULL)
	{
		OeCloseFolder(m_pSpamBoxStoreFolder);
	}

	if (bLogin)
	{
	 
		if (OutlookType == 1) //OutLook
		{
			OlLogout();
		}
		else if (OutlookType == 2) //OutLook
		{
			OeLogout();
		}

	}
 
}

void CWizLearnTest::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizLearnTest)
	DDX_Control(pDX, IDC_STATIC_TEXT, m_MainText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizLearnTest, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizLearnTest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_MESSAGE(WM_OLNOTIFY,OnOlNotify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizLearnTest message handlers

BOOL CWizLearnTest::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	

	CWizSheet* pSheet = (CWizSheet*)GetParent();
 
	pSheet->SetWizardButtons(0);
	m_MainText.SetWindowText("聯繫郵件軟體中");
	//pSheet->SetWizardButtons(PSWIZB_BACK);
	

	 


	SetTimer(1,0,NULL);
	
	
	return CPropertyPageEx::OnSetActive();
}

void CWizLearnTest::OnOlNotify(WPARAM wParam, LPARAM lParam)
{
	
	CWizSheet* pSheet = (CWizSheet*)GetParent();

	if (wParam == 1) //Outlook
	{

		if (olNotify.lpContentsTable != NULL)
		{
			OlUnRegFolderNotify(&olNotify);
		    memset(&olNotify,0,sizeof(olNotify));

	        pSheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

			OlCloseFolder(lpLearnSpamFolder);
			lpLearnSpamFolder = NULL;

			OlCloseFolder(lpSpamFolder);
			lpSpamFolder = NULL;

			OlLogout();
				
			bLogin = false;

			m_MainText.SetWindowText("測試成功! SpamDog 已將信件測試學習為垃圾信件,並移往垃圾信件匣");
		}
	
	
	} else if (wParam == 2) //Outlook Express
	{
			
			if (oeNotify.RegisterFolder != NULL)
			{
				OeUnRegFolderNotify(&oeNotify);
				memset(&oeNotify,0,sizeof(oeNotify));
		
		
				OeCloseFolder(m_pLearnSpamBoxStoreFolder);
				m_pLearnSpamBoxStoreFolder = NULL;

				OeCloseFolder(m_pSpamBoxStoreFolder);
				m_pSpamBoxStoreFolder = NULL;

				OeLogout();
				bLogin = false;

				pSheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
				m_MainText.SetWindowText("測試成功! SpamDog 已將信件測試學習為垃圾信件,並移往垃圾信件匣");
			}
	
	}

	 MessageBeep(MB_OK);
	 SetForegroundWindow();
}

void CWizLearnTest::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);

	CWizSheet* pSheet = (CWizSheet*)GetParent();
	HRESULT hr = S_FALSE;

	OutlookType = pSheet->OutlookType;
	if (pSheet->OutlookType == 1) //OutLook
	{
	
		hr = OlLogin(pSheet->OutlookProf.GetBuffer(pSheet->OutlookProf.GetLength()));

		if (hr == S_OK)
		{
			bLogin = true;
			LPMAPIFOLDER InBoxFolder;
			//LPMAPIFOLDER CreateFolder;
			if (OlOpenInBoxFolder(&InBoxFolder) == S_OK)
			{
				if (OlCreateFolder(InBoxFolder,"[垃圾信件匣]","垃圾信件將放於此處",&lpSpamFolder) == S_OK)
				{
					//OlCloseFolder(lpSpamFolder);				
				}

				if (OlCreateFolder(InBoxFolder,"[無法辨識信件匣]","無法辨識信件將放於此處,請拖拉到學習信件匣",&lpUnknownFolder) == S_OK)
				{
					OlCloseFolder(lpUnknownFolder);				
				}

				if (OlCreateFolder(InBoxFolder,"[學習正常信件匣]","拖拉信件至此,學習為正常信件",&lpLearnOkFolder) == S_OK)
				{
					OlCloseFolder(lpLearnOkFolder);				
				}

				if (OlCreateFolder(InBoxFolder,"[學習白名單信件匣]","拖拉信件至此,加入白名單",&lpWhiteFolder) == S_OK)
				{
					OlCloseFolder(lpWhiteFolder);				
				}

				if (OlCreateFolder(InBoxFolder,"[學習垃圾信件匣]","拖拉信件至此,學習為垃圾信件",&lpLearnSpamFolder) == S_OK)
				{
					//OlCloseFolder(lpLearnSpamFolder);				
					//lpLearnSpamFolder = NULL;
					memset(&olNotify,0,sizeof(olNotify));
					olNotify.RegType = TABLE_ROW_ADDED ;//| TABLE_ROW_DELETED;
					olNotify.RegisterFolder = lpLearnSpamFolder;

					OlRegFolderNotify((LPVOID)this,&olNotify,&MailOlNotify);
				}

			
#ifdef SPAMDOGFULL
				if (OlCreateFolder(InBoxFolder,"[系統訊息]","系統訊息",&lpSysMsgFolder) == S_OK)
				{
					OlCloseFolder(lpSysMsgFolder);				
				}
#endif

				//if (OlCreateFolder(InBoxFolder,"[Test]","test",&lpTestFolder) == S_OK)
				//{
					//OlCloseFolder(lpTestFolder);
					
				
				 

					//OlUnRegFolderNotify(&olNotify);

					

				//}

				


				OlCloseFolder(InBoxFolder);

				m_MainText.SetWindowText("Outlook 請拖拉信件到 [學習垃圾信件匣] 進行學習測試!");
			
			}

		
		}
		
		
		

		pSheet->OutlookProf.ReleaseBuffer();
	}
	else if (pSheet->OutlookType == 2) //OutLook Express
	{
	
	 
		hr =  OeLogin();

		if (hr == S_OK)
		{		 
			IStoreFolder *InBoxFolder = NULL;
			hr = OeOpenInBoxFolder(&InBoxFolder);
			if (hr == S_OK)
			{
			    bLogin = true;
				//hr = OeCreateFolder(InBoxFolder,"[test]",&m_pTestFolder);
				//if (hr == S_OK)
				//{
					//OeCloseFolder(m_pTestFolder);
				//}

				hr = OeCreateFolder(InBoxFolder,"[垃圾信件匣]",&m_pSpamBoxStoreFolder);
				if (hr == S_OK)
				{
					//OeCloseFolder(m_pSpamBoxStoreFolder);
				}

				hr = OeCreateFolder(InBoxFolder,"[無法辨識信件匣]",&m_pUnknownBoxStoreFolder);
				if (hr == S_OK)
				{
					OeCloseFolder(m_pUnknownBoxStoreFolder);
				}

				hr = OeCreateFolder(InBoxFolder,"[學習正常信件匣]",&m_pLearnOkBoxStoreFolder);
				if (hr == S_OK)
				{
					OeCloseFolder(m_pLearnOkBoxStoreFolder);
				}

				hr = OeCreateFolder(InBoxFolder,"[學習垃圾信件匣]",&m_pLearnSpamBoxStoreFolder);
				if (hr == S_OK)
				{
					//OeCloseFolder(m_pLearnSpamBoxStoreFolder);
						memset(&oeNotify,0,sizeof(oeNotify));
						oeNotify.RegType = WM_NEWMSGS ;//| WM_DELETEMSGS;				
				
						OeRegFolderNotify((LPVOID)this,m_pLearnSpamBoxStoreFolder,&oeNotify,&MailOeNotify);
				}

				hr = OeCreateFolder(InBoxFolder,"[學習白名單信件匣]",&m_pLearnWhiteBoxStoreFolder);
				if (hr == S_OK)
				{
					OeCloseFolder(m_pLearnWhiteBoxStoreFolder);
				}

				#ifdef SPAMDOGFULL

				hr = OeCreateFolder(InBoxFolder,"[系統訊息]",&m_pSysMsgStoreFolder);
				if (hr == S_OK)
				{
					OeCloseFolder(m_pSysMsgStoreFolder);
				}

				#endif

				
			

				

				OeCloseFolder(InBoxFolder);

				m_MainText.SetWindowText("Outlook Express 請拖拉信件到 [學習垃圾信件匣] 進行學習測試!");

			}

			
		}
	}

	
	pSheet->SetWizardButtons(PSWIZB_BACK);

	if (hr != S_OK)
	{
	
	    if (pSheet->OutlookType == 2) //OutLook Express
		{
			AfxMessageBox("很報歉 ! SpamDog 在聯繫Outlook Express時發生錯誤!\n請重新開機再重覆執行一次");
		}
		else
		{
			AfxMessageBox("很報歉 ! SpamDog 在聯繫Outlook時發生錯誤!\n請確定您 IE 的[網際網路選項][程式集][電子郵件位址]有指定到 Outlook 或請重新開機再重覆執行一次");	
		}

		m_MainText.SetWindowText("聯繫郵件軟體失敗");
	
	}

	CPropertyPageEx::OnTimer(nIDEvent);
}

LRESULT CWizLearnTest::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	//ApRelease();

	return CPropertyPageEx::OnWizardNext();
}

void CWizLearnTest::OnButton1() 
{
	// TODO: Add your control notification handler code here
	 
}

LRESULT CWizLearnTest::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
	//this->~CWizLearnTest();
    ApRelease();

	return CPropertyPageEx::OnWizardBack();
}

BOOL CWizLearnTest::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	//m_MainText.SetWindowText("");	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
