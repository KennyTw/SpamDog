// MsgFrm.cpp : implementation file
//

#include "stdafx.h"
#include "SpamDogLite.h"
#include "MsgFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//  ===========================================================================
//  The definitions for the layering stuff
//  ===========================================================================
#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer
#define MSG_QUEUE_LIMIT 2

//  ===========================================================================
//  function pointer definitions for SetLayeredWindowAttributes in User32.dll
//  ===========================================================================
typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
lpfn g_pSetLayeredWindowAttributes;

/////////////////////////////////////////////////////////////////////////////
// CMsgFrm dialog


CMsgFrm::CMsgFrm(CWnd* pParent /*=NULL*/,char MsgType)
	: CDialog(CMsgFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgFrm)
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	m_MsgType = MsgType;

	capmouse = false;
	//msgenable = true;
	InProcess = 0;

	mBgColor = RGB(0xff,0x99,0);
	mTextColor = RGB(0,0,0);

	LayerLevel=1;

	hicon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CMsgFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgFrm)
	DDX_Control(pDX, IDC_STATIC_TIME, m_MsgTime);
		DDX_Control(pDX, IDC_BUTTON1, m_CloseBtn);
		DDX_Control(pDX, IDC_STATIC2, m_MsgText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgFrm, CDialog)
	//{{AFX_MSG_MAP(CMsgFrm)
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgFrm message handlers
void CMsgFrm::ShowMsg(COLORREF BgColor,COLORREF TextColor,char *MsgStr )
{
	

	CTime t = CTime::GetCurrentTime();
	m_MsgTime.SetWindowText(t.Format("%H:%M") );

	CMainFrm *mainfrm = (CMainFrm *) AfxGetMainWnd();

	if (mainfrm->Settings.ShowMsg)
	{
		

	/*	if (InProcess > 0 )
		{
		    if (InProcess > MSG_QUEUE_LIMIT)
			{
			
				KillTimer(1);
				KillTimer(2);
				KillTimer(3);

				ShowQueueMsg();
			}
			
			InterlockedIncrement(&InProcess);
			 
			MsgObj mmsg;
			mmsg.BgColor = BgColor;
			mmsg.TextColor = TextColor;

			if (strlen(MsgStr) > 100)
			{
				strncpy(mmsg.MsgStr , MsgStr , 100);
				mmsg.MsgStr[100] = 0;

			}
			else
			{
				strcpy(mmsg.MsgStr , MsgStr);
			}

			mmsg.MsgTime = CTime::GetCurrentTime();
			
			m_MsgQueue.insert(m_MsgQueue.end(),mmsg);

			
			
		}
		else if (InProcess == 0)
		{
			
		
			InterlockedIncrement(&InProcess);	
			*/

			mBgColor = BgColor;
			mTextColor = TextColor;

			m_MsgText.SetWindowText(MsgStr);

			m_CloseBtn.SetRepaintParent(true);
			m_CloseBtn.SetNeedText(false);
			m_CloseBtn.SetBitMap(IDB_BITMAP3);		
			m_CloseBtn.SetButtonBgColor(BgColor,BgColor,BgColor);

			Invalidate();
			UpdateWindow();


			//LayerLevel
			if (g_pSetLayeredWindowAttributes != NULL)
				g_pSetLayeredWindowAttributes(this->m_hWnd, 0, LayerLevel , LWA_ALPHA); 
		
			KillTimer(1);
			KillTimer(2);
			KillTimer(3);

		 
			SetTimer(1,mainfrm->Settings.MsgSpeed,NULL);

	 
			ShowWindow(SW_SHOWNOACTIVATE);
		//}
	
	}
	
	
}

void CMsgFrm::ShowQueueMsg()
{
	
	if (capmouse) return;

	ShowWindow(SW_SHOWNOACTIVATE);
	CMainFrm *mainfrm = (CMainFrm *) AfxGetMainWnd();

	if (InProcess > 0 )
	{
		InterlockedDecrement(&InProcess);
	}

	if (m_MsgQueue.size() > 0 )
	{
	
		if (g_pSetLayeredWindowAttributes != NULL)
		g_pSetLayeredWindowAttributes(this->m_hWnd, 0, 255 , LWA_ALPHA);

		


		MsgObj mmsg;
		MsgQueue::iterator itor;				
		itor =  m_MsgQueue.begin();

		 

			mmsg = *itor;

		
			m_MsgTime.SetWindowText(mmsg.MsgTime.Format("%H:%M") );


		
		
			mBgColor = mmsg.BgColor;
			mTextColor = mmsg.TextColor;
	 

			m_MsgText.SetWindowText(mmsg.MsgStr);

			m_CloseBtn.SetRepaintParent(true);
			m_CloseBtn.SetNeedText(false);
			m_CloseBtn.SetBitMap(IDB_BITMAP3);		
			m_CloseBtn.SetButtonBgColor(mmsg.BgColor,mmsg.BgColor,mmsg.BgColor);

			Invalidate();
			UpdateWindow();

			m_MsgQueue.erase(itor); 



	}

	if (InProcess > MSG_QUEUE_LIMIT)
	{
		//int delay = 2000 / InProcess;
		//SetTimer(2,delay,NULL);
		m_MsgQueue.clear();
		SetTimer(2,mainfrm->Settings.MsgDelayTime / 3,NULL);
	}
	else
	{
		SetTimer(2,mainfrm->Settings.MsgDelayTime / 3,NULL);
		
	}

}

