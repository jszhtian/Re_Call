# Microsoft Developer Studio Project File - Name="nnnEdit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=nnnEdit - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "nnnEdit.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "nnnEdit.mak" CFG="nnnEdit - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "nnnEdit - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "nnnEdit - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nnnEdit - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\nnnEditRelease"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "nnnEdit - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\nnnEditRelease"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\nnnEditRelease\nnnEdit_Debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "nnnEdit - Win32 Release"
# Name "nnnEdit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\allEffectParam.cpp
# End Source File
# Begin Source File

SOURCE=.\autoVoice.cpp
# End Source File
# Begin Source File

SOURCE=.\aviSave.cpp
# End Source File
# Begin Source File

SOURCE=.\buttonPic.cpp
# End Source File
# Begin Source File

SOURCE=.\case.cpp
# End Source File
# Begin Source File

SOURCE=.\checkDWQ.cpp
# End Source File
# Begin Source File

SOURCE=.\commanddata.cpp
# End Source File
# Begin Source File

SOURCE=.\commonDIB.cpp
# End Source File
# Begin Source File

SOURCE=.\configData.cpp
# End Source File
# Begin Source File

SOURCE=.\conteDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\conteMessageDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\conteMessageView.cpp
# End Source File
# Begin Source File

SOURCE=.\conteView.cpp
# End Source File
# Begin Source File

SOURCE=.\controldoc.cpp
# End Source File
# Begin Source File

SOURCE=.\controlview.cpp
# End Source File
# Begin Source File

SOURCE=.\copyPreEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\defaultControl.cpp
# End Source File
# Begin Source File

SOURCE=.\deleteNonVoice.cpp
# End Source File
# Begin Source File

SOURCE=.\effectdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\effectParam.cpp
# End Source File
# Begin Source File

SOURCE=.\effectPic.cpp
# End Source File
# Begin Source File

SOURCE=.\effectview.cpp
# End Source File
# Begin Source File

SOURCE=.\filmcase.cpp
# End Source File
# Begin Source File

SOURCE=.\filmcasedata.cpp
# End Source File
# Begin Source File

SOURCE=.\filmcasedoc.cpp
# End Source File
# Begin Source File

SOURCE=.\filmcaseview.cpp
# End Source File
# Begin Source File

SOURCE=.\filmdata.cpp
# End Source File
# Begin Source File

SOURCE=.\filmdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\filmview.cpp
# End Source File
# Begin Source File

SOURCE=.\frameChange.cpp
# End Source File
# Begin Source File

SOURCE=.\gamemessagedoc.cpp
# End Source File
# Begin Source File

SOURCE=.\gamemessageview.cpp
# End Source File
# Begin Source File

SOURCE=.\komadata.cpp
# End Source File
# Begin Source File

SOURCE=.\layerdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\layerview.cpp
# End Source File
# Begin Source File

SOURCE=.\lockVoice.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\mainscreendoc.cpp
# End Source File
# Begin Source File

SOURCE=.\mainscreenview.cpp
# End Source File
# Begin Source File

SOURCE=.\makeBook.cpp
# End Source File
# Begin Source File

SOURCE=.\makeCheckDWQBat.cpp
# End Source File
# Begin Source File

SOURCE=.\makeCheckVawBat.cpp
# End Source File
# Begin Source File

SOURCE=.\makeUseDWQBat.cpp
# End Source File
# Begin Source File

SOURCE=.\makeUseVawBat.cpp
# End Source File
# Begin Source File

SOURCE=.\menuCheckControl.cpp
# End Source File
# Begin Source File

SOURCE=.\messagedata.cpp
# End Source File
# Begin Source File

SOURCE=.\messageSerialControl.cpp
# End Source File
# Begin Source File

SOURCE=.\messageSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\messageWindowControl.cpp
# End Source File
# Begin Source File

SOURCE=.\minicompiler.cpp
# End Source File
# Begin Source File

SOURCE=.\miniPic.cpp
# End Source File
# Begin Source File

SOURCE=.\minipiccache.cpp
# End Source File
# Begin Source File

SOURCE=.\movieCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\myapplication.cpp
# End Source File
# Begin Source File

SOURCE=.\myBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\myBmp.cpp
# End Source File
# Begin Source File

SOURCE=.\mybutton.cpp
# End Source File
# Begin Source File

SOURCE=.\myclientwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\myconfig.cpp
# End Source File
# Begin Source File

SOURCE=.\mydib.cpp
# End Source File
# Begin Source File

SOURCE=.\mydocument.cpp
# End Source File
# Begin Source File

SOURCE=.\myfileopen.cpp
# End Source File
# Begin Source File

SOURCE=.\myframewindow.cpp
# End Source File
# Begin Source File

SOURCE=.\myinputdialog.cpp
# End Source File
# Begin Source File

SOURCE=.\myOnTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\myPopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\myResourceID.cpp
# End Source File
# Begin Source File

