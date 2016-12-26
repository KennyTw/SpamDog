// WizSetEmailIdent.cpp : implementation file
//

#include "stdafx.h"
#include "spamdog.h"
#include "WizSetEmailIdent.h"


#include "../Swsocket/CoreClass.h"
#include "../Swsocket/ClientClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizSetEmailIdent property page

IMPLEMENT_DYNCREATE(CWizSetEmailIdent, CPropertyPageEx)

CWizSetEmailIdent::CWizSetEmailIdent() : CPropertyPageEx(CWizSetEmailIdent::IDD)
{
	//{{AFX_DATA_INIT(CWizSetEmailIdent)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP;
}

CWizSetEmailIdent::~CWizSetEmailIdent()
{
}

void CWizSetEmailIdent::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizSetEmailIdent)
	DDX_Control(pDX, IDC_EDIT_EMAILLIST, m_EMailList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizSetEmailIdent, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizSetEmailIdent)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizSetEmailIdent message handlers
void CWizSetEmailIdent::SendIdentEmail(char* IdentMailStr)
{




}

LRESULT CWizSetEmailIdent::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	int rc = 0;

	CString maillist;
	m_EMailList.GetWindowText(maillist);

	maillist.TrimLeft();
	maillist.TrimRight();

	if (maillist.GetLength() == 0)
	{
		AfxMessageBox("請輸入 Email ");
		return -1;
	}
	else
	{
	
		 CCoreSocket::Startup();

		 char path_buffer[_MAX_PATH];
		 char drive[_MAX_DRIVE];
		 char dir[_MAX_DIR];
		 char fname[_MAX_FNAME];
		 char ext[_MAX_EXT];

		 
		 HINSTANCE hInstance = GetModuleHandle(NULL);
		 GetModuleFileName(hInstance, path_buffer, MAX_PATH);

		 _splitpath( path_buffer, drive, dir, fname, ext );

		 char IniPath[_MAX_PATH];

		 strcpy(IniPath,drive);
		 strcat(IniPath,dir);	   
		 strcat(IniPath,"SpamDog.ini");

		 int DogId = GetPrivateProfileInt("SysSetup","DogId",0,IniPath);
  		 char DogIdStr[255];
		 itoa(DogId,DogIdStr,10);


		 char ServerIP[16];
		 GetPrivateProfileString("SysSetup","ServerIp","61.62.70.233",ServerIP,sizeof(ServerIP),IniPath);

		 char ServerPath[_MAX_PATH];
		 strcpy(ServerPath , "/SendEmailIdent.asp?");
		 strcat(ServerPath , "DogId=");
		 strcat(ServerPath , DogIdStr);
		 strcat(ServerPath , "&I=");
		 strcat(ServerPath , maillist.GetBuffer(maillist.GetLength()));	 
		 
		 CHTTPclient http; 
		 HANDLE hand = http.IPGET(ServerIP,"www.softworking.com",ServerPath,60 * 1000, 60 * 1000);

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

					rtn = WaitForSingleObject(hand,5);
			} 

		   FILE *fp = fopen(http.GetTempFilePath(),"rb");
		   if (fp != NULL)
		   {
				 
					int CheckCode = 0;		   
			   
					int status = 0;
					fscanf(fp,"%d\n",&status); //取status
					fscanf(fp,"%d\n",&CheckCode); //取CheckCode

					if (CheckCode == 991)
					{

					
					    if (status == 1)
						{
							//check ok   
							rc = 0;

						
						}
						else 
						{
						    //check Error
							rc = -1;
							
						}
					
					} 
					else
					{
							//check Error
						rc = -2;
					}

				
			  

			   fclose(fp);
		   }
		   else
		   {
					//Connect False
				rc = -2;
		   
		   }

		 
		   DeleteFile(http.GetTempFilePath());
		   CCoreSocket::Cleanup(); 


		   if (rc == -2)
		   {
			   AfxMessageBox("連線失敗請重新嘗試或檢查連線!");
			   return -1;
		   }
		   if (rc == -1)
		   {	
			   AfxMessageBox("Email無法送達,請檢查 Email 帳號!");
			   return -1;		   
		   }
 
	
	}

	

	

	return CPropertyPageEx::OnWizardNext();
}
