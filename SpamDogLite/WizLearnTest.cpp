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

	//WizFrm->m_MainText.SetWindowText("���զ��\!");
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
	//WizFrm->m_MainText.SetWindowText("���զ��\!");

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

	m_strHeaderTitle = _T("SpamDog �ǲߴ���");
	m_strHeaderSubTitle = _T("�ж}�ҳ]�w���l��{�� , �öi����սT�{"); 



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
	m_MainText.SetWindowText("�pô�l��n�餤");
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

			m_MainText.SetWindowText("���զ��\! SpamDog �w�N�H����վǲ߬��U���H��,�ò����U���H��X");
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
				m_MainText.SetWindowText("���զ��\! SpamDog �w�N�H����վǲ߬��U���H��,�ò����U���H��X");
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
				if (OlCreateFolder(InBoxFolder,"[�U���H��X]","�U���H��N��󦹳B",&lpSpamFolder) == S_OK)
				{
					//OlCloseFolder(lpSpamFolder);				
				}

				if (OlCreateFolder(InBoxFolder,"[�L�k���ѫH��X]","�L�k���ѫH��N��󦹳B,�Щ�Ԩ�ǲ߫H��X",&lpUnknownFolder) == S_OK)
				{
					OlCloseFolder(lpUnknownFolder);				
				}

				if (OlCreateFolder(InBoxFolder,"[�ǲߥ��`�H��X]","��ԫH��ܦ�,�ǲ߬����`�H��",&lpLearnOkFolder) == S_OK)
				{
					OlCloseFolder(lpLearnOkFolder);				
				}

				if (OlCreateFolder(InBoxFolder,"[�ǲߥզW��H��X]","��ԫH��ܦ�,�[�J�զW��",&lpWhiteFolder) == S_OK)
				{
					OlCloseFolder(lpWhiteFolder);				
				}

				if (OlCreateFolder(InBoxFolder,"[�ǲߩU���H��X]","��ԫH��ܦ�,�ǲ߬��U���H��",&lpLearnSpamFolder) == S_OK)
				{
					//OlCloseFolder(lpLearnSpamFolder);				
					//lpLearnSpamFolder = NULL;
					memset(&olNotify,0,sizeof(olNotify));
					olNotify.RegType = TABLE_ROW_ADDED ;//| TABLE_ROW_DELETED;
					olNotify.RegisterFolder = lpLearnSpamFolder;

					OlRegFolderNotify((LPVOID)this,&olNotify,&MailOlNotify);
				}

			
#ifdef SPAMDOGFULL
				if (OlCreateFolder(InBoxFolder,"[�t�ΰT��]","�t�ΰT��",&lpSysMsgFolder) == S_OK)
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

				m_MainText.SetWindowText("Outlook �Щ�ԫH��� [�ǲߩU���H��X] �i��ǲߴ���!");
			
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

				hr = OeCreateFolder(InBoxFolder,"[�U���H��X]",&m_pSpamBoxStoreFolder);
				if (hr == S_OK)
				{
					//OeCloseFolder(m_pSpamBoxStoreFolder);
				}

				hr = OeCreateFolder(InBoxFolder,"[�L�k���ѫH��X]",&m_pUnknownBoxStoreFolder);
				if (hr == S_OK)
				{
					OeCloseFolder(m_pUnknownBoxStoreFolder);
				}

				hr = OeCreateFolder(InBoxFolder,"[�ǲߥ��`�H��X]",&m_pLearnOkBoxStoreFolder);
				if (hr == S_OK)
				{
					OeCloseFolder(m_pLearnOkBoxStoreFolder);
				}

				hr = OeCreateFolder(InBoxFolder,"[�ǲߩU���H��X]",&m_pLearnSpamBoxStoreFolder);
				if (hr == S_OK)
				{
					//OeCloseFolder(m_pLearnSpamBoxStoreFolder);
						memset(&oeNotify,0,sizeof(oeNotify));
						oeNotify.RegType = WM_NEWMSGS ;//| WM_DELETEMSGS;				
				
						OeRegFolderNotify((LPVOID)this,m_pLearnSpamBoxStoreFolder,&oeNotify,&MailOeNotify);
				}

				hr = OeCreateFolder(InBoxFolder,"[�ǲߥզW��H��X]",&m_pLearnWhiteBoxStoreFolder);
				if (hr == S_OK)
				{
					OeCloseFolder(m_pLearnWhiteBoxStoreFolder);
				}

				#ifdef SPAMDOGFULL

				hr = OeCreateFolder(InBoxFolder,"[�t�ΰT��]",&m_pSysMsgStoreFolder);
				if (hr == S_OK)
				{
					OeCloseFolder(m_pSysMsgStoreFolder);
				}

				#endif

				
			

				

				OeCloseFolder(InBoxFolder);

				m_MainText.SetWindowText("Outlook Express �Щ�ԫH��� [�ǲߩU���H��X] �i��ǲߴ���!");

			}

			
		}
	}

	
	pSheet->SetWizardButtons(PSWIZB_BACK);

	if (hr != S_OK)
	{
	
	    if (pSheet->OutlookType == 2) //OutLook Express
		{
			AfxMessageBox("�ܳ��p ! SpamDog �b�pôOutlook Express�ɵo�Ϳ��~!\n�Э��s�}���A���а���@��");
		}
		else
		{
			AfxMessageBox("�ܳ��p ! SpamDog �b�pôOutlook�ɵo�Ϳ��~!\n�нT�w�z IE ��[���ں����ﶵ][�{����][�q�l�l���}]�����w�� Outlook �νЭ��s�}���A���а���@��");	
		}

		m_MainText.SetWindowText("�pô�l��n�饢��");
	
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
