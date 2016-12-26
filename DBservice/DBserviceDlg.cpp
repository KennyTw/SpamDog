// DBserviceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBservice.h"
#include "DBserviceDlg.h"

#include <afxdb.h>

#include "../Swparser/MailParser.h"
#include "../Swzip/Compress.h"
#include "../Swspam/Checker.h"

#include "../Swsocket/CoreClass.h"
#include "../Swsocket/ClientClass.h"

#include <conio.h>
#include "WhoisClient.h"

#pragma warning(disable:4146) 
#import "C:\Program Files\common files\system\ado\msado15.dll"  no_namespace rename("EOF", "EndOfFile")

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
// CDBserviceDlg dialog

CDBserviceDlg::CDBserviceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBserviceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBserviceDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	hConsole = NULL;
	/*AllocConsole();
	hConsole = CreateFile("CONOUT$", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT );
	SetConsoleTitle("DBserver Console");*/

}

CDBserviceDlg::~CDBserviceDlg()
{
	if (hConsole != NULL)
	{
	  FreeConsole();
	  hConsole = NULL;
	}
}

void CDBserviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBserviceDlg)
	DDX_Control(pDX, IDC_CHECK1, m_CkDb);
	DDX_Control(pDX, IDC_EDIT4, m_ZipDesc);
	DDX_Control(pDX, IDC_EDIT1, m_ZipFile);
	DDX_Control(pDX, IDC_EDIT2, m_MailId);
	DDX_Control(pDX, IDC_STATICTIME, m_STATICTIME);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDBserviceDlg, CDialog)
	//{{AFX_MSG_MAP(CDBserviceDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBserviceDlg message handlers

