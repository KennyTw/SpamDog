// UpgradeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Upgrade.h"
#include "UpgradeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CUpgradeDlg dialog

CUpgradeDlg::CUpgradeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpgradeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpgradeDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpgradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpgradeDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUpgradeDlg, CDialog)
	//{{AFX_MSG_MAP(CUpgradeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg message handlers

BOOL CUpgradeDlg::OnInitDialog()
{

	 /*AllocConsole();
	 HANDLE  hConsole = CreateFile("CONOUT$", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	 SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT );
	 SetConsoleTitle("SpamDog Debug Console");*/


	CDialog::OnInitDialog();

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
	//CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);   

	
	//m_check.SetCheck(BST_CHECKED);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUpgradeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUpgradeDlg::OnPaint() 
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
HCURSOR CUpgradeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUpgradeDlg::OnButton1() 
{
	
	
	 int CutNum = 0;

	 //if (m_check.GetCheck() == BST_CHECKED) CutNum = 1;

	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];


	   
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );



			char IndexDbPath[_MAX_PATH];
			char overflowDbPath[_MAX_PATH];
			char datadbPath[_MAX_PATH];	   

			strcpy(IndexDbPath,drive);
			strcat(IndexDbPath,dir);   

			strcpy(overflowDbPath,IndexDbPath);
			strcpy(datadbPath,IndexDbPath);		

			strcat(IndexDbPath,"DB\\db1new.db");
			strcat(overflowDbPath,"DB\\db2new.db");
			strcat(datadbPath,"DB\\db3new.db");

			DeleteFile(IndexDbPath);
			DeleteFile(overflowDbPath);
			DeleteFile(datadbPath);
	   
		
 
			CDB *newdb=NULL;
			newdb = new CDB(IndexDbPath,overflowDbPath,datadbPath,sizeof(DataFileHeader),1024 * 10);

		    
			//開啟 db1,2,3

			char IndexDbOldPath[_MAX_PATH];
			char overflowDbOldPath[_MAX_PATH];
			char datadbOldPath[_MAX_PATH];	   

			strcpy(IndexDbOldPath,drive);
			strcat(IndexDbOldPath,dir);   
	   
			strcpy(overflowDbOldPath,IndexDbOldPath);
			strcpy(datadbOldPath,IndexDbOldPath);

			strcat(IndexDbOldPath,"DB\\db1.db");
			strcat(overflowDbOldPath,"DB\\db2.db");
			strcat(datadbOldPath,"DB\\db3.db"); 

			int writecount = 0;

			for (int process = 0 ; process <= 1 ; process ++)
			{

			FILE *fp1 = fopen(IndexDbOldPath,"rb");
			FILE *fp2 = fopen(overflowDbOldPath,"rb");
			FILE *fp3 = fopen(datadbOldPath,"rb");

			int keycount = 0;
			//跳掉 index header
			fseek(fp1,sizeof(IndexHeader),SEEK_SET);
			//fseek(fp3,sizeof(DataFileHeader),SEEK_SET);
			while ( !feof( fp1 ))
			{ 
					Bucket m_Bucket;
					memset(&m_Bucket,0,sizeof(Bucket));
					int ReadSize = fread(&m_Bucket,sizeof(char),sizeof(Bucket),fp1);
					if (ReadSize == 0 ) break;

					if (m_Bucket.Status == 0)
					{					
						for (int i = 0 ; i < DefaultBucketSize ; i++)
						{
						    if(m_Bucket.IndexData[i].Status == 1)
							{
									MSG msg;
									while (::PeekMessage(&msg, NULL,   // pump message until none
										NULL, NULL, PM_REMOVE))   // are left in the queue
									{
										TranslateMessage(&msg);
										DispatchMessage(&msg);
									}
							 
							 

								//讀出資料		
								DataFileOld datafileold;
								memset(&datafileold,0,sizeof(DataFile));

								fseek(fp3,m_Bucket.IndexData[i].DataFilePos,SEEK_SET);
								fread(&datafileold,sizeof(char),sizeof(datafileold),fp3);

								//if (datafileold.UsedCount >= CutNum)
								//{


									keycount ++;

									DataFile datafilenew;
									memset(&datafilenew,0,sizeof(DataFile));

									datafilenew.BadRate = datafileold.BadRate;
									datafilenew.OkRate = datafileold.OkRate;
									datafilenew.Status = datafileold.Status;
									datafilenew.UsedCount = datafileold.UsedCount;
									datafilenew.SpamCount = 0;
									
								

									if (process == 1)
									{
									  int datapos = -1;										
									  datapos = newdb->InsertData((char *)&datafilenew,sizeof(DataFile));							 
									  int res = newdb->InsertKey(m_Bucket.IndexData[i].KeyData,datapos);

									  m_progress1.StepIt();

									  writecount++;
									}
									//if (res == -1)
									//{
										//Memo->SetWindowText("發現異常");										
									
									//}

							//	}
							
							}
						}

						if (m_Bucket.ChainData != -1)
						{
					
						//處理  chain						 
						int chain = m_Bucket.ChainData;			 
						IndexData m_IndexData;
						memset(&m_IndexData,0,sizeof(IndexData));
						while (chain != -1)
						{
							
							fseek(fp2,chain,SEEK_SET);
							fread(&m_IndexData,sizeof(char),sizeof(IndexData),fp2);			

								    MSG msg;
									while (::PeekMessage(&msg, NULL,   // pump message until none
										NULL, NULL, PM_REMOVE))   // are left in the queue
									{
										TranslateMessage(&msg);
										DispatchMessage(&msg);
									}


							if (m_IndexData.Status = 1)
							{
														
															

									//讀出資料		
								DataFileOld datafileold;
								memset(&datafileold,0,sizeof(DataFile));

								fseek(fp3,m_IndexData.DataFilePos,SEEK_SET);
								fread(&datafileold,sizeof(char),sizeof(datafileold),fp3);

								//if (datafileold.UsedCount >= CutNum)
								//{
									keycount ++;

								    

									DataFile datafilenew;
									memset(&datafilenew,0,sizeof(datafilenew));

									datafilenew.BadRate = datafileold.BadRate;
									datafilenew.OkRate = datafileold.OkRate;
									datafilenew.Status = datafileold.Status;
									datafilenew.UsedCount = datafileold.UsedCount;
									datafilenew.SpamCount = 0;


									if (process == 1)
									{
										int datapos = -1;									
										datapos = newdb->InsertData((char *)&datafilenew,sizeof(DataFile));								 
										int res = newdb->InsertKey(m_IndexData.KeyData,datapos);

										m_progress1.StepIt();

										writecount++;
									}
								
								//}
							}				 

							if (chain == m_IndexData.ChainData)
							{							
								//Memo->SetWindowText("發現嚴重異常");
							 
								break;
							}
						    chain = m_IndexData.ChainData;			
						}
					
						}
					}

					
					 

			}


			fclose(fp1);
			fclose(fp2);
			fclose(fp3);
			

			if (process == 0)
			{
				m_progress1.SetRange32(0,keycount);
				m_progress1.SetStep(1);
				m_progress1.SetPos(0);

			}
			}


			
			
			
			delete newdb;

			//CString tmpstr;
			//tmpstr.Format("%d",writecount);
			//AfxMessageBox(tmpstr);

			//更換至現在db

			
			
			char datadbRNPath[_MAX_PATH];	   
			char indexdbRNPath[_MAX_PATH];	   
			char overdbRNPath[_MAX_PATH];
		

			strcpy(datadbRNPath,drive);
			strcat(datadbRNPath,dir);
			strcat(datadbRNPath,"DB\\db3.bak");

			strcpy(indexdbRNPath,drive);
			strcat(indexdbRNPath,dir);
			strcat(indexdbRNPath,"DB\\db1.bak");

			strcpy(overdbRNPath,drive);
			strcat(overdbRNPath,dir);
			strcat(overdbRNPath,"DB\\db2.bak");			

			DeleteFile(datadbRNPath);
			DeleteFile(indexdbRNPath);
			DeleteFile(overdbRNPath);
			
			rename(IndexDbOldPath,indexdbRNPath);
			rename(overflowDbOldPath,overdbRNPath);
			rename(datadbOldPath,datadbRNPath);


			rename(IndexDbPath,IndexDbOldPath);
			rename(overflowDbPath,overflowDbOldPath);
			rename(datadbPath,datadbOldPath);

		

			DeleteFile(indexdbRNPath);
			DeleteFile(overdbRNPath);
			DeleteFile(datadbRNPath);

			

			char IpDBPath[_MAX_PATH];	
			memset(IpDBPath,0,_MAX_PATH);

			strcpy(IpDBPath,drive);	    
			strcat(IpDBPath,dir);	  
			strcat(IpDBPath,"DB\\ipdb*.db");


			 //刪除 ipdb
			 SHFILEOPSTRUCT oFile;

			 
			 memset(&oFile, 0,sizeof(SHFILEOPSTRUCT));
			 oFile.hwnd=this->m_hWnd;
			 oFile.wFunc=FO_DELETE;
			 oFile.pFrom=IpDBPath;	
			 oFile.fFlags= FOF_NOCONFIRMATION;
			 SHFileOperation(&oFile);
		     

	  AfxMessageBox("升級完成");

	  EndDialog(0);


 
}

 
