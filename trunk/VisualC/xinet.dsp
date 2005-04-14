# Microsoft Developer Studio Project File - Name="xinet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xinet - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xinet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xinet.mak" CFG="xinet - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xinet - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VC5\lib"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /O2 /I "..\include" /D "_WINDOWS" /D "WIN32" /D "NDEBUG" /D "HAVE_SDL_SDL_MIXER_H" /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "xinet - Win32 Release"
# Begin Source File

SOURCE=..\xinet\net_accept.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_addnodes.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_client.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_client_free.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_handle.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_process.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_read.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_remove_client.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_server.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_server_free.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_setnonblock.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_tag_server.c
# End Source File
# Begin Source File

SOURCE=..\xinet\net_write.c
# End Source File
# End Target
# End Project