BOOL CDBserviceDlg::OnInitDialog()
{
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
	 CoInitialize(NULL);

    //OnTimer(0); 
	//SetTimer(1,1000 * 60 * 30,NULL); //30 min

   
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDBserviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDBserviceDlg::OnPaint() 
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
HCURSOR CDBserviceDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDBserviceDlg::ParseStr(StrList *m_list,char *InStr)
{
	int len = strlen(InStr);
	int SavePos = 0;

	for(int i=0 ; i < len ; i++)
	{
	
		if (InStr[i] == char(13))
		{
		
			if ( i - SavePos > 0)
			{
				char tmpstr[225];
				memset(tmpstr,0,225);
				strncpy(tmpstr,InStr + SavePos , i - SavePos);

				string mstr = tmpstr;
				//Trim2Byte(&mstr);

				//char *findres = strstr(mstr.c_str(),".");
			//	if (findres != NULL)
				  m_list->insert(m_list->end(),mstr);
				//m_List->AddString(tmpstr);
				//OutputDebugString(tmpstr);
				//OutputDebugString("\n");	
				SavePos = i + 1;
			}
		
		}
	
	}

}

void CDBserviceDlg::WriteQueryLog(char *SQL)
{

	FILE *fp=NULL;
	fp = fopen("c:\\sql.txt","r+b");

	if (fp == NULL)
	{
		fp = fopen("c:\\sql.txt","w+b");
	}

	fseek(fp,0,SEEK_END);

	fprintf(fp,"%s\r\n",SQL);

	fclose(fp);


}

void CDBserviceDlg::ConvertMail()
{
 

  CDatabase db,db2,db3,db4,db5;
  db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
  //db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

  db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
  db3.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
  db4.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
  db5.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

  CRecordset rec(&db);


  	_ConnectionPtr mConnection ; 
	_CommandPtr mCommand ;

	char AdoConnStr[255];
	strcpy(AdoConnStr,"Provider=MSDASQL.1;Password=dog1234;Persist Security Info=True;User ID=dog;Data Source=SpamDog2;Initial Catalog=SpamDog2");



	HRESULT hr;
	hr = mConnection.CreateInstance(__uuidof(Connection));	
	if(SUCCEEDED(hr))
	{
		hr = mConnection->Open(AdoConnStr,"","",adModeUnknown);
		if(SUCCEEDED(hr))
		{
			mCommand.CreateInstance(__uuidof(Command));
			mCommand->ActiveConnection = mConnection;
			 
		}
	}


   BOOL QueryOk = false;
   //try
   //{
      WriteQueryLog("select SpamMailId,MailContent,OrgLen,isCompress,addtime,DogId from spammail (nolock) where  SpamMailId in (select SpamMailId from SpamMailEC where (OrgLen > 0 ) and AtVersion = 0)");
	  //QueryOk = rec.Open(CRecordset::forwardOnly,"select SpamMailId,MailContent,OrgLen,isCompress,addtime,DogId from spammail (nolock) where  SpamMailId in (select SpamMailId from SpamMailEC where (OrgLen > 0 ) and AtVersion = 0)");
	  QueryOk = rec.Open(CRecordset::forwardOnly,"select SpamMailId,MailContent,OrgLen,isCompress,addtime,DogId from spammail (nolock) where  OrgLen > 0  and AtVersion = 0");
   //}
   //catch(...)
   //{
	 // AfxMessageBox("Big Query Error");
   //}

  // if (QueryOk)
   //{
	   while (!rec.IsEOF())
	   {
	   
		   bool NeedSendFixMail = false;
		   CString SpamMailId;
		   CString MailContent;
		   CString orglenstr;
		   CString isCompress;
		   CString AddTime;
		   CString DogId;
		   CString gIpId;

		   CString SaveIpId;

		   rec.GetFieldValue((int)0,SpamMailId);
		   rec.GetFieldValue((int)1,MailContent);
		   rec.GetFieldValue((int)2,orglenstr);
		   rec.GetFieldValue((int)3,isCompress);
		   rec.GetFieldValue((int)4,AddTime);
		   rec.GetFieldValue((int)5,DogId);

		   CMailCodec MailCodec;

		   char *debase = NULL;
		   unsigned int sourcelen;
		   char *sourcestr = MailContent.GetBuffer(MailContent.GetLength());		 
	 
		   MailCodec.Base64Decode2(sourcestr,&debase,&sourcelen);

		   ReportMailData maildata;
		   memset(&maildata,0,sizeof(maildata));

				if (isCompress == "1")
				{
			
					unsigned int outsize = atoi(orglenstr);
					char *outbuf = new char[outsize +1];
					memset(outbuf,0,outsize +1);
			
			
					CCompress cc;
					cc.DeCompress((unsigned char*) debase,sourcelen,(unsigned char*) outbuf,&outsize);
		

					
					memcpy(&maildata,outbuf,sizeof(maildata));
					//ParseStr(&m_list,outbuf);
				 
					//FILE *fp=NULL;

					//fp = fopen("c:\\1.txt","w+b");
					//fwrite(outbuf,1,outsize,fp);
					//fclose(fp);

					delete outbuf;

				}
				else
				{
			
					//ParseStr(&m_list,debase);

					//ReportMailData maildata;
					//memset(&maildata,0,sizeof(maildata));
					memcpy(&maildata,debase,sizeof(maildata));

					
				 
			
				}


			if (maildata.Type == 1)
			{
				//處理 sys report mail , update Atversion = 2
				CString query;
				query.Format("update spammail set AtVersion = 2 where SpamMailId=%s",SpamMailId);
				WriteQueryLog(query.GetBuffer(query.GetLength()));
				query.ReleaseBuffer();

				mConnection->Execute(query.AllocSysString(),NULL,NULL);
			} 
			//else if (maildata.Type == 2)
			//{//ok mail
			
			//}
			else
			if (strlen(maildata.Sender) > 0 && strlen(maildata.Subject) > 0 && strlen(maildata.IP) > 0 && (maildata.Type == 0  || maildata.Type == 2))
			{
				if (mCommand->Parameters->Count > 0)
				{
					mCommand->Parameters->Delete("Subject");
					mCommand->Parameters->Delete("Sender");
				}

				TrimCntrl(maildata.Subject);
				TrimCntrl(maildata.Sender);

				//if (maildata.Type == 2) 

					//處理 ip

					int OkRate = 0;
					int BadRate = 0;

					if (maildata.Type == 0)
					{
					    BadRate = 1;
					}
					else
					{
						
						OkRate = 1;
					}

					CString sql;
					CRecordset rec2(&db2);
					sql.Format("select IpId,Ok,Bad from iptbl (nolock) where Ip = '%s'",maildata.IP);

					WriteQueryLog(sql.GetBuffer(sql.GetLength()));
					sql.ReleaseBuffer();

					if (rec2.Open(CRecordset::forwardOnly,sql))
					{
						  if(!rec2.IsEOF())
						  {
							    //確認誤判
							    
							    CString IpId;
								CString sql;

								rec2.GetFieldValue((int)0,IpId);

								SaveIpId = IpId;

								CRecordset rec4(&db4);
								sql.Format("select top 1 RateLogId from ratelog (nolock) where Ipid = %s and DogId= %s and (ok > %d or bad > %d) and AtVersion = 0",IpId,DogId,OkRate,BadRate);

								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();

								if (rec4.Open(CRecordset::forwardOnly,sql))
								{
									if(!rec4.IsEOF())
									{
										//可能有誤判
										CString RateLogId;
										rec4.GetFieldValue((int)0,RateLogId);

										sql.Format("select SubId from subtbl (nolock)  where DogId=%s and ipid = %s and subject = ? and sender = ?  and  AddTime >= DATEADD(day, -7, getdate())",DogId,IpId);

										WriteQueryLog(sql.GetBuffer(sql.GetLength()));
										sql.ReleaseBuffer();

										mCommand->CommandText  = sql.AllocSysString();
										mCommand->CommandType = adCmdText;
					
										_ParameterPtr p1 = mCommand->CreateParameter("Subject",adChar ,adParamInput,strlen(maildata.Subject),maildata.Subject);	
										mCommand->Parameters->Append(p1);

										_ParameterPtr p2 = mCommand->CreateParameter("Sender",adChar ,adParamInput,strlen(maildata.Sender),maildata.Sender);	
										mCommand->Parameters->Append(p2);
										
										_RecordsetPtr mRecordset;

										//try
										//{										
											mRecordset = mCommand->Execute(NULL,NULL,adCmdText);
										//}
										//catch(...)
										//{
											//AfxMessageBox(adCmdText);
										//}

										    if (!mRecordset->EndOfFile)
											{

										//CRecordset rec5(&db3);
										//sql.Format("select SubId from subtbl (nolock)  where DogId=%s and ipid = %s and sender = '%s' and subject = '%s' and  AddTime > DATEADD(day, -7, getdate())",DogId,IpId,maildata.Sender,maildata.Subject);
									
										//if (rec5.Open(CRecordset::forwardOnly,sql))
										//{
											//if(!rec5.IsEOF())
											//{
												//誤判
												CString SubId;
												SubId = (char *) (_bstr_t) mRecordset->Fields->GetItem("SubId")->Value;
												//rec5.GetFieldValue((int)0,SubId);
												//SubId = mRecordset->Fields->GetItem("ident")->Value.SetString(; 

												//刪除 ratelog
												sql.Format("update ratelog set AtVersion = 1 where RateLogId = %s",RateLogId);												
												WriteQueryLog(sql.GetBuffer(sql.GetLength()));
												sql.ReleaseBuffer();
												db5.ExecuteSQL(sql);
												//刪除 subtbl
												sql.Format("delete from subtbl where SubId = %s",SubId);
												WriteQueryLog(sql.GetBuffer(sql.GetLength()));
												sql.ReleaseBuffer();
												db5.ExecuteSQL(sql);
												//更正 iptbl
												sql.Format("update iptbl set Bad = Bad - %d , Ok = Ok - %d , lastusedtime = getdate() where Bad - %d >=0 and Ok - %d >= 0 and Ipid = %s",BadRate,OkRate,BadRate,OkRate,IpId);
												WriteQueryLog(sql.GetBuffer(sql.GetLength()));
												sql.ReleaseBuffer();

												db5.ExecuteSQL(sql);
												
												
												


											}
											else
											{
												NeedSendFixMail = true;
												gIpId = IpId;
												
											}
											//else
											//{											
												//沒誤判偏修
												CString sql2;
												sql2.Format("insert into ratelog (Ok,Bad,DogId,SpamMailId,Ipid) values (%d,%d,%s,%s,%s)",OkRate,BadRate,DogId,SpamMailId,IpId);
												
												WriteQueryLog(sql2.GetBuffer(sql2.GetLength()));
												sql2.ReleaseBuffer();

												db5.ExecuteSQL(sql2);

												sql.Format("update iptbl set Bad = Bad + %d , Ok = Ok + %d , lastusedtime = getdate() where Ipid = %s",BadRate,OkRate,IpId);
												
												WriteQueryLog(sql.GetBuffer(sql.GetLength()));
												sql.ReleaseBuffer();

												db5.ExecuteSQL(sql);

												
											//}
										//}

												if (mCommand->Parameters->Count > 0)
												{
													mCommand->Parameters->Delete("Subject");
													mCommand->Parameters->Delete("Sender");
												}
									}
									else
									{
									
										CString sql2;
										sql2.Format("insert into ratelog (Ok,Bad,DogId,SpamMailId,Ipid) values (%d,%d,%s,%s,%s)",OkRate,BadRate,DogId,SpamMailId,IpId);
										WriteQueryLog(sql2.GetBuffer(sql2.GetLength()));
										sql2.ReleaseBuffer();

										db3.ExecuteSQL(sql2);

										sql.Format("update iptbl set Bad = Bad + %d , Ok = Ok + %d , lastusedtime = getdate() where Ipid = %s",BadRate,OkRate,IpId);
										WriteQueryLog(sql.GetBuffer(sql.GetLength()));
										sql.ReleaseBuffer();

										db3.ExecuteSQL(sql);

									
									}


								}
							
  
						  }
						  else
						  {
						  
							    //找不到 ip
								CRecordset rec3(&db3);

							    CString sql;
								sql.Format("insert into iptbl (Ip,Ok,Bad,lastusedtime) values ('%s',%d,%d,getdate())",maildata.IP,OkRate,BadRate);
								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();

								db3.ExecuteSQL(sql);

								

								sql.Format("select ipid from iptbl where ip = '%s'",maildata.IP);
								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();

								if (rec3.Open(CRecordset::forwardOnly,sql))
								{
									if(!rec3.IsEOF())
									{

										CString IpId;
										rec3.GetFieldValue((int)0,IpId);
										SaveIpId = IpId;

										CString sql2;
										sql2.Format("insert into ratelog (Ok,Bad,DogId,SpamMailId,Ipid) values (%d,%d,%s,%s,%s)",OkRate,BadRate,DogId,SpamMailId,IpId);
										WriteQueryLog(sql2.GetBuffer(sql2.GetLength()));
										sql2.ReleaseBuffer();

										db3.ExecuteSQL(sql2);



									}
								}

								rec3.Close();

							 

						  
						  }
					}



		
				CString Sql;
				Sql.Format("insert into subtbl (Subject,IP,Sender,orgsid,ReportTime,Type,DogId , IpId) values (?,'%s',?,%s,'%s',%d,%s,%s)",maildata.IP,SpamMailId,AddTime,maildata.Type,DogId,SaveIpId);
				//mCommand->CommandText = "insert into subtbl (Subject,IP,TimeStr) values (?,?,?)";

				WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				sql.ReleaseBuffer();
			
		
			

				if (strlen(maildata.Subject) > 0 && strlen(maildata.Sender) > 0)
				{
					mCommand->CommandText  = Sql.AllocSysString();
					mCommand->CommandType = adCmdText;
					
					_ParameterPtr p1 = mCommand->CreateParameter("Subject",adChar ,adParamInput,strlen(maildata.Subject),maildata.Subject);	
					mCommand->Parameters->Append(p1);

					_ParameterPtr p2 = mCommand->CreateParameter("Sender",adChar ,adParamInput,strlen(maildata.Sender),maildata.Sender);	
					mCommand->Parameters->Append(p2);

				
			 
				
					//try
					//{
					  mCommand->Execute(NULL,NULL,adCmdText);
					//}
					//catch(...)
					//{
					//	AfxMessageBox(Sql);
					//}
				}

				CString query;
				query.Format("update spammail set AtVersion = 1 where SpamMailId=%s",SpamMailId);
				WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				sql.ReleaseBuffer();
				//try
				//{
				  mConnection->Execute(query.AllocSysString(),NULL,NULL);
				//}
				//catch(...)
				//{
					//AfxMessageBox(query);
				
				//}
			

			}
			else
			{
				CString query;
				query.Format("update spammail set AtVersion = 1 where SpamMailId=%s",SpamMailId);
				WriteQueryLog(query.GetBuffer(query.GetLength()));
				query.ReleaseBuffer();
				//try
				//{
				  mConnection->Execute(query.AllocSysString(),NULL,NULL);
				//}
				//catch(...)
				//{
					//AfxMessageBox(query);
				//}
			
			
			}

			
				//db2.ExecuteSQL(query);
			
				MailContent.ReleaseBuffer(); 
				MailCodec.FreeBase64Decode(debase);

				if (NeedSendFixMail)
				{
					//try
					//{
					 SendFixMail(DogId,gIpId);
					//}
					//catch(...)
					//{
						//AfxMessageBox("SendFixMail");					
					//}
				}

				if (strlen(maildata.Sender) > 0 && strlen(maildata.IP) > 0 && strlen(maildata.Subject) > 0 && (maildata.Type == 0  || maildata.Type == 2))
				{

						char Rip[20];
						strcpy(Rip,maildata.IP);				
						IpReserve(Rip);

						//確認是否加入 RBL
						CString sql;
						CRecordset rec2(&db2);

						sql.Format("select ipid from iptbl where (Bad - Ok >= 5) and ipid = %s and RblStatus = 0",SaveIpId);
						WriteQueryLog(sql.GetBuffer(sql.GetLength()));
						sql.ReleaseBuffer();

						if (rec2.Open(CRecordset::forwardOnly,sql))
						{
							if (!rec2.IsEOF())
							{
								//要加入 rbl
								//dnscmd localhost /RecordAdd test.com www.my A 10.22.22.22

								char Cmd[255];
								strcpy(Cmd,"c:/dnscmd.exe 192.168.1.3 /RecordAdd rbl.softworking.com ");
								strcat(Cmd,Rip);
								strcat(Cmd," A 127.0.0.5");
								WinExec(Cmd,SW_SHOW);

								sql.Format("update iptbl set RblStatus = 1 where ipid = %s",SaveIpId);
								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();
								db3.ExecuteSQL(sql);

							}
						}

						rec2.Close();
						sql.Format("select ipid from iptbl where (Bad - Ok <= 5) and ipid = %s and RblStatus <> 0",SaveIpId);
						WriteQueryLog(sql.GetBuffer(sql.GetLength()));
						sql.ReleaseBuffer();

						if (rec2.Open(CRecordset::forwardOnly,sql))
						{
							if (!rec2.IsEOF())
							{
								//要拉出 rbl
								//dnscmd localhost /RecordDelete test.com www.my A 

								char Cmd[255];
								strcpy(Cmd,"c:/dnscmd.exe 192.168.1.3 /RecordDelete rbl.softworking.com ");
								strcat(Cmd,Rip);
								strcat(Cmd," A /f");
								WinExec(Cmd,SW_SHOW);

								sql.Format("update iptbl set RblStatus = 0 where ipid = %s",SaveIpId);
								WriteQueryLog(sql.GetBuffer(sql.GetLength()));
								sql.ReleaseBuffer();

								db3.ExecuteSQL(sql);

							}
						}


				}


		   rec.MoveNext();

	   }

   //}

	
//掃全部的 block ip

  
  rec.Close();
  CString sql;
  sql.Format("select IP,ipid from iptbl (nolock) where (Bad - Ok >= 5)  and RblStatus = 0");

  WriteQueryLog(sql.GetBuffer(sql.GetLength()));
  sql.ReleaseBuffer();

  if (rec.Open(CRecordset::forwardOnly,sql))
  {
	   while (!rec.IsEOF())
	   {   
		  
		   CString IP;		   
		   CString IPid;

		   rec.GetFieldValue((int)0,IP);
		   rec.GetFieldValue((int)1,IPid);

		   char Rip[20];
		   strcpy(Rip,IP);				
		   IpReserve(Rip);

		   char Cmd[255];
		   strcpy(Cmd,"c:/dnscmd.exe 192.168.1.3 /RecordAdd rbl.softworking.com ");
		   strcat(Cmd,Rip);
		   strcat(Cmd," A 127.0.0.5");
		   WinExec(Cmd,SW_SHOW);

		   sql.Format("update iptbl set RblStatus = 1 where ipid = %s",IPid);
		   WriteQueryLog(sql.GetBuffer(sql.GetLength()));
		   sql.ReleaseBuffer();

		   db2.ExecuteSQL(sql);

		rec.MoveNext();


	   }
	  
  }

     

  db.Close();
  
  mConnection->Close();

  db2.Close();
  db3.Close();
  db4.Close();
  db5.Close();

  //沒問題就清 log
  DeleteFile("c:\\sql.txt");
  FILE *fp;
  fp=fopen("c:\\sql.txt","w+b");
  fclose(fp);
  

}


void CDBserviceDlg::ConvertReport()
{
	/*
		AtVersion = 0 未處理
		AtVersion = 1 已處理完畢
		AtVersion = 2 Ok->Bad (停用)
		AtVersion = 3 Bad->Ok (停用)
		AtVersion = 4 Ok
		AtVersion = 5 Bad
		AtVersion = 6 Reset
		AtVersion = 7 偏修觀察中 
		AtVersion = 8 gateway (未處理)
		AtVersion = 9 gateway (已處理)
		AtVersion = 10 enews (未處理)
		AtVersion = 11 enews (已處理)


		AtVersion = 998 Stop update (尚未 update)
		AtVersion = 999 Stop update (已 update)


	*/

		StrList m_list;

	    CDatabase db,db2,db3;
		db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
		db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
		db3.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

		CRecordset rec(&db);

		

		if (rec.Open(CRecordset::forwardOnly,"select RpId,RpStr,OrgLen,isCompress,report.RemoteIp,RpStr2,OrgLen2,isCompress2,report.DogId from report (nolock) , dogid (nolock) where  report.dogid = dogid.dogid and dogid.version = 2 and  (OrgLen > 0 or OrgLen2 > 0 ) and AtVersion = 0"))
		{

			CString szResult;
			CString orglenstr;
			CString isCompress;
			CString sRpid;
			CString RemoteIp;

			CString szResult2;
			CString orglenstr2;
			CString isCompress2;
			CString sDogId;
		
	
			while(!rec.IsEOF())
			{
			

				rec.GetFieldValue((int)0,sRpid);
				rec.GetFieldValue((int)1,szResult);
				rec.GetFieldValue((int)2,orglenstr);
				rec.GetFieldValue((int)3,isCompress);
				rec.GetFieldValue((int)4,RemoteIp);

				rec.GetFieldValue((int)5,szResult2);
				rec.GetFieldValue((int)6,orglenstr2);
				rec.GetFieldValue((int)7,isCompress2);
				rec.GetFieldValue((int)8,sDogId);

				


			

				CMailCodec MailCodec;

				char *debase = NULL;
				unsigned int sourcelen;
				char *sourcestr = szResult.GetBuffer(szResult.GetLength());		 
	 
				MailCodec.Base64Decode2(sourcestr,&debase,&sourcelen);


				if (isCompress == "1")
				{
			
					unsigned int outsize = atoi(orglenstr);
					char *outbuf = new char[outsize +1];
					memset(outbuf,0,outsize +1);
			
			
					CCompress cc;
					cc.DeCompress((unsigned char*) debase,sourcelen,(unsigned char*) outbuf,&outsize);
		

					ParseStr(&m_list,outbuf);
				 

					delete outbuf;

				}
				else
				{
			
					ParseStr(&m_list,debase);
				 
			
				}

				StrList::iterator itor;
				for (itor =  m_list.begin(); itor != m_list.end() ; itor++)
				{
					string tmpstr = *itor;

					CString sql;
					CRecordset rec2(&db2);
					sql.Format("select IpId,Ok,Bad,AtVersion,FixBlockCount,IpType from iptbl (nolock) where Ip = '%s'",tmpstr.c_str());

					if (rec2.Open(CRecordset::forwardOnly,sql))
					{
						  if(!rec2.IsEOF())
						  {
							    //確認誤判
							    
							    CString IpId;
								CDBVariant Ok;
								CDBVariant Bad;
								CDBVariant Version;
								CDBVariant FixBlockCount;
								CDBVariant IpType;

								rec2.GetFieldValue((int)0,IpId);
								rec2.GetFieldValue((int)1,Ok,SQL_C_SLONG);
								rec2.GetFieldValue((int)2,Bad,SQL_C_SLONG);
								rec2.GetFieldValue((int)3,Version,SQL_C_SLONG);
								rec2.GetFieldValue((int)4,FixBlockCount,SQL_C_SLONG);
								rec2.GetFieldValue((int)5,IpType,SQL_C_SLONG);

								

								

								CString sql;

							if (Version.m_iVal != 998 && Version.m_iVal != 999 && IpType.m_iVal == 0)
							{
								if (Ok.m_iVal > 0 && Version.m_iVal != 8 &&  Version.m_iVal != 9 && Version.m_iVal != 10 && Version.m_iVal != 11 )
								{
									
									//sql.Format("update iptbl set Bad = Bad + 1 , AtVersion =  2 where Ipid = %s",IpId);
									//if (FixBlockCount.m_iVal >= 5) //偏修大於 5
									///{
									  //sql.Format("update iptbl set Bad = Bad + 1 , AtVersion =  7 , FixBlockCount = FixBlockCount + 1, LastFixBlockTime = getdate(), lastusedtime = getdate() where Ipid = %s",IpId);
									//}
									//else
									//{
									  //sql.Format("update iptbl set Bad = Bad + 1 , AtVersion =  0 , FixBlockCount = FixBlockCount + 1, LastFixBlockTime = getdate(), lastusedtime = getdate() where Ipid = %s",IpId);
									//}

									SendFixMail(sDogId,IpId);


								}
								else
								{
									 
									

									
									CString sql2;
									sql2.Format("insert into ratelog (Bad,DogId,ReportId,Ipid) values (1,%s,%s,%s)",sDogId,sRpid,IpId);
									db3.ExecuteSQL(sql2);

									sql.Format("update iptbl set Bad = Bad + 1 , lastusedtime = getdate() where Ipid = %s",IpId);
									db3.ExecuteSQL(sql);
								
								}

							

								
							}


						


 
						  }
						  else
						  {
						  
							    CRecordset rec3(&db3);

							    CString sql;
								sql.Format("insert into iptbl (Ip,Bad,lastusedtime) values ('%s',1,getdate())",tmpstr.c_str());
								db3.ExecuteSQL(sql);

								sql.Format("select ipid from iptbl where ip = '%s'",tmpstr.c_str());
								if (rec3.Open(CRecordset::forwardOnly,sql))
								{
									if(!rec3.IsEOF())
									{

										CString IpId;
										rec3.GetFieldValue((int)0,IpId);

										CString sql2;
										sql2.Format("insert into ratelog (Bad,DogId,ReportId,Ipid) values (1,%s,%s,%s)",sDogId,sRpid,IpId);
										db3.ExecuteSQL(sql2);



									}
								}

								rec3.Close();

								//db3.ExecuteSQL(sql);

						  
						  }

						  
					}

	
				}

				m_list.clear();

				szResult.ReleaseBuffer(); 
				MailCodec.FreeBase64Decode(debase);


				if (szResult2.GetLength() > 0)
				{
					char *debase = NULL;
					unsigned int sourcelen;
					char *sourcestr = szResult2.GetBuffer(szResult2.GetLength());
 

					MailCodec.Base64Decode2(sourcestr,&debase,&sourcelen);
		

					if (isCompress2 == "1")
					{
			
						unsigned int outsize = atoi(orglenstr2);
						char *outbuf = new char[outsize +1];
						memset(outbuf,0,outsize +1);
			
			
						CCompress cc;
						cc.DeCompress((unsigned char*) debase,sourcelen,(unsigned char*) outbuf,&outsize);
		

						ParseStr(&m_list,outbuf);
				 

						delete outbuf;

					}
					else
					{
			
						ParseStr(&m_list,debase);			 
			
					}

					StrList::iterator itor;
					for (itor =  m_list.begin(); itor != m_list.end() ; itor++)
					{
						string tmpstr = *itor;
				

					   	CString sql;
						CRecordset rec2(&db2);
						sql.Format("select IpId,Ok,Bad,AtVersion,FixBlockCount,IpType from iptbl (nolock) where Ip = '%s'",tmpstr.c_str());

						if (rec2.Open(CRecordset::forwardOnly,sql))
						{
						  if(!rec2.IsEOF())
						  {
							    
							    CString IpId;
								CDBVariant Ok;
								CDBVariant Bad;
								CDBVariant Version;
								CDBVariant FixBlockCount;
								CDBVariant IpType;

								rec2.GetFieldValue((int)0,IpId);
								rec2.GetFieldValue((int)1,Ok,SQL_C_SLONG);
								rec2.GetFieldValue((int)2,Bad,SQL_C_SLONG);
								rec2.GetFieldValue((int)3,Version,SQL_C_SLONG);
								rec2.GetFieldValue((int)4,FixBlockCount,SQL_C_SLONG);
								rec2.GetFieldValue((int)5,IpType,SQL_C_SLONG);


								CString sql;
								
							if (Version.m_iVal != 998 && Version.m_iVal != 999 && IpType.m_iVal == 0)
							{

								if (Bad.m_iVal > 0 && Version.m_iVal != 8 &&  Version.m_iVal != 9 && Version.m_iVal != 10 && Version.m_iVal != 11)
								{
									//sql.Format("update iptbl set Ok = Ok + 1 , AtVersion = 3 where Ipid = %s",IpId);
									//if (FixBlockCount.m_iVal >= 5) //偏修大於 5
									//{
									  //sql.Format("update iptbl set Ok = Ok + 1 , AtVersion = 7 , FixBlockCount = FixBlockCount + 1, LastFixBlockTime = getdate(), lastusedtime = getdate() where Ipid = %s",IpId);
									//}
									//else
									//{
									 // sql.Format("update iptbl set Ok = Ok + 1 , AtVersion = 0 , FixBlockCount = FixBlockCount + 1, LastFixBlockTime = getdate(), lastusedtime = getdate() where Ipid = %s",IpId);
									//}

									SendFixMail(sDogId,IpId);

									//CString sql2;
									//sql2.Format("update iptbl set BkRpId=%s where IpId=%s",sRpid,IpId);
									//db3.ExecuteSQL(sql2);
								}
								else
								{
									CString sql2;
									sql2.Format("insert into ratelog (Ok,DogId,ReportId,Ipid) values (1,%s,%s,%s)",sDogId,sRpid,IpId);
									db3.ExecuteSQL(sql2);

									sql.Format("update iptbl set Ok = Ok + 1 , lastusedtime = getdate() where Ipid = %s",IpId);
									db3.ExecuteSQL(sql);
								}

								
							}

						

 
						  }
						  else
						  {
						  
							    //CString sql;
								//sql.Format("insert into iptbl (Ip,Ok) values ('%s',1)",tmpstr.c_str());

								//db3.ExecuteSQL(sql);
							    CRecordset rec3(&db3);

							    CString sql;
								sql.Format("insert into iptbl (Ip,Ok,lastusedtime) values ('%s',1,getdate())",tmpstr.c_str());
								db3.ExecuteSQL(sql);

								sql.Format("select ipid from iptbl where ip = '%s'",tmpstr.c_str());
								if (rec3.Open(CRecordset::forwardOnly,sql))
								{
									if(!rec3.IsEOF())
									{

										CString IpId;
										rec3.GetFieldValue((int)0,IpId);

										CString sql2;
										sql2.Format("insert into ratelog (Ok,DogId,ReportId,Ipid) values (1,%s,%s,%s)",sDogId,sRpid,IpId);
										db3.ExecuteSQL(sql2);



									}
								}

								rec3.Close();

						  
						  }
						}

		  			}

					m_list.clear();

					szResult.ReleaseBuffer(); 
					MailCodec.FreeBase64Decode(debase);

					//---------------------------------------------------
				} 


				CString sql ;
				sql.Format("update report set atversion = 1 where rpid = %s ",sRpid);
				db2.ExecuteSQL(sql);


				rec.MoveNext();
			}
		}


		db.Close();
		db2.Close();
		db3.Close();
		//db4.Close();
		//db5.Close();

}

void CDBserviceDlg::TrimCntrl(char* InStr)
{

	int len = strlen(InStr);
	int Count = 0;
 

	for(int i = 0 ; i < len ; i++)
	{
	
		if ((!iscntrl(InStr[i]) || InStr[i] > 127 || InStr[i] < 0) && InStr[i] != '\'')
		{
				InStr[Count] = InStr[i];
				Count++;		
		}
	
	}


	InStr[Count] = 0;

}

void CDBserviceDlg::GenFile(bool ReGen)
{

		FILE *fp;
		CTime today;
		CString FileNamePath;
		CString FileName;

	
	
		StrList m_list;

	    CDatabase db,db2;
		db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
		db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
	

		if (ReGen)
		{
				
			db2.ExecuteSQL("delete from LiveUpdate where FileName like '%.sdb'");

			 SHFILEOPSTRUCT oFile;
			 
			 memset(&oFile, 0,sizeof(SHFILEOPSTRUCT));
			 oFile.hwnd=this->m_hWnd;
			 oFile.wFunc=FO_DELETE;
			 oFile.pFrom="G:\\*.sdb";	
			 oFile.fFlags= FOF_NOCONFIRMATION;
			 SHFileOperation(&oFile);
		}

		CRecordset rec(&db);

		int HeaderCount = 0;

		CString OkSaveStr;
		int OkCount = 0;
		UpdateHeader m_OKupdateHeader;
		memset(&m_OKupdateHeader,0,sizeof(m_OKupdateHeader));

		CString sql;
		if (ReGen)
		{
		    sql = "select Ip,ipid from iptbl (nolock) where (Ok > Bad  and AtVersion = 1) or (AtVersion=4)";
		}
		else
		{
			sql = "select Ip,ipid from iptbl (nolock) where (Ok > Bad  and AtVersion = 0) or (AtVersion=4)";
		}


		if (rec.Open(CRecordset::forwardOnly,sql))
		{

			CString Ip; 
			CString Ipid;

			while(!rec.IsEOF())
			{
				rec.GetFieldValue((int)0,Ip);
				rec.GetFieldValue((int)1,Ipid);
				
				Ip.TrimLeft();
			    Ip.TrimRight();

				OkSaveStr = OkSaveStr + Ip;
				OkSaveStr = OkSaveStr + char(13);

				OkCount++;
				
				if (!ReGen)
				{

					CString sql;
					sql.Format("update iptbl set AtVersion = 1 where ipid = %s",Ipid);
					db2.ExecuteSQL(sql);
				}

				rec.MoveNext();
			}
		}

				
		
		if (OkCount > 0)
		{
			m_OKupdateHeader.ItemCount = OkCount;
			m_OKupdateHeader.Type = 0 ; //ok
			m_OKupdateHeader.DataLen = strlen(OkSaveStr);
			HeaderCount++;
		}


		///bad
		rec.Close();
		CString BadSaveStr;
		int BadCount = 0;
		UpdateHeader m_BadupdateHeader;
		memset(&m_BadupdateHeader,0,sizeof(m_BadupdateHeader));

		if (ReGen)
		{
		    sql = "select Ip,ipid from iptbl (nolock) where ( Bad > Ok and AtVersion = 1) or (AtVersion=5)";
		}
		else
		{
			sql = "select Ip,ipid from iptbl (nolock) where (Bad > Ok and  AtVersion = 0) or (AtVersion=5)";
		}

		if (rec.Open(CRecordset::forwardOnly,sql))
		{

			CString Ip; 
			CString Ipid;


			while(!rec.IsEOF())
			{
				rec.GetFieldValue((int)0,Ip);
				rec.GetFieldValue((int)1,Ipid);

				
				Ip.TrimLeft();
				Ip.TrimRight();

				BadSaveStr = BadSaveStr + Ip;
				BadSaveStr = BadSaveStr + char(13);

				BadCount++;

				if (!ReGen)
				{

					CString sql;
					sql.Format("update iptbl set AtVersion = 1 where ipid = %s",Ipid);
					db2.ExecuteSQL(sql);
				}

				rec.MoveNext();
			}
		}

				
		

		if (BadCount > 0)
		{
			m_BadupdateHeader.ItemCount = BadCount;
			m_BadupdateHeader.Type = 1 ; //Bad
			m_BadupdateHeader.DataLen = BadSaveStr.GetLength();
			HeaderCount++;
		}


		//Fix unknown
	 
		rec.Close();
		CString FixUnknownSaveStr;
		int FixUnknownCount = 0;
		UpdateHeader m_FixUnknownupdateHeader;
		memset(&m_FixUnknownupdateHeader,0,sizeof(m_FixUnknownupdateHeader));

		if (ReGen)
		{
		    sql = "select Ip,ipid,AtVersion from iptbl (nolock) where AtVersion = 999 or AtVersion = 998";
		}
		else
		{
			sql = "select Ip,ipid,AtVersion from iptbl (nolock) where AtVersion = 6 or AtVersion = 998";
		}


		if (rec.Open(CRecordset::forwardOnly,sql))
		{

			CString Ip; 
			CString Ipid;

			CDBVariant Version;
							
 

			while(!rec.IsEOF())
			{
				rec.GetFieldValue((int)0,Ip);
				rec.GetFieldValue((int)1,Ipid);
				rec.GetFieldValue((int)2,Version,SQL_C_SLONG);

				Ip.TrimLeft();
				Ip.TrimRight();

				FixUnknownSaveStr = FixUnknownSaveStr + Ip;
				FixUnknownSaveStr = FixUnknownSaveStr + char(13);

				FixUnknownCount++;

				if (!ReGen)
				{
		
					CString sql;
					if (Version.m_iVal == 998)
					{
						sql.Format("update iptbl set AtVersion = 999 where ipid = %s",Ipid);
					}
					else
					{
				  		sql.Format("update iptbl set AtVersion = 1 where ipid = %s",Ipid);
					}
				

					db2.ExecuteSQL(sql);
				}

				rec.MoveNext();
			}
		}

				
		

		if (FixUnknownCount > 0)
		{
			m_FixUnknownupdateHeader.ItemCount = FixUnknownCount;
			m_FixUnknownupdateHeader.Type = 2 ; //Fix unknown
			m_FixUnknownupdateHeader.DataLen = FixUnknownSaveStr.GetLength();

			HeaderCount++;
		}

		//gateway
	 
		rec.Close();
		CString GatewaySaveStr;
		int GatewayCount = 0;
		UpdateHeader m_GatewayupdateHeader;
		memset(&m_GatewayupdateHeader,0,sizeof(m_GatewayupdateHeader));

		if (ReGen)
		{
		    sql = "select Ip,ipid,AtVersion from iptbl (nolock) where AtVersion = 8 or AtVersion = 9";
		}
		else
		{
			sql = "select Ip,ipid,AtVersion from iptbl (nolock) where AtVersion = 8 ";
		}


		if (rec.Open(CRecordset::forwardOnly,sql))
		{

			CString Ip; 
			CString Ipid;

			CDBVariant Version;
							
 

			while(!rec.IsEOF())
			{
				rec.GetFieldValue((int)0,Ip);
				rec.GetFieldValue((int)1,Ipid);
				rec.GetFieldValue((int)2,Version,SQL_C_SLONG);

				Ip.TrimLeft();
				Ip.TrimRight();

				GatewaySaveStr = GatewaySaveStr + Ip;
				GatewaySaveStr = GatewaySaveStr + char(13);

				GatewayCount++;

				if (!ReGen)
				{
		
					CString sql;
					
					sql.Format("update iptbl set AtVersion = 9 where ipid = %s",Ipid);
					db2.ExecuteSQL(sql);
				}

				rec.MoveNext();
			}
		}

				
		

		if (GatewayCount > 0)
		{
			m_GatewayupdateHeader.ItemCount = GatewayCount;
			m_GatewayupdateHeader.Type = 3 ; //gateway
			m_GatewayupdateHeader.DataLen = GatewaySaveStr.GetLength();

			HeaderCount++;
		}


		//enews
	 
		rec.Close();
		CString EnewsSaveStr;
		int EnewsCount = 0;
		UpdateHeader m_EnewsupdateHeader;
		memset(&m_EnewsupdateHeader,0,sizeof(m_EnewsupdateHeader));

		if (ReGen)
		{
		    sql = "select Ip,ipid,AtVersion from iptbl (nolock) where AtVersion = 10 or AtVersion = 11";
		}
		else
		{
			sql = "select Ip,ipid,AtVersion from iptbl (nolock) where AtVersion = 10 ";
		}


		if (rec.Open(CRecordset::forwardOnly,sql))
		{

			CString Ip; 
			CString Ipid;

			CDBVariant Version;
							
 

			while(!rec.IsEOF())
			{
				rec.GetFieldValue((int)0,Ip);
				rec.GetFieldValue((int)1,Ipid);
				rec.GetFieldValue((int)2,Version,SQL_C_SLONG);

				Ip.TrimLeft();
				Ip.TrimRight();

				EnewsSaveStr = EnewsSaveStr + Ip;
				EnewsSaveStr = EnewsSaveStr + char(13);

				EnewsCount++;

				if (!ReGen)
				{
		
					CString sql;
					
					sql.Format("update iptbl set AtVersion = 11 where ipid = %s",Ipid);
					db2.ExecuteSQL(sql);
				}

				rec.MoveNext();
			}
		}

				
		

		if (EnewsCount > 0)
		{
			m_EnewsupdateHeader.ItemCount = EnewsCount;
			m_EnewsupdateHeader.Type = 4 ; //Enews
			m_EnewsupdateHeader.DataLen = EnewsSaveStr.GetLength();

			HeaderCount++;
		}


		/*
		//Fix Bad	
		rec.Close();
		CString FixBadSaveStr;
		int FixBadCount = 0;
		UpdateHeader m_FixBadupdateHeader;
		memset(&m_FixBadupdateHeader,0,sizeof(m_FixBadupdateHeader));

		if (rec.Open(CRecordset::forwardOnly,"select Ip from iptbl where AtVersion = 4"))
		{

			CString Ip; 

			Ip.TrimLeft();
			Ip.TrimRight();

			while(!rec.IsEOF())
			{
				rec.GetFieldValue((int)0,Ip);

				FixBadSaveStr = FixBadSaveStr + Ip;
				FixBadSaveStr = FixBadSaveStr + char(13);

				FixBadCount++;

				rec.MoveNext();
			}
		}

				
		

		if (FixBadCount > 0)
		{
			m_FixBadupdateHeader.ItemCount = FixBadCount;
			m_FixBadupdateHeader.Type = 3 ; //Fix Bad
			m_FixBadupdateHeader.DataLen = FixBadSaveStr.GetLength();
			HeaderCount++;
		}*/

		//計算總大小
		int TotalSize = (sizeof(UpdateHeader) * HeaderCount)  + 
						OkSaveStr.GetLength() +
						BadSaveStr.GetLength() +
						FixUnknownSaveStr.GetLength() +
						GatewaySaveStr.GetLength() +
						EnewsSaveStr.GetLength(); //+
						//FixBadSaveStr.GetLength();


		if (TotalSize > 0)
		{

			char *compressbuff = new char [TotalSize];

			int CopySize = 0;
			if (m_OKupdateHeader.ItemCount > 0)
			{
			
				memcpy(compressbuff,&m_OKupdateHeader , sizeof(UpdateHeader));
				CopySize = sizeof(UpdateHeader);
			
				memcpy(compressbuff+CopySize,OkSaveStr.GetBuffer(OkSaveStr.GetLength()),OkSaveStr.GetLength());
				CopySize = CopySize + OkSaveStr.GetLength();
			}

			if (m_BadupdateHeader.ItemCount > 0)
			{
			
				memcpy(compressbuff+CopySize,&m_BadupdateHeader , sizeof(UpdateHeader));
				CopySize = CopySize + sizeof(UpdateHeader);
			
				memcpy(compressbuff+CopySize,BadSaveStr.GetBuffer(BadSaveStr.GetLength()),BadSaveStr.GetLength());
				CopySize = CopySize + BadSaveStr.GetLength();
			}

			if (m_FixUnknownupdateHeader.ItemCount > 0)
			{
			
				memcpy(compressbuff+CopySize,&m_FixUnknownupdateHeader , sizeof(UpdateHeader));
				CopySize = CopySize + sizeof(UpdateHeader);
			
				memcpy(compressbuff+CopySize,FixUnknownSaveStr.GetBuffer(FixUnknownSaveStr.GetLength()),FixUnknownSaveStr.GetLength());
				CopySize = CopySize + FixUnknownSaveStr.GetLength();
			}


			if (m_GatewayupdateHeader.ItemCount > 0)
			{
			
				memcpy(compressbuff+CopySize,&m_GatewayupdateHeader , sizeof(UpdateHeader));
				CopySize = CopySize + sizeof(UpdateHeader);
			
				memcpy(compressbuff+CopySize,GatewaySaveStr.GetBuffer(GatewaySaveStr.GetLength()),GatewaySaveStr.GetLength());
				CopySize = CopySize + GatewaySaveStr.GetLength();
			}

			if (m_EnewsupdateHeader.ItemCount > 0)
			{
			
				memcpy(compressbuff+CopySize,&m_EnewsupdateHeader , sizeof(UpdateHeader));
				CopySize = CopySize + sizeof(UpdateHeader);
			
				memcpy(compressbuff+CopySize,EnewsSaveStr.GetBuffer(EnewsSaveStr.GetLength()),EnewsSaveStr.GetLength());
				CopySize = CopySize + EnewsSaveStr.GetLength();
			}


		/*
		if (m_FixBadupdateHeader.ItemCount > 0)
		{
			
			memcpy(compressbuff+CopySize,&m_FixBadupdateHeader , sizeof(UpdateHeader));
			CopySize = CopySize + sizeof(UpdateHeader);
			
			memcpy(compressbuff+CopySize,FixBadSaveStr.GetBuffer(FixBadSaveStr.GetLength()),FixBadSaveStr.GetLength());
			CopySize = CopySize + FixBadSaveStr.GetLength();
		}*/


			CCompress cc;
			ZipHeader zipheader;
			memset(&zipheader,0,sizeof(zipheader));		
	 
			unsigned int outsize = cc.EstimateOutBuffSize(TotalSize);
			char *outbuff = new char [outsize];
			memset(outbuff,0,outsize);

			FileName.Format("%s.sdb",today.GetCurrentTime().Format("%Y%m%d%H%M%S"));
	 
			//FileNamePath = "C:\\webroot\\softworking\\update\\" + FileName;
			FileNamePath = "G:\\" + FileName;
			fp = fopen(FileNamePath,"w+b");
			

			HRESULT res;
			if ( (res = cc.DoCompress((unsigned char*)compressbuff,TotalSize,(unsigned char *) outbuff,&outsize)) == S_OK)
			{
				zipheader.isCompress = true;
				zipheader.OrgLen = TotalSize;
				zipheader.ThisLen = outsize;

				fwrite(&zipheader,1,sizeof(zipheader),fp);				
				fwrite(outbuff,1,outsize,fp);
			//在加上是否壓縮,及原始長度

			} 
			else if (res == NO_NEED_COMPRESS)
			{
				zipheader.isCompress = false; 
				zipheader.OrgLen = TotalSize;
				zipheader.ThisLen = TotalSize;

				fwrite(&zipheader,1,sizeof(zipheader),fp);
				fwrite(compressbuff,1,TotalSize,fp);

 
			}

			


			
			

			fclose(fp);


			delete compressbuff;
			delete outbuff;


		

			CString sql;
			sql.Format("insert into LiveUpdate (FileName) values ('%s') ", FileName);
			db2.ExecuteSQL(sql);
	 

		}

		db.Close();
		db2.Close();


			

}

void CDBserviceDlg::IpReserve(char *IP)
{

		int len = strlen(IP);
		char Zone1[4];
		char Zone2[4];
		char Zone3[4];
		char Zone4[4];

		memset(Zone1,0,4);
		memset(Zone2,0,4);
		memset(Zone3,0,4);
		memset(Zone4,0,4);

		int ZoneIdx = 0;
		int SavePos = 0;

		for(int i= 0 ; i < len ; i++)
		{
			if (IP[i] == '.')
			{
			
				if (ZoneIdx == 0)
				{					
					strncpy(Zone4,IP + SavePos, i - SavePos);
					//ZoneIdx ++;
				}
				else if (ZoneIdx == 1)
				{
					strncpy(Zone3,IP + SavePos, i - SavePos);
					//ZoneIdx ++;
				}
				else if (ZoneIdx == 2)
				{
					strncpy(Zone2,IP + SavePos, i - SavePos);
					strncpy(Zone1,IP + i + 1, len-i);
					 
					break;
				}
			 

				ZoneIdx ++;
				SavePos = i + 1;

			}
		}

		strcpy(IP,Zone1);
		strcat(IP,".");
		strcat(IP,Zone2);
		strcat(IP,".");
		strcat(IP,Zone3);
		strcat(IP,".");
		strcat(IP,Zone4);
	 

}

void CDBserviceDlg::GenSysMail()
{

		FILE *fp;
		CTime today;
		CString FileName;
	 
		
		fp = fopen("D:\\SoftWorking\\SoftWorking\\SpamDog\\ok.eml","r+b"); 
		
		unsigned long TotalSize=0;
		fseek(fp,0,SEEK_END);
		TotalSize = ftell(fp);
		fseek(fp,0,SEEK_SET);
	 
		char *compressbuff = new char [TotalSize];
		fread(compressbuff,1,TotalSize,fp);

	 
		CCompress cc;
		ZipHeader zipheader;
		memset(&zipheader,0,sizeof(zipheader));		
	 
		unsigned int outsize = cc.EstimateOutBuffSize(TotalSize);
		char *outbuff = new char [outsize];
		memset(outbuff,0,outsize);

		HRESULT res;
		if ( (res = cc.DoCompress((unsigned char*)compressbuff,TotalSize,(unsigned char *) outbuff,&outsize)) == S_OK)
		{
			zipheader.isCompress = true;
			zipheader.OrgLen = TotalSize;
			zipheader.ThisLen = outsize;	
			//在加上是否壓縮,及原始長度

		} 
		else if (res == NO_NEED_COMPRESS)
		{
			zipheader.isCompress = false; 
			zipheader.OrgLen = TotalSize;
			zipheader.ThisLen = TotalSize;

 
		}



		CMailCodec MailCodec;
		char *EncodeStr = MailCodec.Base64Encode2(outbuff,outsize);


		FILE *fp2 = fopen("D:\\SoftWorking\\SoftWorking\\SpamDog\\20050712.eml","w+b");
		//fwrite(&zipheader,1,sizeof(zipheader),fp2);
		fwrite(EncodeStr,1,strlen(EncodeStr),fp2);
		fclose(fp2);

		MailCodec.FreeBase64Encode(EncodeStr);  

	 

		fclose(fp);

		delete compressbuff;
		delete outbuff;

	 


			

}

void CDBserviceDlg::OnButton1() 
{
	ConvertReport();	
	AfxMessageBox("ok");
}

void CDBserviceDlg::OnButton2() 
{
    ConvertMail();
	AfxMessageBox("ok");
	
}

void CDBserviceDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//KillTimer(nIDEvent);

	m_STATICTIME.SetWindowText(CTime::GetCurrentTime().Format("%H:%M:%S"));
	
	
	//ConvertMail();
	//ConvertReport(); //一定要在 convvert mail 後
	//GenFile();
	//GetHostName();
	//GetReportMail();



	//SetTimer(1,1000 * 60 * 1,NULL); //1 min

	WinExec("c:\\SpamDogService.exe",SW_SHOW);

	
	if (nIDEvent > 0)
	CDialog::OnTimer(nIDEvent);
}

