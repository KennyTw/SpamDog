# Microsoft Developer Studio Project File - Name="SpamDogLite" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SpamDogLite - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SpamDogLite.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SpamDogLite.mak" CFG="SpamDogLite - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SpamDogLite - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SpamDogLite - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SpamDogLite", SVAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SpamDogLite - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 MAPI32.lib Swspam.lib Swmatcher.lib Swvirus.lib Swparser.lib Swzip.lib Swfiledb.lib Swsocket.lib Dbghelp.lib SwOlapi.lib  Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"../Swspam/Release" /libpath:"../Swmatcher/Release" /libpath:"../Swvirus/Debug" /libpath:"../Swparser/Release" /libpath:"../Swzip/Release" /libpath:"../Swfiledb/Release" /libpath:"../SwSocket/Release" /libpath:"../SwVirus/Release" /libpath:"../SwOlApi/Release"

!ELSEIF  "$(CFG)" == "SpamDogLite - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_LOCALLAN" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 MAPI32.lib Swspam.lib Swmatcher.lib Swvirus.lib Swparser.lib Swzip.lib Swfiledb.lib Swsocket.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../Swspam/Debug" /libpath:"../Swmatcher/Debug" /libpath:"../Swvirus/Debug" /libpath:"../Swparser/Debug" /libpath:"../Swzip/Debug" /libpath:"../Swfiledb/Debug" /libpath:"../SwSocket/Debug"

!ENDIF 

# Begin Target

# Name "SpamDogLite - Win32 Release"
# Name "SpamDogLite - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutThis.cpp
# End Source File
# Begin Source File

SOURCE=..\UI\ColorBox.cpp
# End Source File
# Begin Source File

SOURCE=..\UI\FlatButton.cpp
# End Source File
# Begin Source File

SOURCE=.\LiveUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\MailFolderLearn.cpp
# End Source File
# Begin Source File

SOURCE=.\MailReportFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MapiAddr.cpp
# End Source File
# Begin Source File

SOURCE=.\MAPIAdviseSink.cpp
# End Source File
# Begin Source File

SOURCE=.\MapiEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MapiFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\MapiProf.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgSetupFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OEAddr.cpp
# End Source File
# Begin Source File

SOURCE=.\OEapi.cpp
# End Source File
# Begin Source File

SOURCE=.\OEWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherSetupFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportTab.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleEditFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleSetupFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanSetupFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupTab.cpp
# End Source File
# Begin Source File

SOURCE=.\SpamDogLite.cpp
# End Source File
# Begin Source File

SOURCE=.\SpamDogLite.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysSetupFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\TurnDB.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser3.cpp
# End Source File
# Begin Source File

SOURCE=.\WizDone.cpp
# End Source File
# Begin Source File

SOURCE=.\WizGetDogId.cpp
# End Source File
# Begin Source File

SOURCE=.\WizLearnTest.cpp
# End Source File
# Begin Source File

SOURCE=.\WizOther.cpp
# End Source File
# Begin Source File

SOURCE=.\WizSetOutlook.cpp
# End Source File
# Begin Source File

SOURCE=.\WizSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\WizWelcome.cpp
# End Source File
# Begin Source File

SOURCE=..\UI\WPStatic.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutThis.h
# End Source File
# Begin Source File

SOURCE=..\Global.h
# End Source File
# Begin Source File

SOURCE=.\LiveUpdate.h
# End Source File
# Begin Source File

SOURCE=.\MailFolderLearn.h
# End Source File
# Begin Source File

SOURCE=.\MailReportFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MapiAddr.h
# End Source File
# Begin Source File

SOURCE=.\MAPIAdviseSink.h
# End Source File
# Begin Source File

SOURCE=.\MapiEx.h
# End Source File
# Begin Source File

SOURCE=.\MapiFolder.h
# End Source File
# Begin Source File

SOURCE=.\MapiProf.h
# End Source File
# Begin Source File

SOURCE=.\MsgFrm.h
# End Source File
# Begin Source File

SOURCE=.\MsgSetupFrm.h
# End Source File
# Begin Source File

SOURCE=.\OEAddr.h
# End Source File
# Begin Source File

SOURCE=.\OEapi.h
# End Source File
# Begin Source File

SOURCE=.\OEWnd.h
# End Source File
# Begin Source File

SOURCE=.\OtherSetupFrm.h
# End Source File
# Begin Source File

SOURCE=.\ReportFrm.h
# End Source File
# Begin Source File

SOURCE=.\ReportTab.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RuleEditFrm.h
# End Source File
# Begin Source File

SOURCE=.\RuleSetupFrm.h
# End Source File
# Begin Source File

SOURCE=.\ScanSetupFrm.h
# End Source File
# Begin Source File

SOURCE=.\SetupFrm.h
# End Source File
# Begin Source File

SOURCE=.\SetupTab.h
# End Source File
# Begin Source File

SOURCE=.\SpamDogLite.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysSetupFrm.h
# End Source File
# Begin Source File

SOURCE=.\TurnDB.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser3.h
# End Source File
# Begin Source File

SOURCE=.\WizDone.h
# End Source File
# Begin Source File

SOURCE=.\WizGetDogId.h
# End Source File
# Begin Source File

SOURCE=.\WizLearnTest.h
# End Source File
# Begin Source File

SOURCE=.\WizOther.h
# End Source File
# Begin Source File

SOURCE=.\WizSetOutlook.h
# End Source File
# Begin Source File

SOURCE=.\WizSheet.h
# End Source File
# Begin Source File

SOURCE=.\WizWelcome.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\baner256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bluecase.ico
# End Source File
# Begin Source File

SOURCE=.\res\delete.bmp
# End Source File
# Begin Source File

SOURCE=.\res\explorer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\explorer.ico
# End Source File
# Begin Source File

SOURCE=.\res\explorer2.ico
# End Source File
# Begin Source File

SOURCE=.\res\hammer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\List.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mail.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mail2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NoReport.bmp
# End Source File
# Begin Source File

SOURCE=".\res\normal view.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Recycle Bin.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\RecycleBin.ico
# End Source File
# Begin Source File

SOURCE=.\res\search.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SpamDog.ico
# End Source File
# Begin Source File

SOURCE=.\res\SpamDog.rc2
# End Source File
# Begin Source File

SOURCE=.\res\spamdogap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Trash1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Trash2.ico
# End Source File
# Begin Source File

SOURCE=.\res\User.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section SpamDog : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
# Section SpamDog : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser3.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