/*
void CMsgFrm::SetEnable(bool enable)
{
	msgenable = enable;
}
*/

void CMsgFrm::ShowNewMsg(COLORREF BgColor,COLORREF TextColor,char *MsgStr)
{
	//if (g_pSetLayeredWindowAttributes == NULL)
	//{
    //  		return;
	//}

	CTime t = CTime::GetCurrentTime(); 
	
	CMsgFrm *newmsg  = new CMsgFrm(this,2);
	newmsg->Create(IDD_MSGFRM_DIALOG,0);

	char TempMsgStr[255];
	if (strlen(MsgStr) > 100)
	{
				strncpy(TempMsgStr , MsgStr , 100);
				TempMsgStr[100] = 0;

	}
	else
	{
				strcpy(TempMsgStr , MsgStr);
	}


	newmsg->SetNewContent(BgColor,TextColor,TempMsgStr);
	newmsg->m_MsgTime.SetWindowText(t.Format("%H:%M") );

	Invalidate();
	UpdateWindow();

				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

	
	m_MsgObjList.insert(m_MsgObjList.end(),(int)newmsg);


}

void CMsgFrm::SetNewContent(COLORREF BgColor,COLORREF TextColor,char *MsgStr)
{

	if (g_pSetLayeredWindowAttributes != NULL)
	g_pSetLayeredWindowAttributes(this->m_hWnd, 0, 255 , LWA_ALPHA);

	mBgColor = BgColor;
	mTextColor = TextColor;

	//delete m_pEditBkBrush;
	//m_pEditBkBrush = new CBrush(mBgColor);
	
	
	//CString NewStr;
	//AutoWrap(MsgStr,&NewStr);
	//CStatic	*MsgStatic = (CStatic *) GetDlgItem(IDC_STATIC5);
	m_MsgText.SetWindowText(MsgStr);

	m_CloseBtn.SetRepaintParent(true);
	m_CloseBtn.SetNeedText(false);
	m_CloseBtn.SetBitMap(IDB_BITMAP3);
	//m_CloseBtn.SetButtonTextColor(RGB(0,0,0),RGB(255,255,255),RGB(0,0,0));	
	m_CloseBtn.SetButtonBgColor(BgColor,BgColor,BgColor);

	Invalidate(true);
	
	

	ShowWindow(SW_SHOWNOACTIVATE);
}

void CMsgFrm::CloseAlert(CWnd* obj)
{
	MsgObjList::iterator itor;
	for (itor =  m_MsgObjList.begin(); itor != m_MsgObjList.end() ; itor++)
	{
		int objpointer = *itor;
		CMsgFrm *itemobj;
		itemobj = (CMsgFrm *) objpointer;

		if ((CWnd *) itemobj == obj)
		{
			itemobj->DestroyWindow();
			delete itemobj;
			m_MsgObjList.erase(itor);
			break;
		}

	}
}