void CDBserviceDlg::OnButton3() 
{
    this->GenFile();	

	AfxMessageBox("ok");
}

void CDBserviceDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
  this->GenSysMail();
  AfxMessageBox("ok");
}

void CDBserviceDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	CoUninitialize();
	// TODO: Add your message handler code here
	
}

void CDBserviceDlg::MergeKeyFile()
{
	
		CDatabase db,db2;
		db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
		db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");


		

		bool HasOk = false;
		bool HasBad = false;
		bool HasUnknown = false;
		bool HasGateway = false;
		bool HasEnews = false;

		int HeaderCount = 0;

		CString OkSaveStr;
		//int OkCount = 0;
		UpdateHeader m_OKupdateHeader;
		memset(&m_OKupdateHeader,0,sizeof(m_OKupdateHeader));
		m_OKupdateHeader.Type = 0;

		CString BadSaveStr;
		//int BadCount = 0;
		UpdateHeader m_BadupdateHeader;
		memset(&m_BadupdateHeader,0,sizeof(m_BadupdateHeader));
		m_BadupdateHeader.Type = 1;

		CString FixUnknownSaveStr;
		//int FixUnknownCount = 0;
		UpdateHeader m_FixUnknownupdateHeader;
		memset(&m_FixUnknownupdateHeader,0,sizeof(m_FixUnknownupdateHeader));
		m_FixUnknownupdateHeader.Type = 2;

		CString GatewaySaveStr;		
		UpdateHeader m_GatewayupdateHeader;
		memset(&m_GatewayupdateHeader,0,sizeof(m_GatewayupdateHeader));
		m_GatewayupdateHeader.Type = 3;

		CString EnewsSaveStr;		
		UpdateHeader m_EnewsupdateHeader;
		memset(&m_EnewsupdateHeader,0,sizeof(m_EnewsupdateHeader));
		m_GatewayupdateHeader.Type = 4;


		CRecordset rec(&db);

		CString UpdateNo;
		CString UpFileName;	
		
		bool NeedMerge = false;

		if (rec.Open(CRecordset::forwardOnly,"select count(*) from liveupdate where FileName like '%.sdb' and addtime < CONVERT(CHAR(12),  getdate() - 1, 111) ")) //保留今天跟昨天
		{
			CString selectcount;
			int selcount = 0;

			if (!rec.IsEOF())
			{
				rec.GetFieldValue((int)0,selectcount);
				selcount = atoi(selectcount);

				if (selcount > 1) NeedMerge = true;
			}
		}

		rec.Close();

		if (NeedMerge && rec.Open(CRecordset::forwardOnly,"select UpdateNo,FileName from liveupdate where FileName like '%.sdb' where addtime < CONVERT(CHAR(12),  getdate() -1 , 111) order by UpdateNo"))
		{

		 
		
	
			while(!rec.IsEOF())
			{
			

				rec.GetFieldValue((int)0,UpdateNo);
				rec.GetFieldValue((int)1,UpFileName);

			 

				FILE *fp = fopen("Z:\\" + UpFileName, "rb");
				if (fp != NULL)
				{
	
					int readbyte = 0;		 
					ZipHeader zipheader;
					fread(&zipheader,1,sizeof(zipheader),fp);

					if (zipheader.OrgLen > 0)
					{

						char* DataBuffer = new char[zipheader.OrgLen];		  
						int BufferSize=0;
		 
		 
						if (zipheader.isCompress)
						{
		 
						
							CCompress pzip;				

							char *DataTemp = new char[zipheader.ThisLen];
							int readsize = fread(DataTemp,1,zipheader.ThisLen,fp);

							if (readsize  == zipheader.ThisLen)
							{
								pzip.DeCompress((unsigned char *) DataTemp,zipheader.ThisLen,(unsigned char *)DataBuffer,&zipheader.OrgLen);				  
							}

							BufferSize = zipheader.OrgLen;							
							delete DataTemp;

				
						}
						else
						{

							int readsize = fread(DataBuffer,1,zipheader.OrgLen,fp);
							if (readsize  == zipheader.ThisLen)
							{							
								BufferSize = readsize;						 
							}
		 
						}

						if (BufferSize > 0)
						{
		 
		 
							int ReadPos = 0;		 

							while(ReadPos < BufferSize)
							{
								UpdateHeader updateheader;
								memcpy(&updateheader , DataBuffer + ReadPos , sizeof(UpdateHeader));
								ReadPos += sizeof(UpdateHeader);

								if (updateheader.Type == 0) //ok
								{
									if (!HasOk)
									  HeaderCount++;

									HasOk = true;
									m_OKupdateHeader.ItemCount += updateheader.ItemCount;
									m_OKupdateHeader.DataLen += updateheader.DataLen;

									char *tempchar = new char[updateheader.DataLen+1];
									memcpy(tempchar,DataBuffer + ReadPos,updateheader.DataLen);
									tempchar[updateheader.DataLen] = 0;

									OkSaveStr += tempchar;

									delete tempchar;

									
								} 
								else if (updateheader.Type == 1) //bad
								{
									if (!HasBad)
									  HeaderCount++;

									HasBad = true;
									m_BadupdateHeader.ItemCount += updateheader.ItemCount;
									m_BadupdateHeader.DataLen += updateheader.DataLen;

									char *tempchar = new char[updateheader.DataLen+1];
									memcpy(tempchar,DataBuffer + ReadPos,updateheader.DataLen);
									tempchar[updateheader.DataLen] = 0;

									BadSaveStr += tempchar;

									delete tempchar;

								 
								
								}
								else if (updateheader.Type == 2) //unknown
								{
									if (!HasUnknown)
									  HeaderCount++;

									HasUnknown = true;
									
									m_FixUnknownupdateHeader.ItemCount += updateheader.ItemCount;
									m_FixUnknownupdateHeader.DataLen += updateheader.DataLen;

									char *tempchar = new char[updateheader.DataLen+1];
									memcpy(tempchar,DataBuffer + ReadPos,updateheader.DataLen);
									tempchar[updateheader.DataLen] = 0;

									FixUnknownSaveStr += tempchar;

									delete tempchar;

									 
								
								}
								else if (updateheader.Type == 3) //gateway
								{
									if (!HasGateway)
									  HeaderCount++;

									HasGateway = true;
									
									m_GatewayupdateHeader.ItemCount += updateheader.ItemCount;
									m_GatewayupdateHeader.DataLen += updateheader.DataLen;

									char *tempchar = new char[updateheader.DataLen+1];
									memcpy(tempchar,DataBuffer + ReadPos,updateheader.DataLen);
									tempchar[updateheader.DataLen] = 0;

									GatewaySaveStr += tempchar;

									delete tempchar;

								}
								else if (updateheader.Type == 4) //Enews
								{
									if (!HasEnews)
									  HeaderCount++;

									HasEnews = true;
									
									m_EnewsupdateHeader.ItemCount += updateheader.ItemCount;
									m_EnewsupdateHeader.DataLen += updateheader.DataLen;

									char *tempchar = new char[updateheader.DataLen+1];
									memcpy(tempchar,DataBuffer + ReadPos,updateheader.DataLen);
									tempchar[updateheader.DataLen] = 0;

									EnewsSaveStr += tempchar;

									delete tempchar;

								}


				 
							    
								
							    //分到新位置
								ReadPos = updateheader.DataLen + ReadPos;

				
			    
							}

	 
		 
						}

		                delete DataBuffer;
	
					}
				
					fclose(fp);
					DeleteFile("Z:\\" + UpFileName);


				}			 
	
				rec.MoveNext();

			}
		}


		
		//int HeaderCount = 0;

		 
		   	 

			//計算總大小
			int TotalSize = (sizeof(UpdateHeader) * HeaderCount)  + 
						OkSaveStr.GetLength() +
						BadSaveStr.GetLength() +
						FixUnknownSaveStr.GetLength() +
						GatewaySaveStr.GetLength() + 
						EnewsSaveStr.GetLength(); //+
						//FixBadSaveStr.GetLength();


			if (TotalSize > 0)
			{

				char *compressbuff = new char [TotalSize];

				int CopySize = 0;
				if (m_OKupdateHeader.ItemCount > 0)
				{
			
					memcpy(compressbuff,&m_OKupdateHeader , sizeof(UpdateHeader));
					CopySize = sizeof(UpdateHeader);
			
					memcpy(compressbuff+CopySize,OkSaveStr.GetBuffer(OkSaveStr.GetLength()),OkSaveStr.GetLength());
					CopySize = CopySize + OkSaveStr.GetLength();
				}

				if (m_BadupdateHeader.ItemCount > 0)
				{
			
					memcpy(compressbuff+CopySize,&m_BadupdateHeader , sizeof(UpdateHeader));
					CopySize = CopySize + sizeof(UpdateHeader);
			
					memcpy(compressbuff+CopySize,BadSaveStr.GetBuffer(BadSaveStr.GetLength()),BadSaveStr.GetLength());
					CopySize = CopySize + BadSaveStr.GetLength();
				}

				if (m_FixUnknownupdateHeader.ItemCount > 0)
				{
			
					memcpy(compressbuff+CopySize,&m_FixUnknownupdateHeader , sizeof(UpdateHeader));
					CopySize = CopySize + sizeof(UpdateHeader);
			
					memcpy(compressbuff+CopySize,FixUnknownSaveStr.GetBuffer(FixUnknownSaveStr.GetLength()),FixUnknownSaveStr.GetLength());
					CopySize = CopySize + FixUnknownSaveStr.GetLength();
				}	 

				if (m_GatewayupdateHeader.ItemCount > 0)
				{
			
					memcpy(compressbuff+CopySize,&m_GatewayupdateHeader , sizeof(UpdateHeader));
					CopySize = CopySize + sizeof(UpdateHeader);
			
					memcpy(compressbuff+CopySize,GatewaySaveStr.GetBuffer(GatewaySaveStr.GetLength()),GatewaySaveStr.GetLength());
					CopySize = CopySize + GatewaySaveStr.GetLength();
				}
				
				if (m_EnewsupdateHeader.ItemCount > 0)
				{
			
					memcpy(compressbuff+CopySize,&m_EnewsupdateHeader , sizeof(UpdateHeader));
					CopySize = CopySize + sizeof(UpdateHeader);
			
					memcpy(compressbuff+CopySize,EnewsSaveStr.GetBuffer(EnewsSaveStr.GetLength()),EnewsSaveStr.GetLength());
					CopySize = CopySize + EnewsSaveStr.GetLength();
				}


					CCompress cc;
					ZipHeader zipheader;
					memset(&zipheader,0,sizeof(zipheader));		
	 
					unsigned int outsize = cc.EstimateOutBuffSize(TotalSize);
					char *outbuff = new char [outsize];
					memset(outbuff,0,outsize);

					CString FileName;
					CString FileNamePath;
					CTime today;

					FileName.Format("%s.sdb",today.GetCurrentTime().Format("%Y%m%d%H%M%S"));
	 
			 
					FileNamePath = "Z:\\" + FileName;
					FILE *fp = fopen(FileNamePath,"w+b");
			

					HRESULT res;
					if ( (res = cc.DoCompress((unsigned char*)compressbuff,TotalSize,(unsigned char *) outbuff,&outsize)) == S_OK)
					{	
						zipheader.isCompress = true;
						zipheader.OrgLen = TotalSize;
						zipheader.ThisLen = outsize;

						fwrite(&zipheader,1,sizeof(zipheader),fp);				
						fwrite(outbuff,1,outsize,fp);
						//在加上是否壓縮,及原始長度

					} 
					else if (res == NO_NEED_COMPRESS)
					{
						zipheader.isCompress = false; 
						zipheader.OrgLen = TotalSize;
						zipheader.ThisLen = TotalSize;

						fwrite(&zipheader,1,sizeof(zipheader),fp);
						fwrite(compressbuff,1,TotalSize,fp);

 
					} 
			

					fclose(fp);


					delete compressbuff;
					delete outbuff;

					CString SQL;
					SQL.Format("update LiveUpdate set FileName='%s' where updateno=%s",FileName,UpdateNo);
					db2.ExecuteSQL(SQL);
					SQL.Format("delete from LiveUpdate where updateno < %s and FileName like '%c.sdb'",UpdateNo,'%');
					db2.ExecuteSQL(SQL);
			}


		db.Close();
		db2.Close();

	//select * from liveupdate where addtime < getdate() - 3

	//open File

}

void CDBserviceDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	OnTimer(0); 
	SetTimer(1,1000 * 60 * 1,NULL); //1 min
}

void CDBserviceDlg::OnButton5() 
{
		MergeKeyFile();	
		AfxMessageBox("ok");
}

void CDBserviceDlg::UnZipFileMail(char *FilePath, int orglen)
{

	FILE *fp = fopen(FilePath,"r+b");
	if (fp != NULL)
	{
	
		fseek(fp,0,SEEK_END);
		int FileSize = ftell(fp);
		fseek(fp,0,SEEK_SET);

		char *buff = new char[FileSize];

		fread(buff,1,FileSize,fp);

		CMailCodec MailCodec;

		char *outputbuff;
	    MailCodec.Base64Decode2(buff,&outputbuff,(unsigned int *) &FileSize);
		
		 

		

		CCompress cc;

		char *outbuff = new char[orglen];

			cc.DeCompress((unsigned char*)outputbuff,FileSize,(unsigned char*)outbuff,(unsigned int *)&FileSize);

			FILE *fp2 = fopen("c:\\demail.eml","w+b");
			fwrite(outbuff,1,FileSize,fp2);
			fclose(fp2);


		fclose(fp);

		MailCodec.FreeBase64Decode(outputbuff);

		delete outbuff;
		delete buff;

	
	}

}

