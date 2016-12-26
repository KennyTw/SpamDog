// LiveUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LiveUpdate.h"
#include "LiveUpdateDlg.h"
#include "Update.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_NOTIFYICON WM_USER+5 



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CLiveUpdateDlg dialog

CLiveUpdateDlg::CLiveUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLiveUpdateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLiveUpdateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	

	TimeCounter =3;
	MainWnd = 0;
	MainPID = 0;
}

void CLiveUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLiveUpdateDlg)
	DDX_Control(pDX, IDC_EDIT_MEMO, m_memo);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_STATIC_INFO, m_info);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLiveUpdateDlg, CDialog)
	//{{AFX_MSG_MAP(CLiveUpdateDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_NOTIFYICON,OnNotifyIcon)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLiveUpdateDlg message handlers

BOOL CLiveUpdateDlg::OnInitDialog()
{
	
	

	

	CDialog::OnInitDialog();

				

	FirstStart = true;

	/*
	CTime t1( 2005, 6, 1, 0, 0, 0 ); 
	CTime t2((time_t) CTime::GetCurrentTime().GetTime());
	
	CTimeSpan ts = t2-t1;
	if (ts.GetSeconds() >= 0)
	{
	
		AfxMessageBox("很抱歉 ! 此版本已過期 , 請至 http://www.softworking.com 下載更新版本 !");
		EndDialog(0);

		//如果有則刪除
		HKEY RegKey;
		RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&RegKey);		
	
		DWORD m_dwCount;
		RegQueryValueEx(RegKey,"SpamDog",0,NULL,NULL,&m_dwCount);
		if (m_dwCount > 0)
		{		
		  //RegSetValueEx(RegKey,"SpamDog",0,REG_SZ,(const unsigned char *)path_buffer,strlen(path_buffer));
			RegDeleteValue(RegKey,"SpamDog");

		}
        
		RegCloseKey(RegKey);



		return false;
	
	}*/

	
	
	  


	char *cmd = GetCommandLine();
	//AfxMessageBox(cmd);
	//EndDialog(0);

    int len = strlen(cmd);


	int count1 = 0;
	int Save1 = 0;
	int Save2 = 0;
	int parmidx = 0;

	int StartPos = 0;
	int ParmCount = 0;
	int SavePos = 0;
	
	for (int i=0 ; i < len ; i++)
	{
		if (cmd[i] == '"')
		{
			count1++ ;
			if (count1 == 2)
			{
				StartPos = i + 2;
				break;
			}

		}
	}

	if (StartPos == 0 ) 
	{
		EndDialog(0);
		return false;
	}

	for (int j=StartPos ; j < len ; j++)
	{
	
		if (cmd[j] == ' ' || j == len-1)
		{
			ParmCount ++;

			if (ParmCount == 1)
			{
				SavePos = StartPos;
				char tmp[255];
				strncpy(tmp,cmd+SavePos,j-SavePos);
				tmp[j-SavePos] = 0;			
				//AfxMessageBox(tmp);
				MainWnd = (HWND) atoi(tmp);	

			}
			else if (ParmCount == 2)
			{
			
				

				char tmp[255];
				strncpy(tmp,cmd+SavePos,j-SavePos);
				tmp[j-SavePos] = 0;			
				//AfxMessageBox(tmp);
				MainPID =   atoi(tmp);
			
			}
			else if (ParmCount == 3)
			{
			
				char tmp[255];
				strncpy(tmp,cmd+SavePos,j+1-SavePos);
				tmp[j+1-SavePos] = 0;
				
				CallType = atoi(tmp);
				
				 
			
			}


			SavePos = j + 1;
		

		}
	
	}


	 

	if (ParmCount != 3 )
	{
		EndDialog(0);
		return false;
	
	}

	 
 
	/*
	for (int i=0 ; i < len ; i++)
	{
	
		if (cmd[i] == '"')
		{
			count1 ++;
		} 
		else if (cmd[i] == ' ')
		{
			if (count1 >= 2)
			{			    
				parmidx ++;
				if (parmidx == 1)
				{
					Save1 = i + 1;			
				}
				else if (parmidx == 2)
				{
					Save2 = i + 1;	
				}
			}
		}else if (i == len -1 )
		{  
			if (Save2 - Save1 > 0)
			{
			    char tmp[255];
				strncpy(tmp,cmd+Save1,Save2-Save1);
				tmp[Save2-Save1] = 0;
				//AfxMessageBox(tmp);
				MainWnd = (HWND) atoi(tmp);				
			}

			if (i - Save2 > 0)
			{
				char tmp[255];
				strncpy(tmp,cmd+Save2,i - Save2+1);
				tmp[i - Save2+1] = 0;
				//AfxMessageBox(tmp);
				MainPID =   atoi(tmp);		
			}

		}

	
	}*/


	/*
	CString cmdstr = GetCommandLine();	 
	int pos = cmdstr.Find(" ");
	if (pos != -1)
	{
	  cmdstr.Delete(0,pos+1);

	  MainWnd = (HWND) atoi(cmdstr.GetBuffer(cmdstr.GetLength()));
	  cmdstr.ReleaseBuffer();

	  //::SendMessage(MainWnd,WM_CLOSESPAMDOG,0,0);
	}*/

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	
	

	 

	m_info.SetWindowText("更新中"); 
	m_memo.SetWindowText("更新中\r\n");

	

	SetTimer(2,10,NULL);

     /*AllocConsole();
	 HANDLE hConsole = CreateFile("CONOUT$", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	 SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT );
	 SetConsoleTitle("SpamDog Debug Console");*/
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLiveUpdateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID==SC_MINIMIZE)
	{ 
			   ShowWindow(SW_HIDE);	
	}
	else
	{
			   CDialog::OnSysCommand(nID,lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLiveUpdateDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLiveUpdateDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLiveUpdateDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);

	if (nIDEvent == 1)
	{

		TimeCounter--;
		CString showstr;
		showstr.Format("準備關閉.....%d",TimeCounter);
		AddLine(showstr);
		m_info.SetWindowText(showstr);

		if (TimeCounter == 0)
		{
		
			KillTimer(nIDEvent);
			DestroyWindow();
		}

	} else if (nIDEvent == 2)
	{
	
			KillTimer(2);
			
			CUpdate up(this);
			//up.SetProgress(&m_Progress);
			//up.SetStatic(&m_info);
			
			InProcess = true;
			if (CallType == 1 )
			{
				AddLine("傳送垃圾郵件資料");
				
				up.SendReport();
				
			}
			else if (CallType == 2)
			{
				AddLine("取得最新資料庫");
				up.GetLastDB();
				up.UpdateIni();
			
			}

 
			
			
			
		
			InProcess = false;		 

		

			
			SetTimer(1,1000,NULL); 

	
	}

	
}

void CLiveUpdateDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (!InProcess)
	{
		CDialog::OnOK(); 
		CDialog::OnClose();

		KillTimer(1);
		KillTimer(2);

		DestroyWindow();
	}
	else
	{
		PostQuitMessage(0); 
		AddLine("更新中斷..請稍後..準備關閉中...");
	}
}

void CLiveUpdateDlg::OnOK()
{

}

void CLiveUpdateDlg::OnCancel()
{

}


afx_msg void CLiveUpdateDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam) 
{ 
  
	if ((wParam == IDR_MAINFRAME ))
	{
		if (lParam == WM_LBUTTONDOWN) 
		{
			
			if (FirstStart)
			{
			
				FirstStart = false;

				RECT rWorkArea;
				CRect wrect;

				SystemParametersInfo(SPI_GETWORKAREA,  
						sizeof(RECT),
						&rWorkArea,
						0);  
	
				GetWindowRect(&wrect);

				SetWindowPos(NULL,rWorkArea.right - wrect.Width() ,rWorkArea.bottom - wrect.Height() ,wrect.Width(),wrect.Height(),SWP_HIDEWINDOW );
				SetWindowPos(&CWnd::wndTopMost,0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE);

				
			}

			ShowWindow(SW_RESTORE);

		


		}  
	}

} 

int CLiveUpdateDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	

   

	

	//GetWindowRect(&wrect);	
	//SetWindowPos(0,0,0, 0,0,SWP_HIDEWINDOW  );
	
	
	 	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;


	 
	
	// TODO: Add your specialized creation code here

	  tnd.cbSize=sizeof(NOTIFYICONDATA);
	  tnd.hWnd=this->m_hWnd;
	  tnd.uID=IDR_MAINFRAME;//IDR_MAINFRAME;
	  tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
	  tnd.uCallbackMessage=WM_NOTIFYICON;
	  tnd.hIcon=m_hIcon;
	  strcpy(tnd.szTip,"SpamDog LiveUpdate");
	  Shell_NotifyIcon(NIM_ADD,&tnd);

#ifdef _CONSOLEDBG
     AllocConsole();
	 hConsole = CreateFile("CONOUT$", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	 SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT );
	 SetConsoleTitle("SpamDog Liveupdate Debug Console");

	 printf("test\r\n");
#endif


	
	return 0;
}

void CLiveUpdateDlg::OnDestroy() 
{
 
	CDialog::OnDestroy();
	Shell_NotifyIcon(NIM_DELETE,&tnd);

#ifdef _CONSOLEDBG
	FreeConsole();
	hConsole = NULL;
#endif	

	
}
 

void CLiveUpdateDlg::AddLine(CString InStr)
{

	CString tempstr;
	m_memo.GetWindowText(tempstr);

	tempstr = tempstr + InStr;
	tempstr = tempstr + "\r\n";	

	m_memo.SetWindowText(tempstr);


    DWORD dwSel = m_memo.GetSel();
	m_memo.SetSel(HIWORD(dwSel), -1);


}