BOOL CMsgFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect wrect;
	RECT rWorkArea;

	SystemParametersInfo(SPI_GETWORKAREA,  
						sizeof(RECT),
						&rWorkArea,
						0);  
	
    //int cx = GetSystemMetrics(SM_CXFULLSCREEN);        
   // int cy = GetSystemMetrics(SM_CYFULLSCREEN);

	//::GetWindowRect(GetDesktopWindow()->m_hWnd,&rect);
	GetWindowRect(&wrect);
	SetWindowPos(NULL,rWorkArea.right - wrect.Width() ,rWorkArea.bottom - wrect.Height() ,0,0,SWP_NOSIZE | SWP_HIDEWINDOW | SWP_NOZORDER);
	 


	// TODO: Add extra initialization here
	ModifyStyleEx(0,WS_EX_LAYERED);
	  //  get the function pointer for SetLayeredWindowAttributes in User32.dll
    hUser32 = GetModuleHandle(_T("USER32.DLL"));
    g_pSetLayeredWindowAttributes = (lpfn)GetProcAddress(hUser32, "SetLayeredWindowAttributes");

    if (g_pSetLayeredWindowAttributes == NULL)
	{
       // AfxMessageBox ("Layering is not supported in this version of Windows", MB_ICONEXCLAMATION);
		//return false;
	}

	if (m_MsgType == 2)
	{
		CenterWindow(GetDesktopWindow());

			    MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
	}
	else
	{
		//m_CloseBtn.ShowWindow(SW_HIDE);		
	}
	
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CMsgFrm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	 //return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
		int r1= GetRValue(mBgColor),g1=GetGValue(mBgColor),b1=GetBValue(mBgColor); //Any start color
		int r2=255,g2=255,b2=255; //Any stop color
		int i=0;
		CRect FrmRect;
		int r=r1,g=g1,b=b1;
		//CString sTitle;					 
		//CStatic	*MsgStatic;
		//CBrush   *m_Brush = new CBrush(RGB(255, 255, 255));
		//	CWnd* pParent = GetParent();
		
		//CBrush bb;
		//HBRUSH color;
	

	switch (nCtlColor) 
	{

		  
		 case CTLCOLOR_BTN:
			  
				pDC->SetBkMode(TRANSPARENT); 
				return HBRUSH(GetStockObject(NULL_BRUSH));
				//return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
				break;
		  case CTLCOLOR_STATIC:  
			   //OutputDebugString("CTLCOLOR_STATIC\n");
			    pDC->SetTextColor(mTextColor);
				pDC->SetBkMode(TRANSPARENT);
			
				//m_Brush.CreateStockObject(HOLLOW_BRUSH);

				//return m_Brush;
			    return HBRUSH(GetStockObject(NULL_BRUSH));
				
				break;

		  case CTLCOLOR_DLG:
			   //OutputDebugString("CTLCOLOR_DLG\n");
			   

			  //pDC->SetBkColor(RGB(255,255,255)); 
			  //CDC *dc;
			   
			   //dc = GetDC();
			   GetClientRect(&FrmRect);
			   pDC->SetBkMode(TRANSPARENT);
			   pDC->SetTextColor(mTextColor);

			   
			   
			  
			   

			   //pDC->FillRect(&FrmRect,&bb);
			   
			   //CDC *dc2;
			   //dc2 = GetDC();
			   //dc2->FillSolidRect(0,0,40,FrmRect.Height(),RGB(r1,g1,b1));
			  
				   	for(i=0;i<FrmRect.Height();i++)
					{ 
						
						if (i < FrmRect.Height() - 20 )
						{
							r = r1 + (i * (r2-r1) / FrmRect.Height());
							g = g1 + (i * (g2-g1) / FrmRect.Height());
							b = b1 + (i * (b2-b1) / FrmRect.Height());
						}
						pDC->FillSolidRect(40,i,FrmRect.Width(),1,RGB(r,g,b));					
						
					}

			       /* 
					for(i=0;i<FrmRect.Width();i++)
					{ 
						
						if (i < FrmRect.Width() - 100 && i > 40)
						{
							r = r1 + (i * (r2-r1) / FrmRect.Width());
							g = g1 + (i * (g2-g1) / FrmRect.Width());
							b = b1 + (i * (b2-b1) / FrmRect.Width());
						}
						pDC->FillSolidRect(i,0,1,FrmRect.Height(),RGB(r,g,b));					
						
					}*/
					
  			        pDC->FillSolidRect(0,0,40,FrmRect.Height(),RGB(r1,g1,b1)); 

					pDC->DrawIcon(4,20,hicon);
			 
			   
			   //return CDialog::OnCtlColor(pDC, pWnd, CTLCOLOR_DLG);
			  
			   return HBRUSH(GetStockObject(NULL_BRUSH));
			  // return (HBRUSH)(m_Brush->GetSafeHandle());

			   break;
          
		  /*case CTLCOLOR_DLG:
			   //OutputDebugString("CTLCOLOR_DLG\n");
              // Set color to green on black and return the background  brush.
			  //D_pDC = pDC;   
				GetClientRect(&FrmRect);
                pDC->SetTextColor(mTextColor);
                pDC->SetBkColor(mBgColor);
			    //pDC->SetBkMode(TRANSPARENT);

			
				
					for(i=0;i<FrmRect.Height();i++)
					{ 
						int r,g,b;
						r = r1 + (i * (r2-r1) / FrmRect.Height());
						g = g1 + (i * (g2-g1) / FrmRect.Height());
						b = b1 + (i * (b2-b1) / FrmRect.Height());
						pDC->FillSolidRect(0,i,FrmRect.Width(),1,RGB(r,g,b));					
						//pDC->FillSolidRect(i,0,1,rect.Height(),RGB(r,g,b));
					}
				

					// return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
				
					//m_brush =  HBRUSH(GetStockObject(NULL_BRUSH));
				//}

				 

				*/	 				 
				/*	MsgStatic = (CStatic *) GetDlgItem(IDC_STATIC5);
					MsgStatic->GetWindowText(sTitle);

					if (sTitle.IsEmpty() == FALSE )
					{		 		
						pDC->DrawText((LPCTSTR)sTitle, -1, &FrmRect, DT_WORDBREAK | DT_CENTER  );
					}*/ 

				//return  HBRUSH(GetStockObject(NULL_BRUSH));
					//return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

          default:
              return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}