void CDBserviceDlg::UnZipFile(char *FilePath,char *DestPath , int orglen)
{

	FILE *fp = fopen(FilePath,"r+b");
	if (fp != NULL)
	{
	
		fseek(fp,0,SEEK_END);
		int FileSize = ftell(fp);
		fseek(fp,0,SEEK_SET);

		char *buff = new char[FileSize];

		fread(buff,1,FileSize,fp);

		CMailCodec MailCodec;

		char *outputbuff;
	    MailCodec.Base64Decode2(buff,&outputbuff,(unsigned int *) &FileSize);
		
		 

		

		CCompress cc;

		char *outbuff = new char[orglen];

			cc.DeCompress((unsigned char*)outputbuff,FileSize,(unsigned char*)outbuff,(unsigned int *)&FileSize);

			FILE *fp2 = fopen(DestPath,"w+b");
			fwrite(outbuff,1,FileSize,fp2);
			fclose(fp2);


		fclose(fp);

		MailCodec.FreeBase64Decode(outputbuff);

		delete outbuff;
		delete buff;

	
	}

}



void CDBserviceDlg::GetHostName()
{
		CDatabase db,db2;
		db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
		db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

		CCoreSocket::Startup();

		CRecordset rec(&db);
		if (rec.Open(CRecordset::forwardOnly,"select SubId,Ip from subtbl (nolock) where status = 0 "))
		{
		
			CString SubId,Ip;

			while(!rec.IsEOF())
			{
			

				rec.GetFieldValue((int)0,SubId);			
				rec.GetFieldValue((int)1,Ip);

				Ip.TrimLeft();
				Ip.TrimRight();


				CDNSclient dns;

				HANDLE ch = dns.Resolve("168.95.1.1",(char *)Ip.GetBuffer(Ip.GetLength()),qtPTR);	 
				WaitForSingleObject(ch,INFINITE);	
				//strcpy(Result,dns.GetQueryResult()); 

				CString sql;
				sql.Format("update subtbl set status = 1 , HostName = '%s' where subid=%s",dns.GetQueryResult() ,SubId);
				db2.ExecuteSQL(sql);

				//AfxMessageBox(dns.GetQueryResult());

				Ip.ReleaseBuffer();

					
				


				rec.MoveNext();
				
			}
		
		}



		CCoreSocket::Cleanup();
		db.Close();
		db2.Close();
}

