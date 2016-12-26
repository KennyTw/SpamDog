; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLiveUpdateDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "LiveUpdate.h"

ClassCount=3
Class1=CLiveUpdateApp
Class2=CLiveUpdateDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_LIVEUPDATE_DIALOG

[CLS:CLiveUpdateApp]
Type=0
HeaderFile=LiveUpdate.h
ImplementationFile=LiveUpdate.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CLiveUpdateApp

[CLS:CLiveUpdateDlg]
Type=0
HeaderFile=LiveUpdateDlg.h
ImplementationFile=LiveUpdateDlg.cpp
Filter=D
LastObject=IDC_EDIT1
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=LiveUpdateDlg.h
ImplementationFile=LiveUpdateDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LIVEUPDATE_DIALOG]
Type=1
Class=CLiveUpdateDlg
ControlCount=3
Control1=IDC_STATIC_INFO,static,1342308352
Control2=IDC_PROGRESS1,msctls_progress32,1350565889
Control3=IDC_EDIT_MEMO,edit,1352667268

