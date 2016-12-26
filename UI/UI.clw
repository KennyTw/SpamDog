; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWPStatic
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "UI.h"

ClassCount=6
Class1=CUIApp
Class2=CUIDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CFlatButton
Class5=CColorBox
Class6=CWPStatic
Resource3=IDD_UI_DIALOG

[CLS:CUIApp]
Type=0
HeaderFile=UI.h
ImplementationFile=UI.cpp
Filter=N

[CLS:CUIDlg]
Type=0
HeaderFile=UIDlg.h
ImplementationFile=UIDlg.cpp
Filter=D
LastObject=IDC_STATICWP
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=UIDlg.h
ImplementationFile=UIDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_UI_DIALOG]
Type=1
Class=CUIDlg
ControlCount=3
Control1=IDC_COLORBOX,static,1342177284
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_STATICWP,static,1342312448

[CLS:CFlatButton]
Type=0
HeaderFile=FlatButton.h
ImplementationFile=FlatButton.cpp
BaseClass=CButton
Filter=W
VirtualFilter=BWC
LastObject=CFlatButton

[CLS:CColorBox]
Type=0
HeaderFile=ColorBox.h
ImplementationFile=ColorBox.cpp
BaseClass=CStatic
Filter=W
LastObject=CColorBox
VirtualFilter=WC

[CLS:CWPStatic]
Type=0
HeaderFile=WPStatic.h
ImplementationFile=WPStatic.cpp
BaseClass=CStatic
Filter=W
LastObject=CWPStatic
VirtualFilter=WC