void CDBserviceDlg::GetReportMail()
{
		CDatabase db,db2;
		db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
		db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");

		CCoreSocket::Startup();

		CRecordset rec(&db);
		if (rec.Open(CRecordset::forwardOnly,"select top 50  Ipid,Ip from iptbl  (nolock) where  bad > 10 and  reportmail is null  order by bad desc "))
		{
		
			CString IpId,Ip;

			while(!rec.IsEOF())
			{
			

				rec.GetFieldValue((int)0,IpId);			
				rec.GetFieldValue((int)1,Ip);

				Ip.TrimLeft();
				Ip.TrimRight();

				CReportMail rm;

				rm.SetIp(Ip.GetBuffer(Ip.GetLength()));
				Ip.ReleaseBuffer();

				//_cprintf("%s\r\n",rm.GetContact());

			 

				CString sql;

				char *email = rm.GetSpamCop();
				_cprintf("%s\r\n",email);

				if (strlen(email) > 0)
				{
					
					sql.Format("update iptbl set ReportMail = '%s' where ipid=%s",email,IpId);
					db2.ExecuteSQL(sql);
				}

				//AfxMessageBox(dns.GetQueryResult());

			 

					
				


				rec.MoveNext();
				
			}
		
		}



		CCoreSocket::Cleanup();
		db.Close();
		db2.Close();
}


void CDBserviceDlg::OnButton7() 
{
	UnZipFileMail("c:\\cmpmail.txt",35474 * 10);
	AfxMessageBox("ok");
}

void CDBserviceDlg::OnButton8() 
{
	GetHostName();

	AfxMessageBox("ok");
       	
}


void CReportMail::SetIp(char *ip)
{
	strcpy(IP,ip);

	int iplen = strlen(ip);
	int zonecount = 0;
	int SavePos = 0;

	for (int i = 0 ; i <= iplen ; i++)
	{
		if (ip[i] == '.' || i == iplen)
		{
		   memset(IPs[zonecount],0,4);
		   memcpy(IPs[zonecount],ip+SavePos,i-SavePos);

		   iIPs[zonecount][0] = atoi(IPs[zonecount]);

		   zonecount ++;
		   SavePos = i+1;
		}
	
	}
}

int CReportMail::GetNic(char *ANic)
{

	ANic[0] = 0;
	//apnic 
	if ((iIPs[0][0] >= 58 && iIPs[0][0] <= 61) ||
		(iIPs[0][0] >= 124 && iIPs[0][0] <= 126) ||		
		(iIPs[0][0] >= 202 && iIPs[0][0] <= 203) ||
		(iIPs[0][0] >= 210 && iIPs[0][0] <= 211) ||
		(iIPs[0][0] >= 218 && iIPs[0][0] <= 222) ||
		iIPs[0][0] == 168)
	{
	
		strcpy(ANic,"202.12.29.13");
		return APNIC;

	}
	//arin
	else if ((iIPs[0][0] >= 63 && iIPs[0][0] <= 73) ||
		     (iIPs[0][0] == 199) ||
			 (iIPs[0][0] >= 204 && iIPs[0][0] <= 209) ||
			 (iIPs[0][0] == 216) )
	{
	
		strcpy(ANic,"192.149.252.44");
		return ARIN;
	}
	//RIPE
	else if ((iIPs[0][0] == 62) ||
		     (iIPs[0][0] >= 80 && iIPs[0][0] <= 88) ||
			 (iIPs[0][0] >= 193 && iIPs[0][0] <= 195) ||
			 (iIPs[0][0] >= 212 && iIPs[0][0] <= 213) ||
			 (iIPs[0][0] == 217) )
	{
	
		strcpy(ANic,"193.0.0.135");
		return RIPE;
	}
	//LACNIC
	else if ((iIPs[0][0] >= 200 && iIPs[0][0] <= 201)  )
	{
	
		strcpy(ANic,"200.160.2.15");
		return LACNIC;
	}
	//AFRINIC
	else if (iIPs[0][0] == 41 )//|| iIPs[0][0] == 212))
	{
	
		strcpy(ANic,"196.216.2.1");
		return AFRINIC;
	}


return 0;
}

void CReportMail::ParseAPNIC(char *WhoisStr)
{

	int len = strlen(WhoisStr);
	int line = 0;
	int step = 0;
	char Email[200];
	int EmailCount = 0;

	memset(Email,0,200);

	for (int i = 0 ; i < len ; i++)
	{
	
		if (step == 0 && WhoisStr[i] == 10 && WhoisStr[i+1] == 10 )//&&
			//WhoisStr[i+2] == 13 && WhoisStr[i+3] == 10)
		{
			 
				
			line ++;

			if (line == 2)
			{
				//memmove(WhoisStr,WhoisStr+i+2,len-i);
				//break;
				step = 1;
				i = i + 3;


			}
		
		} 
		else if (step == 1)
		{
		
			if (strnicmp(WhoisStr+i,"e-mail:",7) == 0 || strnicmp(WhoisStr+i,"mailbox:",8) == 0 )
			{
			
				step = 2;
				i = i + 7;

			}
			
		}
		else if (step == 2)
		{
			if (WhoisStr[i] != 32 && WhoisStr[i] != 10)
			{
				Email[EmailCount] = WhoisStr[i];
				EmailCount ++;
				
			}
			else if (EmailCount > 0)
			{
				break;
			}
		
		}

	}

	//_cprintf("%s\r\n",Email);
	TrimAt(Email);
	strcpy(contact,Email);

}

void CReportMail::ParseLACNIC(char *WhoisStr)
{

	int len = strlen(WhoisStr);
	int line = 0;
	int step = 0;
	char Email[200];
	int EmailCount = 0;

	memset(Email,0,200);

	for (int i = 0 ; i < len ; i++)
	{
	
		if (step == 0 && WhoisStr[i] == 13 && WhoisStr[i+1] == 10 &&
			WhoisStr[i+2] == 13 && WhoisStr[i+3] == 10)
		{
			 
				
			line ++;

			if (line == 2)
			{
				//memmove(WhoisStr,WhoisStr+i+2,len-i);
				//break;
				step = 1;
				i = i + 3;


			}
		
		} 
		else if (step == 1)
		{
		
			if (strnicmp(WhoisStr+i,"e-mail:",7) == 0 )
			{
			
				step = 2;
				i = i + 7;

			}
			
		}
		else if (step == 2)
		{
			if (WhoisStr[i] != 32 && WhoisStr[i] != 13)
			{
				Email[EmailCount] = WhoisStr[i];
				EmailCount ++;
				
			}
			else if (EmailCount > 0)
			{
				break;
			}
		
		}

	}

	//_cprintf("%s\r\n",Email);
	TrimAt(Email);
	strcpy(contact,Email);

}