SOURCE=.\mySearchDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\myview.cpp
# End Source File
# Begin Source File

SOURCE=.\programdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\programview.cpp
# End Source File
# Begin Source File

SOURCE=.\sameMessageDeutch.cpp
# End Source File
# Begin Source File

SOURCE=.\script1.rc
# End Source File
# Begin Source File

SOURCE=.\scriptCompiler.cpp
# End Source File
# Begin Source File

SOURCE=.\scriptdata.cpp
# End Source File
# Begin Source File

SOURCE=.\scriptSoundControl.cpp
# End Source File
# Begin Source File

SOURCE=.\scriptVoiceControl.cpp
# End Source File
# Begin Source File

SOURCE=.\scrollview.cpp
# End Source File
# Begin Source File

SOURCE=.\searchOrReplace.cpp
# End Source File
# Begin Source File

SOURCE=.\selectdialog.cpp
# End Source File
# Begin Source File

SOURCE=.\selectobjectonly.cpp
# End Source File
# Begin Source File

SOURCE=.\starttitle.cpp
# End Source File
# Begin Source File

SOURCE=.\storybookdata.cpp
# End Source File
# Begin Source File

SOURCE=.\storybookdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\storybookview.cpp
# End Source File
# Begin Source File

SOURCE=.\storydata.cpp
# End Source File
# Begin Source File

SOURCE=.\storydoc.cpp
# End Source File
# Begin Source File

SOURCE=.\storyview.cpp
# End Source File
# Begin Source File

SOURCE=.\suuji.cpp
# End Source File
# Begin Source File

SOURCE=.\vardoc.cpp
# End Source File
# Begin Source File

SOURCE=.\varview.cpp
# End Source File
# Begin Source File

SOURCE=.\volumeControl.cpp
# End Source File
# Begin Source File

SOURCE=.\windowUpdateControl.cpp
# End Source File
# Begin Source File

SOURCE=.\zahyodoc.cpp
# End Source File
# Begin Source File

SOURCE=.\zahyoview.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\allEffectParam.h
# End Source File
# Begin Source File

SOURCE=.\autoVoice.h
# End Source File
# Begin Source File

SOURCE=.\aviSave.h
# End Source File
# Begin Source File

SOURCE=.\bitmapnumber.h
# End Source File
# Begin Source File

SOURCE=.\buttonPic.h
# End Source File
# Begin Source File

SOURCE=.\case.h
# End Source File
# Begin Source File

SOURCE=.\checkDWQ.h
# End Source File
# Begin Source File

SOURCE=.\commanddata.h
# End Source File
# Begin Source File

SOURCE=.\commanddatatype.h
# End Source File
# Begin Source File

SOURCE=.\commonDIB.h
# End Source File
# Begin Source File

SOURCE=.\configData.h
# End Source File
# Begin Source File

SOURCE=.\conteDoc.h
# End Source File
# Begin Source File

SOURCE=.\conteMessageDoc.h
# End Source File
# Begin Source File

SOURCE=.\conteMessageView.h
# End Source File
# Begin Source File

SOURCE=.\conteView.h
# End Source File
# Begin Source File

SOURCE=.\controldoc.h
# End Source File
# Begin Source File

SOURCE=.\controlview.h
# End Source File
# Begin Source File

SOURCE=.\copyPreEffect.h
# End Source File
# Begin Source File

SOURCE=.\defaultControl.h
# End Source File
# Begin Source File

SOURCE=.\define.h
# End Source File
# Begin Source File

SOURCE=.\deleteNonVoice.h
# End Source File
# Begin Source File

SOURCE=.\effectdoc.h
# End Source File
# Begin Source File

SOURCE=.\effectParam.h
# End Source File
# Begin Source File

SOURCE=.\effectPic.h
# End Source File
# Begin Source File

SOURCE=.\effectview.h
# End Source File
# Begin Source File

SOURCE=.\filmcase.h
# End Source File
# Begin Source File

SOURCE=.\filmcasedata.h
# End Source File
# Begin Source File

SOURCE=.\filmcasedoc.h
# End Source File
# Begin Source File

SOURCE=.\filmcaseview.h
# End Source File
# Begin Source File

SOURCE=.\filmdata.h
# End Source File
# Begin Source File

SOURCE=.\filmdoc.h
# End Source File
# Begin Source File

SOURCE=.\filmview.h
# End Source File
# Begin Source File

SOURCE=.\frameChange.h
# End Source File
# Begin Source File

SOURCE=.\gamemessagedoc.h
# End Source File
# Begin Source File

SOURCE=.\gamemessageview.h
# End Source File
# Begin Source File

SOURCE=.\komadata.h
# End Source File
# Begin Source File

SOURCE=.\layerdoc.h
# End Source File
# Begin Source File

SOURCE=.\layerview.h
# End Source File
# Begin Source File

SOURCE=.\lockVoice.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\mainscreendoc.h
# End Source File
# Begin Source File

SOURCE=.\mainscreenview.h
# End Source File
# Begin Source File

