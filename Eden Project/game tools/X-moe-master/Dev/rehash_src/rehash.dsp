# Microsoft Developer Studio Project File - Name="rehash" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=rehash - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit\
 NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den\
 Befehl
!MESSAGE 
!MESSAGE NMAKE /f "rehash.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "rehash.mak" CFG="rehash - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "rehash - Win32 Release" (basierend auf\
  "Win32 (x86) Console Application")
!MESSAGE "rehash - Win32 Debug" (basierend auf\
  "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rehash - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "rehash - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "rehash - Win32 Release"
# Name "rehash - Win32 Debug"
# Begin Group "algo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\algo\crc16.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\crc16.h
# End Source File
# Begin Source File

SOURCE=.\algo\crc32.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\crc32.h
# End Source File
# Begin Source File

SOURCE=.\algo\fcs.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\fcs.h
# End Source File
# Begin Source File

SOURCE=.\algo\ghash.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\ghash.h
# End Source File
# Begin Source File

SOURCE=.\algo\gosthash.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\gosthash.h
# End Source File
# Begin Source File

SOURCE=.\algo\haval.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\haval.h
# End Source File
# Begin Source File

SOURCE=.\algo\md2.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\md2.h
# End Source File
# Begin Source File

SOURCE=.\algo\md4.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\md4.h
# End Source File
# Begin Source File

SOURCE=.\algo\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\md5.h
# End Source File
# Begin Source File

SOURCE=.\algo\sha1.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\sha1.h
# End Source File
# Begin Source File

SOURCE=.\algo\sha2.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\sha2.h
# End Source File
# Begin Source File

SOURCE=.\algo\sizehash.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\sizehash.h
# End Source File
# Begin Source File

SOURCE=.\algo\tiger.cpp
# End Source File
# Begin Source File

SOURCE=.\algo\tiger.h
# End Source File
# End Group
# Begin Group "main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hashmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\hashmgr.h
# End Source File
# Begin Source File

SOURCE=.\rehash.cpp
# End Source File
# Begin Source File

SOURCE=.\rehash.h
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\clutil.cpp
# End Source File
# Begin Source File

SOURCE=.\clutil.h
# End Source File
# End Group
# Begin Group "system"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rhsyscfg.h
# End Source File
# End Group
# End Target
# End Project