void CReportMail::ParseARIN(char *WhoisStr)
{

	int len = strlen(WhoisStr);
	int line = 0;
	int step = 0;
	char Email[200];
	int EmailCount = 0;

	memset(Email,0,200);

	for (int i = 0 ; i < len ; i++)
	{
	
		if (step == 0 && WhoisStr[i] == 10 && WhoisStr[i+1] == 10 )//&&
			//WhoisStr[i+2] == 13 && WhoisStr[i+3] == 10)
		{
			 
				
			//line ++;

			//if (line == 2)
			//{
				//memmove(WhoisStr,WhoisStr+i+2,len-i);
				//break;
				step = 1;
				i = i + 3;


			//}
		
		} 
		else if (step == 1)
		{
		
			if (strnicmp(WhoisStr+i,"Email:",6) == 0)
			{
			
				step = 2;
				i = i + 7;

			}
			
		}
		else if (step == 2)
		{
			if (WhoisStr[i] != 32 && WhoisStr[i] != 10)
			{
				Email[EmailCount] = WhoisStr[i];
				EmailCount ++;
				
			}
			else if (EmailCount > 0)
			{
				Email[EmailCount] = 0;
				//_cprintf("%s\r\n",Email);

				break;

				EmailCount = 0;
				step = 0;
				i --;

			}
		
		}

	}


	TrimAt(Email);
	strcpy(contact,Email);

}
/*
void CReportMail::ParseRIPE(char *WhoisStr)
{

	int len = strlen(WhoisStr);
	int line = 0;
	int step = 0;
	char Email[200];
	int EmailCount = 0;

	memset(Email,0,200);

	for (int i = 0 ; i < len ; i++)
	{
	
		if (step == 0 && WhoisStr[i] == 10 && WhoisStr[i+1] == 10 )//&&
			//WhoisStr[i+2] == 13 && WhoisStr[i+3] == 10)
		{
			 
				
			//line ++;

			//if (line == 2)
			//{
				//memmove(WhoisStr,WhoisStr+i+2,len-i);
				//break;
				step = 1;
				i = i + 3;


			//}
		
		} 
		else if (step == 1)
		{
		
			if (strnicmp(WhoisStr+i,"Email:",6) == 0)
			{
			
				step = 2;
				i = i + 7;

			}
			
		}
		else if (step == 2)
		{
			if (WhoisStr[i] != 32 && WhoisStr[i] != 10)
			{
				Email[EmailCount] = WhoisStr[i];
				EmailCount ++;
				
			}
			else if (EmailCount > 0)
			{
				Email[EmailCount] = 0;
				_cprintf("%s\r\n",Email);

				break;

				EmailCount = 0;
				step = 0;
				i --;

			}
		
		}

	}



}
*/
CReportMail::CReportMail()
{
	memset(contact,0,255);
}

void CReportMail::TrimResponse(char *AStr)
{
	char ContactMail[5][200]; 
	char AbuseMail[200];
	char OtherMail[200];

	AbuseMail[0] = 0;
	OtherMail[0] = 0;	 

	int SavePos = 0;
	int MailCount = 0;
	int step = 0;

	int len = strlen(AStr);
	for (int i = 0 ; i < len ; i ++)
	{
	
		if (step == 0 && strnicmp(AStr+i,"Reporting addresses:",20) == 0) //先找 Reporting addresses:
		{
			step = 1;
			i += 19;
			/*
			if (MailCount < 5)
			{
				memset(ContactMail[MailCount],0,200);
				strncpy(ContactMail[MailCount],AStr+SavePos,i-SavePos);
				step = 1;
				MailCount ++;

			} */
		
		} else if(step == 1 &&  strnicmp(AStr+i,"mailto:",7) == 0 )  //找 mailto:
		{
		
			SavePos = i + 7;
			i += 6;
			step = 2;
		
		}
		else if (step == 2 && AStr[i] == '"') //找 " 
		{
			if (MailCount < 5)
			{
				memset(ContactMail[MailCount],0,200);
				strncpy(ContactMail[MailCount],AStr+SavePos,i-SavePos);
				step = 1;
				MailCount ++;

			} 
		
		}
		 
	}


	if (MailCount > 0)
	{
	
		for (int i = 0 ; i < MailCount ; i++)
		{
			int len = strlen(ContactMail[i]);
			for (int j = 0 ; j < len ; j++)
			{
				if (strnicmp((ContactMail[i])+j,"spam",4) == 0)
				{
					strcpy(AStr,ContactMail[i]);
					return;
				} 
				else if (strnicmp((ContactMail[i])+j,"abuse",4) == 0)
				{

					strcpy(AbuseMail,ContactMail[i]);
				}
				else 
				{
					strcpy(OtherMail,ContactMail[i]);
				}
			}
		
		}
	
	}


	if (AbuseMail[0] != 0)
	{
		strcpy(AStr,AbuseMail);		
	}
	else if (OtherMail[0] != 0)
	{
	    strcpy(AStr,OtherMail);	
	}
	else
	{
		AStr[0] = 0;
	}

}

void CReportMail::TrimContact(char *AStr)
{

	char ContactMail[5][200]; 
	char AbuseMail[200];
	char OtherMail[200];

	AbuseMail[0] = 0;
	OtherMail[0] = 0;	 

	int SavePos = 0;
	int MailCount = 0;
	int step = 0;

	int len = strlen(AStr);
	for (int i = 0 ; i < len ; i ++)
	{
	
		if (step == 0 && AStr[i] == 32)
		{
			if (MailCount < 5)
			{
				memset(ContactMail[MailCount],0,200);
				strncpy(ContactMail[MailCount],AStr+SavePos,i-SavePos);
				step = 1;
				MailCount ++;

			} 
		
		} else if(step == 1 && AStr[i] == 10)
		{
		
			SavePos =  i + 1;
			step = 0;

		}
	}


	if (MailCount > 0)
	{
	
		for (int i = 0 ; i < MailCount ; i++)
		{
			int len = strlen(ContactMail[i]);
			for (int j = 0 ; j < len ; j++)
			{
				if (strnicmp((ContactMail[i])+j,"spam",4) == 0)
				{
					strcpy(AStr,ContactMail[i]);
					return;
				} 
				else if (strnicmp((ContactMail[i])+j,"abuse",4) == 0)
				{

					strcpy(AbuseMail,ContactMail[i]);
				}
				else 
				{
					strcpy(OtherMail,ContactMail[i]);
				}
			}
		
		}
	
	}


	if (AbuseMail[0] != 0)
	{
		strcpy(AStr,AbuseMail);		
	}
	else if (OtherMail[0] != 0)
	{
	    strcpy(AStr,OtherMail);	
	}
	else
	{
		AStr[0] = 0;
	}

}
void CReportMail::TrimAt(char *AStr)
{

	int len = strlen(AStr);
	for (int i = 0 ; i < len ; i ++)
	{
	
		if (AStr[i] == '@')
		{
		
			strcpy(AStr,AStr+i+1);
			return;
		
		}
	}

}
char* CReportMail::GetSpamCop()
{

	CHTTPclient http;

	CString sendstr;
	sendstr = "/sc?track=";
	sendstr = sendstr + IP;
	
	_cprintf("QueryIp: %s\n",IP);

    HANDLE hand = http.IPGET("220.130.117.23","www.spamcop.net",sendstr.GetBuffer(sendstr.GetLength()),60 * 1000, 60 * 1000);  

    DWORD rtn = WaitForSingleObject(hand,60 * 1000);
	if  (rtn == WAIT_OBJECT_0)
	{
	
		FILE *fp;
		fp = fopen(http.GetTempFilePath(),"r+b");
	 

		fseek(fp,0,SEEK_END);
		int filesize = ftell(fp);

		fseek(fp,0,SEEK_SET);

		char *buff = new char[filesize+1];
		memset(buff,0,filesize+1);

		fread(buff,1,filesize,fp);



		//_cprintf("%s",buff);

		fclose(fp);

		TrimResponse(buff);
		strcpy(contact,buff);


		delete buff;
	
	}

	DeleteFile(http.GetTempFilePath());
	
	
	return contact;
}

char* CReportMail::GetContact()
{

	char nic[16];
	memset(nic,0,16);

	int niccode = GetNic(nic);

	//CCoreSocket::Startup();
	
	CWhoisClient ws;
	char ipstr[100];
	memset(ipstr,0,100);
	if (niccode == 2)
	{
	 strcpy(ipstr,"+");
	 strcat(ipstr,IP);
	}
	else
	{
	 strcpy(ipstr,IP);
	}

	HANDLE h = ws.Send(nic, ipstr, 3000, 10000);
	DWORD h2 = WaitForSingleObject(h, 30000);

	 

	if (h2 == WAIT_OBJECT_0)
	{
		 
		_cprintf("%s\r\n", ws.m_Result);
		//_cprintf("%d\n",niccode);
	
		if (niccode == APNIC ||  niccode == RIPE || niccode == AFRINIC )
		{
			ParseAPNIC(ws.m_Result);

			/*FILE *fp;
			fp = fopen("c:\\1.txt","w+b");
			fwrite(ws.m_Result,1,strlen(ws.m_Result),fp);
			fclose(fp);*/
		}
		else if (niccode == ARIN)
		{
			ParseARIN(ws.m_Result);		
		}  
		else if (niccode == LACNIC)
		{
		
			ParseLACNIC(ws.m_Result);	

		}	 
		else
		{
			//_cprintf("%s\r\n","Undifined!");
		}
		 
		
	}


	if (contact[0] != 0)
	{
		
		_cprintf("%s\n",contact);
		
		CWhoisClient ws2;
		h = ws2.Send("208.31.42.95", contact, 3000, 10000);
		h2 = WaitForSingleObject(h, 30000);	 

		if (h2 == WAIT_OBJECT_0)
		{
			_cprintf("%s\n",ws2.m_Result);
			TrimContact(ws2.m_Result);
			strcpy(contact,ws2.m_Result);

			

		}
	}
	
	//CCoreSocket::Cleanup();

	return contact;
}

void CDBserviceDlg::OnButton9() 
{

     CCompress cc;
	 
	 CString src,desc;
	 m_ZipFile.GetWindowText(src);
	 m_ZipDesc.GetWindowText(desc);

	 cc.ZipFile(src.GetBuffer(src.GetLength()),desc.GetBuffer(desc.GetLength()));

	 src.ReleaseBuffer();
	 desc.ReleaseBuffer();
	//cc.UnZipFile("c:\\htm.zip","c:\\htm.rtf");

	 MessageBox("ok");
 
}

void CDBserviceDlg::OnButton10() 
{
        CDatabase db;
		db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
	 

		CRecordset rec(&db); 

		CString sql;
		CString mailid;

		m_MailId.GetWindowText(mailid);

		if (m_CkDb.GetCheck() == BST_CHECKED)
		{
			//sql.Format("select MailContent from spammail (nolock) where SpamMailId=%s",mailid);
			sql.Format("select MailContent from spamdog2archive..spammailarchive (nolock) where SpamMailId=%s",mailid);
		}
		else
		{
			sql.Format("select MailContent from spammail (nolock) where SpamMailId=%s",mailid);
		}

		if (rec.Open(CRecordset::forwardOnly,sql))
		{

			CString MailContent;

			if(!rec.IsEOF())
			{
			 
				rec.GetFieldValue((int)0,MailContent);

				FILE *fp;
				fp = fopen("c:\\cmpmail.txt","w+b");
				fwrite(MailContent.GetBuffer(MailContent.GetLength()),1,MailContent.GetLength(),fp);
				MailContent.ReleaseBuffer();

				fclose(fp);

				OnButton7();
			 
			}
			else
			{
				AfxMessageBox("Not Found");
			}



		}
		else
		{
			AfxMessageBox("Not Found");
		}

		db.Close();



}

void CDBserviceDlg::OnButton11() 
{
	// TODO: Add your control notification handler code here
	 CDatabase db;
	 db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234"); 

	 CDatabase db2;
	 db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234"); 

  CRecordset rec(&db);
  CString sql;
  sql.Format("select IP,ipid from iptbl (nolock) where (Bad - Ok >= 5)  and RblStatus = 0");

  if (rec.Open(CRecordset::forwardOnly,sql))
  {
	   while (!rec.IsEOF())
	   {   
		  
		   CString IP;		   
		   CString IPid;

		   rec.GetFieldValue((int)0,IP);
		   rec.GetFieldValue((int)1,IPid);

		   char Rip[20];
		   strcpy(Rip,IP);				
		   IpReserve(Rip);

		   char Cmd[255];
		   strcpy(Cmd,"c:/dnscmd.exe 192.168.1.3 /RecordAdd rbl.softworking.com ");
		   strcat(Cmd,Rip);
		   strcat(Cmd," A 127.0.0.5");
		   WinExec(Cmd,SW_SHOW);

		   sql.Format("update iptbl set RblStatus = 1 where ipid = %s",IPid);
		   db2.ExecuteSQL(sql);

		   Sleep(200);

		rec.MoveNext();


	   }
	  
  }

     

  db.Close();
	 

  db2.Close();

	 AfxMessageBox("ok");

}

void CDBserviceDlg::OnButton12() 
{
     CSpamContact sc;

	 
	 sc.SetIp("65.174.255.255 ");
	 unsigned int IPs = sc.GetIPValue();

	 
 
	
}

void CDBserviceDlg::OnButton13() 
{
     CDatabase db;
	 db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234"); 

	 CString sql;
	 CString mailid;

	 m_MailId.GetWindowText(mailid);

	 sql.Format("update  iptbl set ok = 0 , bad = 0 , atversion = 6  where ipid = %s",mailid);
	 db.ExecuteSQL(sql);
	 

	 db.Close();

	 AfxMessageBox("reset");
}

void CDBserviceDlg::OnButton14() 
{
     CDatabase db;
	 db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234"); 

	 CString sql;
	 CString mailid;

	 m_MailId.GetWindowText(mailid);

	 sql.Format("update  iptbl set ok = 0 , bad = 0 , atversion = 998  where ipid = %s",mailid);
	 db.ExecuteSQL(sql);
	 

	 db.Close();

	 AfxMessageBox("stop"); 	
}