SOURCE=.\makeBook.h
# End Source File
# Begin Source File

SOURCE=.\makeCheckDWQBat.h
# End Source File
# Begin Source File

SOURCE=.\makeCheckVawBat.h
# End Source File
# Begin Source File

SOURCE=.\makeUseDWQBat.h
# End Source File
# Begin Source File

SOURCE=.\makeUseVawBat.h
# End Source File
# Begin Source File

SOURCE=.\menuCheckControl.h
# End Source File
# Begin Source File

SOURCE=.\messagedata.h
# End Source File
# Begin Source File

SOURCE=.\messageSerialControl.h
# End Source File
# Begin Source File

SOURCE=.\messageSetup.h
# End Source File
# Begin Source File

SOURCE=.\messageWindowControl.h
# End Source File
# Begin Source File

SOURCE=.\minicompiler.h
# End Source File
# Begin Source File

SOURCE=.\miniPic.h
# End Source File
# Begin Source File

SOURCE=.\minipiccache.h
# End Source File
# Begin Source File

SOURCE=.\movieCreate.h
# End Source File
# Begin Source File

SOURCE=.\myapplication.h
# End Source File
# Begin Source File

SOURCE=.\myBitmap.h
# End Source File
# Begin Source File

SOURCE=.\myBmp.h
# End Source File
# Begin Source File

SOURCE=.\mybutton.h
# End Source File
# Begin Source File

SOURCE=.\myclientwindow.h
# End Source File
# Begin Source File

SOURCE=.\myconfig.h
# End Source File
# Begin Source File

SOURCE=.\mydib.h
# End Source File
# Begin Source File

SOURCE=.\mydocument.h
# End Source File
# Begin Source File

SOURCE=.\myfileopen.h
# End Source File
# Begin Source File

SOURCE=.\myframewindow.h
# End Source File
# Begin Source File

SOURCE=.\myinputdialog.h
# End Source File
# Begin Source File

SOURCE=.\myOnTimer.h
# End Source File
# Begin Source File

SOURCE=.\myPopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\mySearchDialog.h
# End Source File
# Begin Source File

SOURCE=.\myview.h
# End Source File
# Begin Source File

SOURCE=.\programdoc.h
# End Source File
# Begin Source File

SOURCE=.\programview.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\sameMessageDeutch.h
# End Source File
# Begin Source File

SOURCE=.\scriptCompiler.h
# End Source File
# Begin Source File

SOURCE=.\scriptdata.h
# End Source File
# Begin Source File

SOURCE=.\scriptSoundControl.h
# End Source File
# Begin Source File

SOURCE=.\scriptVoiceControl.h
# End Source File
# Begin Source File

SOURCE=.\scrollview.h
# End Source File
# Begin Source File

SOURCE=.\searchOrReplace.h
# End Source File
# Begin Source File

SOURCE=.\selectdialog.h
# End Source File
# Begin Source File

SOURCE=.\selectobjectonly.h
# End Source File
# Begin Source File

SOURCE=.\starttitle.h
# End Source File
# Begin Source File

SOURCE=.\storybookdata.h
# End Source File
# Begin Source File

SOURCE=.\storybookdoc.h
# End Source File
# Begin Source File

SOURCE=.\storybookview.h
# End Source File
# Begin Source File

SOURCE=.\storydata.h
# End Source File
# Begin Source File

SOURCE=.\storydoc.h
# End Source File
# Begin Source File

SOURCE=.\storyview.h
# End Source File
# Begin Source File

SOURCE=.\suuji.h
# End Source File
# Begin Source File

SOURCE=.\vardoc.h
# End Source File
# Begin Source File

SOURCE=.\varview.h
# End Source File
# Begin Source File

SOURCE=.\volumeControl.h
# End Source File
# Begin Source File

SOURCE=.\windowlist.h
# End Source File
# Begin Source File

SOURCE=.\windowUpdateControl.h
# End Source File
# Begin Source File

SOURCE=.\zahyodoc.h
# End Source File
# Begin Source File

SOURCE=.\zahyoview.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bmpresource\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\button.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_back.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_bgm.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_change.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_copy.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_cut.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_delete.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_do.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_edit.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_foward.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_jump.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_new.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_nocleareffect.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_open.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_overrap.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_overraptype.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_paste.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_save.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_setcg.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\common_undo.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\commonwindowonoff.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\control.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\bmpresource\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\bmpresource\effect_mini.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\film64x48.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\bmpresource\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\bmpresource\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\bmpresource\layer.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\layercop.cur
# End Source File
# Begin Source File

SOURCE=.\bmpresource\mespic.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\nekofont_s.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\nekotitle.BMP
# End Source File
# Begin Source File

SOURCE=.\bmpresource\nodrop.cur
# End Source File
# Begin Source File

SOURCE=.\bmpresource\seleffect.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\story.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\var.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpresource\zahyosw.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=".\仕様書.txt"
# End Source File
# End Target
# End Project
