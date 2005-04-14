# Microsoft Developer Studio Project File - Name="xigfx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xigfx - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xigfx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xigfx.mak" CFG="xigfx - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xigfx - Win32 Release" (based on "Win32 (x86) Static Library")
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
# ADD CPP /nologo /G5 /MD /W3 /O2 /I "..\include" /I "..\..\sdl\include" /D "_WINDOWS" /D "HAVE_SDL_SDL_MIXER_H" /D "HAVE_SDL_SDL_TFF" /D "WIN32" /D "NDEBUG" /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "xigfx - Win32 Release"
# Begin Source File

SOURCE=..\xigfx\fonts.c
# End Source File
# Begin Source File

SOURCE=..\xigfx\iarea.c
# End Source File
# Begin Source File

SOURCE=..\xigfx\tiles.c
# End Source File
# Begin Source File

SOURCE=..\xigfx\views.c
# End Source File
# Begin Source File

SOURCE=..\xigfx\xi_init.c
# End Source File
# Begin Source File

SOURCE=..\xigfx\xi_main.c
# End Source File
# End Target
# End Project