void CDBserviceDlg::OnButton15() 
{
	GenFile(true);

	AfxMessageBox("ok"); 
	

}

void CDBserviceDlg::OnButton16() 
{
	// TODO: Add your control notification handler code here


}

void CDBserviceDlg::OnButton17() 
{
    if (hConsole == NULL)
	{
		AllocConsole();
		hConsole = CreateFile("CONOUT$", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT );
		SetConsoleTitle("DBservice Console");
	}

	GetReportMail();	
}

void CDBserviceDlg::OnButton18() 
{
	 CDatabase db;
	 db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234"); 

	 CString sql;
	 CString mailid;

	 m_MailId.GetWindowText(mailid);

	 sql.Format("update  iptbl set ok = 0 , bad = 0 , atversion = 8  where ipid = %s",mailid);
	 db.ExecuteSQL(sql);
	 

	 db.Close();

	 AfxMessageBox("gateway");
}

void CDBserviceDlg::OnButton19() 
{
	 CDatabase db;
	 db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234"); 

	 CString sql;
	 CString mailid;

	 m_MailId.GetWindowText(mailid);

	 sql.Format("update  iptbl set ok = 0 , bad = 0 , atversion = 10  where ipid = %s",mailid);
	 db.ExecuteSQL(sql);
	 

	 db.Close();

	 AfxMessageBox("enews");
	
}


void CDBserviceDlg::SendFixMail(CString DogId , CString Ipid)
{

	CDatabase db,db2;
	CRecordset rec(&db); 

	db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234"); 
	db2.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
	
	CRecordset rec2(&db2);

	CString sql;

	CString SubjectContent;
	CString MailContent;
	CString ReplaceContent;

	sql.Format("insert into errfixlog (DogId,IpId) values (%s,%s)",DogId,Ipid);
	db.ExecuteSQL(sql);	

	//sql.Format("SELECT top 50 c.Subject, c.type ,  convert(char(19),b.AddTime,20) , Sender FROM iptbl AS a, spammail AS b, subtbl AS c WHERE c.orgsid=b.spammailid And c.ip=a.ip and a.ipid=%s and Dogid = %s order by b.AddTime desc",Ipid,DogId);
	sql.Format("SELECT top 10 Subject, type ,  convert(char(19),AddTime,20) , Sender , IpId FROM subtbl (nolock)  WHERE ipid=%s and Dogid=%s and AddTime >= DATEADD(day, -14, getdate()) order by subid desc",Ipid,DogId);

	BOOL QueryOk = false;
	//try
	//{
	WriteQueryLog(sql.GetBuffer(sql.GetLength()));
	sql.ReleaseBuffer();

	   QueryOk = rec.Open(CRecordset::forwardOnly,sql);
	//}
	//catch(...)
	//{
	  // AfxMessageBox(sql);
	//}

	//if (QueryOk)
	//{

			CString Subject;
			CString MailType;
			CString AddTime;
			CString Sender;
			CString IpId;

			while(!rec.IsEOF())
			{
			 
				int BadRate = 0;
				int OkRate = 0;
				
				rec.GetFieldValue((int)0,Subject);
				rec.GetFieldValue((int)1,MailType);
				rec.GetFieldValue((int)2,AddTime);
				rec.GetFieldValue((int)3,Sender);
				rec.GetFieldValue((int)4,IpId);
				

				CString TypeStr;

				if (MailType.Find("0") == 0)
				{
					TypeStr = "垃圾郵件";
					BadRate = 1;
				}
				else
				{
					TypeStr = "正常郵件";
					OkRate = 1;
				}

				CString tmpReplaceContent;
				tmpReplaceContent.Format("<tr><td width=138><p align=center>&nbsp;%s</td><td>%s</td><td>%s</td><td width=100><p align=center>&nbsp;%s</td></tr>",AddTime,Sender,Subject,TypeStr);

				ReplaceContent += tmpReplaceContent ;

				//刪除 iptbl 的記錄
				//sql.Format("update iptbl set Bad = Bad - %d , Ok = Ok - %d , lastusedtime = getdate() where Ipid = %s",BadRate,OkRate,IpId);
				//db2.ExecuteSQL(sql);


				rec.MoveNext();				
			}


	//}

	rec.Close();
	

	sql.Format("select RateLogId , Ok ,Bad  from ratelog (nolock) where Ipid = %s and DogId= %s  and AtVersion = 0",Ipid,DogId);
	WriteQueryLog(sql.GetBuffer(sql.GetLength()));
	sql.ReleaseBuffer();

	if (rec.Open(CRecordset::forwardOnly,sql))
	{

			CString RateLogId;

			CDBVariant Ok;
			CDBVariant Bad;
			 

			while(!rec.IsEOF())
			{
				rec.GetFieldValue((int)0,RateLogId);
				rec.GetFieldValue((int)1,Ok,SQL_C_SLONG);
				rec.GetFieldValue((int)2,Bad,SQL_C_SLONG);			 
				

				 
				//刪除 iptbl 的記錄
				sql.Format("update iptbl set Bad = Bad - %d , Ok = Ok - %d , lastusedtime = getdate() where Bad - %d >= 0 and ok - %d >= 0 and Ipid = %s",Bad.m_iVal,Ok.m_iVal,Bad.m_iVal,Ok.m_iVal,Ipid);
				WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				sql.ReleaseBuffer();
				//try
				//{
				 db2.ExecuteSQL(sql);
				//}
				//catch(...)
				//{
				  //AfxMessageBox("SendFixMail 1");
				//}

				sql.Format("update ratelog set AtVersion = 1 where RateLogId = %s",RateLogId);
				//try
				//{
				WriteQueryLog(sql.GetBuffer(sql.GetLength()));
				sql.ReleaseBuffer();

			    db2.ExecuteSQL(sql);
				//}
				//catch(...)
				//{
				   //AfxMessageBox("SendFixMail 2");
				//}


				rec.MoveNext();				
			}


	}



	if (ReplaceContent.GetLength() > 0)
	{

		FILE *fp1=NULL;

		fp1 = fopen("c:\\DbConfirm.htm","rb");

		CString ReadContent;
		char buff[1025];
		while ( !feof( fp1 ))
		{
			int readsize = fread(buff,1,1024,fp1);
			buff[readsize] = 0;
			ReadContent += buff;
		}

		fclose(fp1);

		int pos1 = ReadContent.Find("<!--Replace1-->");
		int pos2 = ReadContent.Find("<!--Replace2-->");
		int pos3 = ReadContent.Find("<!--Replace3-->");

 

		MailContent = ReadContent.Mid(0,pos1);
		MailContent += ReplaceContent;
		MailContent += ReadContent.Mid(pos1+15,pos2-pos1-15);
		MailContent += Ipid;
		MailContent += ReadContent.Mid(pos2+15,pos3-pos2-15);
		MailContent += DogId;
		MailContent += ReadContent.Mid(pos3+15);

		FILE *fp2=NULL;
		fp2 = fopen("c:\\out.htm","w+b");
		fwrite(MailContent.GetBuffer(MailContent.GetLength()),1,MailContent.GetLength(),fp2);
		fclose(fp2);

		CString MailHeader;
		MailHeader = "from: 軟體工廠垃圾郵件中心 <service@softworking.com>\r\n" ;
 		MailHeader += "Subject: 信件誤判警告通知 !!\r\n";
 		MailHeader += "Content-Type: text/html\r\n\r\n";
 		 
		MailContent = MailHeader + MailContent;

	}
	

	if (MailContent.GetLength() > 0 )
	{
	

		CCompress cc;

		unsigned int outsize = cc.EstimateOutBuffSize(MailContent.GetLength());
		char *outbuff = new char [outsize];
		memset(outbuff,0,outsize);

	 
	
		HRESULT res = cc.DoCompress((unsigned char*)MailContent.GetBuffer(MailContent.GetLength()),MailContent.GetLength(),(unsigned char *) outbuff,&outsize);
		if (res == S_OK || NO_NEED_COMPRESS)	
		{
			CMailCodec MailCodec;
			char *EncodeStr = MailCodec.Base64Encode2(outbuff,outsize);

			CString sql;
			sql.Format("insert into mail (IsCompress,OrgLen,ThisLen,MailContent) values (1,%d,%d,'%s')",MailContent.GetLength(),strlen(EncodeStr),EncodeStr);
			WriteQueryLog(sql.GetBuffer(sql.GetLength()));
			sql.ReleaseBuffer();

			db2.ExecuteSQL(sql);

			sql = "select Max(MailId)  from mail";

			WriteQueryLog(sql.GetBuffer(sql.GetLength()));
			sql.ReleaseBuffer();

			if (rec2.Open(CRecordset::forwardOnly ,sql))
			{
				if (!rec2.IsEOF())
				{
					CString MailId;
					rec2.GetFieldValue((int)0,MailId);

					//try
					//{
					CString sql;
					sql.Format("insert into pmail (MailId , ToId) values (%s,1)",MailId);					
					WriteQueryLog(sql.GetBuffer(sql.GetLength()));
					sql.ReleaseBuffer();

					db2.ExecuteSQL(sql);

					sql.Format("insert into pmail (MailId , ToId) values (%s,2)",MailId);

					WriteQueryLog(sql.GetBuffer(sql.GetLength()));
					sql.ReleaseBuffer();
					db2.ExecuteSQL(sql);

					sql.Format("insert into pmail (MailId , ToId) values (%s,%s)",MailId,DogId);
					WriteQueryLog(sql.GetBuffer(sql.GetLength()));
					sql.ReleaseBuffer();
					db2.ExecuteSQL(sql);
					//}
					//catch(...)
					//{
						//AfxMessageBox("SendFixMail 3");
					//}
				}

			}
		 
     		
     	 

			MailCodec.FreeBase64Encode(EncodeStr); 
		}
 


 
		delete [] outbuff;

	
	}


	MailContent.ReleaseBuffer();

		db.Close();
		db2.Close();


}

void CDBserviceDlg::OnButton20() 
{
	 
	CSpamContact sc;

	 /*sc.SetIp("200.180.59.226");
	 CString str;
	 str.Format("%u",sc.GetIPValue());
	 OutputDebugString(str);
	 return;

	 sc.SetIp(1376129024);	 
	 OutputDebugString(sc.IP);
	 return;*/
	 

	 



	 CString atext;
	 m_MailId.GetWindowText(atext);

		char InetNumS[16];
		char InetNumE[16];
		char netname[255];
		char country[3];
		char contact[255];

		memset(&contact,0,sizeof(contact));
		memset(&netname,0,sizeof(netname));
		memset(&country,0,sizeof(country));
		memset(&InetNumS,0,sizeof(InetNumS));
		memset(&InetNumE,0,sizeof(InetNumE));

		
		 
		
		

	 sc.SetIp(atext.GetBuffer(atext.GetLength()));
	 sc.GetContact(InetNumS,InetNumE,netname,country,contact);
 
	 AfxMessageBox(InetNumS);
	 AfxMessageBox(InetNumE);
	 AfxMessageBox(netname);

	 AfxMessageBox(contact);
	 AfxMessageBox(country);

	 /*sc.SetIp("200.52.144.0");
	 CString str;
	 str.Format("%u",sc.GetIPValue());
	 OutputDebugString(str);*/


	//CTreeRangeDB rgDB("c:\\file.txt","c:\\data.txt");
	//rgDB.InserKey(2,20,rgDB.FirstRecPos,0);
//	rgDB.Search(35,rgDB.FirstRecPos);
	//rgDB.InserKey(30,50,rgDB.FirstRecPos,0);

	
}

void CDBserviceDlg::OnButton21() 
{
	    CDatabase db;
		db.Open("ODBC;DSN=spamdog2;UID=dog;PWD=dog1234");
	 

		CRecordset rec(&db); 

		CString sql;
		CString CrashId;

		m_MailId.GetWindowText(CrashId);
		sql.Format("select CrashData,OrgLen from crash (nolock) where CrashId=%s",CrashId);
		if (rec.Open(CRecordset::forwardOnly,sql))
		{

			CString CrashContent;
			CString OrgLen;

			if(!rec.IsEOF())
			{
			 
				rec.GetFieldValue((int)0,CrashContent);
				rec.GetFieldValue((int)1,OrgLen);

				FILE *fp;
				fp = fopen("c:\\Crash.dat","w+b");
				fwrite(CrashContent.GetBuffer(CrashContent.GetLength()),1,CrashContent.GetLength(),fp);
				CrashContent.ReleaseBuffer();

				fclose(fp);

				int orgsize = atoi(OrgLen);
				

				UnZipFile("c:\\Crash.dat","c:\\Crash.dmp",orgsize);
				AfxMessageBox("ok");

				DeleteFile("c:\\Crash.dat");

				
			 
			}


		}

		db.Close();
	
}