void CMsgFrm::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect wndRect;
	GetWindowRect(&wndRect);
    ScreenToClient(&wndRect);

	CRect btnRect;
	m_CloseBtn.GetWindowRect(&btnRect);
    ScreenToClient(&btnRect);

	//wndRect.right = wndRect.right - 20;

	

	if (m_MsgType == 1)
	{

	if (wndRect.PtInRect(point))	
	{  // Test if the pointer is on the window
	
		if (!capmouse)
		{
			
			SetCapture();               //  Capture the mouse input
			capmouse = true;

			if (g_pSetLayeredWindowAttributes != NULL)
			g_pSetLayeredWindowAttributes(this->m_hWnd, 0, 255 , LWA_ALPHA);

			KillTimer(1);
			KillTimer(2);
			KillTimer(3);

			//OutputDebugString("in\n");

			
		}

		if (nFlags == MK_LBUTTON)
		{

			
			ReleaseCapture();
			SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,0);
			SetCapture(); 
			

		}
	} 
	else
	{
	
		//if (!btnRect.PtInRect(point))
		//{
			ReleaseCapture();
			capmouse = false;
			CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();

			SetTimer(3,mfrm->Settings.MsgSpeed,NULL);
		//}
		
	}

		if (btnRect.PtInRect(point))
		{

			ReleaseCapture();
			capmouse = false;
			//SetCapture(); 
			
		}

	}
	else
	{
		if (nFlags == MK_LBUTTON)
		{
			SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,0);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CMsgFrm::OnTimer(UINT nIDEvent) 
{
			    MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
	// TODO: Add your message handler code here and/or call default
	
	if (nIDEvent == 1)
	{
		LayerLevel = LayerLevel * 2;

		if (LayerLevel > 255) 
		{
			LayerLevel = 255;
			KillTimer(nIDEvent);

			CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();
			SetTimer(2,mfrm->Settings.MsgDelayTime,NULL);
		}

		if (g_pSetLayeredWindowAttributes != NULL)
		g_pSetLayeredWindowAttributes(this->m_hWnd, 0, LayerLevel , LWA_ALPHA);

		Invalidate();
		UpdateWindow();
	
	}
	else if (nIDEvent == 2)
	{
	
		KillTimer(nIDEvent);

		if (InProcess > 0)
		{
			//¦³ queue
			ShowQueueMsg();
		}
		else
		{

			CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();
		    SetTimer(3,mfrm->Settings.MsgSpeed,NULL);
		
		}

		//Invalidate();
		//UpdateWindow();

		
	} 
	else if (nIDEvent == 3)
	{
	
		LayerLevel = (int) LayerLevel / 2;

		if (InProcess > 0)
		{
			//¦³ queue
			//AfxMessageBox("3 queue");
			KillTimer(nIDEvent);
			ShowQueueMsg();
		} 
		else if (LayerLevel <= 20) 
		{
			LayerLevel = 1;


			KillTimer(nIDEvent);
			ShowWindow(SW_HIDE);

			if (InProcess > 0)
			InterlockedDecrement(&InProcess);

			if (g_pSetLayeredWindowAttributes != NULL)
				g_pSetLayeredWindowAttributes(this->m_hWnd, 0, LayerLevel , LWA_ALPHA);

			
		}

	
	
	}

	CDialog::OnTimer(nIDEvent);
}

void CMsgFrm::OnDestroy() 
{
	
	if (hUser32 != NULL)
		FreeLibrary(hUser32); 
	

	MsgObjList::iterator itor;
	for (itor =  m_MsgObjList.begin(); itor != m_MsgObjList.end() ; itor++)
	{
		int objpointer = *itor;
		CMsgFrm *obj;
		obj = (CMsgFrm *) objpointer;
		obj->DestroyWindow();
		delete obj;

	}

	DestroyIcon(hicon);

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CMsgFrm::OnButton1() 
{
	//if (InProcess > 0)
	//{
		KillTimer(1);
		KillTimer(2);
		KillTimer(3);

		//ShowQueueMsg();

		m_MsgQueue.clear();
	 



		InProcess = 0;
	//}

	//SetFocus();
	//ShowWindow(SW_HIDE);
	if (m_pParent != NULL)
	{
		CMsgFrm *obj = (CMsgFrm *) m_pParent;
		obj->CloseAlert(this);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}	
}
