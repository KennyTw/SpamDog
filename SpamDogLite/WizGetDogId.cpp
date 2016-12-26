// WizGetDogId.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "WizGetDogId.h"
#include "WizSheet.h"

#include "../Swsocket/CoreClass.h"
#include "../Swsocket/ClientClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizGetDogId property page

IMPLEMENT_DYNCREATE(CWizGetDogId, CPropertyPageEx)

CWizGetDogId::CWizGetDogId() : CPropertyPageEx(CWizGetDogId::IDD)
{
	//{{AFX_DATA_INIT(CWizGetDogId)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_psp.dwFlags &= ~PSP_HASHELP;

	m_strHeaderTitle = _T("取得 使用者編號");
	m_strHeaderSubTitle = _T("使用者編號 為您的唯一序號,用來辨識您的身份"); 
}

CWizGetDogId::~CWizGetDogId()
{
}

void CWizGetDogId::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizGetDogId)
	DDX_Control(pDX, IDC_STATIC_MAIN, m_MainText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizGetDogId, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizGetDogId)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizGetDogId message handlers

LRESULT CWizGetDogId::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
//	AfxMessageBox("Next");

 
	return CPropertyPageEx::OnWizardNext();
	//return -1; for cancel
}

BOOL CWizGetDogId::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	   

	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	//pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT);
	pSheet->SetWizardButtons( PSWIZB_BACK);


	m_MainText.SetWindowText("取得使用者編號中 ");
	SetTimer(1,1000,NULL);

	return CPropertyPageEx::OnSetActive();
}

 

void CWizGetDogId::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	KillTimer(nIDEvent);

	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );

	   char IniPath[_MAX_PATH];	 
	  
	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	   
	   strcat(IniPath,"SpamDog.ini");

	   char ServerIp[16];
	   GetPrivateProfileString("SysSetup","ServerIp","61.62.70.233",ServerIp,sizeof(ServerIp),IniPath);

	   CCoreSocket::Startup();
	   
	   CHTTPclient http; 	
	   HANDLE hand = http.IPGET(ServerIp,"www.softworking.com","/GetDogID.asp",60 * 1000, 60 * 1000);
	   
	   int Step = 0;

	   
			

	DWORD rtn = WaitForSingleObject(hand,5);
	while (rtn != WAIT_OBJECT_0)
	{ 
				//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}



				rtn = WaitForSingleObject(hand,500);

				Step ++;
				if (Step % 4 == 0)
				{
					m_MainText.SetWindowText("取得 使用者編號 中 |");
				}
				else if (Step % 4 == 1)
				{
				    m_MainText.SetWindowText("取得 使用者編號 中 /");
				}
				else if (Step % 4 == 2)
				{
					m_MainText.SetWindowText("取得 使用者編號 中 --");
				}				
				else if (Step % 4 == 3)
				{
					m_MainText.SetWindowText("取得 使用者編號 中 \\");
				}

	}


		FILE *fp = NULL;
		fp = fopen(http.GetTempFilePath() , "rb");

		int NewDogID=0;
		fscanf(fp,"%d\n",&NewDogID);

		if (NewDogID > 0)
		{ 

			/*char DogIDStr[25];
			itoa(NewDogID , DogIDStr , 10);

			WritePrivateProfileString("SysSetup","DogId",DogIDStr,inipath);

			DogIdOK = true;*/
			CString IdStr;
			IdStr.Format("您的 使用者編號 是 %d ",NewDogID);


			m_MainText.SetWindowText(IdStr);

			CWizSheet* pSheet = (CWizSheet*)GetParent();
			pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT);
			pSheet->DogId = NewDogID;





		}
		else
		{
			m_MainText.SetWindowText("DogId 取得失敗 , 請檢查您的網路連線是否正常");

		
		}
		
		fclose(fp);

		DeleteFile(http.GetTempFilePath());
	
		//CCoreSocket::Cleanup(); for special user's case


	CPropertyPageEx::OnTimer(nIDEvent);
}
