# Microsoft Developer Studio Project File - Name="xitools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=XITOOLS - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xitools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xitools.mak" CFG="XITOOLS - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xitools - Win32 Release" (based on "Win32 (x86) Static Library")
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
# ADD CPP /nologo /G5 /MD /W3 /O2 /I "..\include" /I "..\..\sdl\include" /D "_WINDOWS" /D "HAVE_SDL_SDL_MIXER_H" /D "WIN32" /D "NDEBUG" /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "xitools - Win32 Release"
# Begin Source File

SOURCE=..\xitools\cfg_findnext.c
# End Source File
# Begin Source File

SOURCE=..\xitools\cfg_findsection.c
# End Source File
# Begin Source File

SOURCE=..\xitools\cfg_freesection.c
# End Source File
# Begin Source File

SOURCE=..\xitools\cfg_freevar.c
# End Source File
# Begin Source File

SOURCE=..\xitools\cfg_isbool.c
# End Source File
# Begin Source File

SOURCE=..\xitools\cfg_loadprefs.c
# End Source File
# Begin Source File

SOURCE=..\xitools\cfg_mapfromenv.c
# End Source File
# Begin Source File

SOURCE=..\xitools\cfg_newsection.c
# End Source File
# Begin Source File

SOURCE=..\xitools\cfg_newvar.c
# End Source File
# Begin Source File

SOURCE=..\xitools\cfg_saveprefs.c
# End Source File
# Begin Source File

SOURCE=..\xitools\errors.c
# End Source File
# Begin Source File

SOURCE=..\xitools\file_getsize.c
# End Source File
# Begin Source File

SOURCE=..\xitools\files.c
# End Source File
# Begin Source File

SOURCE=..\xitools\idtostr.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_check.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_close.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_correctchunk.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_endchunk.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_fixsize.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_goodid.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_goodtype.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_new.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_newchunk.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_open.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_readchunkdata.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_readchunkheader.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_rewind.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_skipchunkdata.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iff_writechunkdata.c
# End Source File
# Begin Source File

SOURCE=..\xitools\iffp_new.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_add.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_addhead.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_delete.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_free.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_getnode.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_insert.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_makecircular.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_new.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_newitem.c
# End Source File
# Begin Source File

SOURCE=..\xitools\list_remove.c
# End Source File
# Begin Source File

SOURCE=..\xitools\md5sum.c
# End Source File
# Begin Source File

SOURCE=..\xitools\node_end.c
# End Source File
# Begin Source File

SOURCE=..\xitools\node_findbycontents.c
# End Source File
# Begin Source File

SOURCE=..\xitools\node_findbyname.c
# End Source File
# Begin Source File

SOURCE=..\xitools\node_makecircular.c
# End Source File
# Begin Source File

SOURCE=..\xitools\node_start.c
# End Source File
# Begin Source File

SOURCE=..\xitools\str_getvalue.c
# End Source File
# Begin Source File

SOURCE=..\xitools\strfind.c
# End Source File
# Begin Source File

SOURCE=..\xitools\strisnum.c
# End Source File
# Begin Source File

SOURCE=..\xitools\strlower.c
# End Source File
# Begin Source File

SOURCE=..\xitools\strsplit.c
# End Source File
# Begin Source File

SOURCE=..\xitools\strtokenise.c
# End Source File
# Begin Source File

SOURCE=..\xitools\strupper.c
# End Source File
# Begin Source File

SOURCE=..\xitools\tag_alloclist.c
# End Source File
# Begin Source File

SOURCE=..\xitools\tag_finditem.c
# End Source File
# Begin Source File

SOURCE=..\xitools\tag_freelist.c
# End Source File
# Begin Source File

SOURCE=..\xitools\tag_getdata.c
# End Source File
# Begin Source File

SOURCE=..\xitools\tag_next.c
# End Source File
# End Target
# End Project
