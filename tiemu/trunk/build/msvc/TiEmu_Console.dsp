# Microsoft Developer Studio Project File - Name="TiEmu_Console" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TiEmu_Console - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TiEmu_Console.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TiEmu_Console.mak" CFG="TiEmu_Console - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TiEmu_Console - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TiEmu_Console - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TiEmu_Console - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "E:\msvc\Output\gtktiemu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /w /W0 /GX /O2 /I "..\..\ti_libs\cables\src" /I "..\..\ti_libs\calcs\src" /I "..\..\ti_libs\libti68k\src\\" /I "..\dep\win32" /D "NDEBUG" /D "HAVE_LIBINTL_H" /D "ENABLE_NLS" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "PENT_COUNTER" /D "HAVE_LIBJPEG" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib gnu-intl.lib glib-1.3.lib gdk-1.3.lib gtk-1.3.lib gmodule-1.3.lib /nologo /subsystem:console /machine:I386 /out:"Test\GtkTiEmu.exe"

!ELSEIF  "$(CFG)" == "TiEmu_Console - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "C:\msvc\Output\tiemu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /GX /ZI /Od /I "." /I "../../../ticables/src" /I "../../../tifiles/src" /I "../../../ticalcs/src" /I "../../src/win32" /I "C:\GTK2DEV\INCLUDE\GTK-2.0" /I "C:\GTK2DEV\LIB\GTK-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\GLIB-2.0" /I "C:\GTK2DEV\LIB\GLIB-2.0\INCLUDE" /I "C:\GTK2DEV\INCLUDE\PANGO-1.0" /I "C:\GTK2DEV\INCLUDE\ATK-1.0" /I "c:\gtk2dev\include" /I "C:\Gtk2Dev\include\libglade-2.0" /I "C:\sources\roms\tiemu\src\core\uae" /I "C:\sources\roms\tiemu\src\core" /I "C:\sources\roms\tiemu\src\core\ti_hw" /I "C:\sources\roms\tiemu\src\hid" /I "C:\sources\roms\tiemu\src\gui" /I ".\libjpeg" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "DEBUGGER" /D "PENT_COUNTER" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"C:\msvc\tilp\TiEmu.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TiEmu_Console - Win32 Release"
# Name "TiEmu_Console - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "core"

# PROP Default_Filter ""
# Begin Group "ti_hw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\ti_hw\debugger.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\hardware.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\ioports.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\keyboard.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\lcd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\linkport.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\m68k.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\memory.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\ti_hw\timer.c
# End Source File
# End Group
# Begin Group "uae"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\uae\cpu0.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu1.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu2.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu3.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu4.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu5.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu6.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu7.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu8.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpu9.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuC.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuD.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpudefs.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuE.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpuF.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\cpustbl.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\newcpu.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\uae\readcpu.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\core\bkpts.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\callbacks.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\core.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\errors.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\images3.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\interface.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\intlist.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\state.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\type2str.c
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\gui\about.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\adbkpt_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\adbkpt_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\bkpt_cause.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\comm.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\dbgopt_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\dbgopt_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\dbkpt_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\dbkpt_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\dboxes.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debug_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\debug_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\filesel.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\fs_misc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\help.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\infos.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\logaddr_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\logaddr_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\manpage.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\pbars.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\pgmentbkpt_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\pgmentbkpt_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\popup.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\refresh.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\release.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\romversion.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\scroptions.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\splash.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\support.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\trap_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\trap_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\viewlog_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\viewlog_dbox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\wizard.c
# End Source File
# End Group
# Begin Group "hid"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\hid\cmdline.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\hid.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\paths.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\printl.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\rcfile.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\skinops.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\tie_error.c
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\tikeys.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\gui\main.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\hid\cmdline.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\extern.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\files.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\intl.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\paths.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\printl.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\rcfile.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\screenshot.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\skin.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\struct.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\ticalc.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\tie_error.h
# End Source File
# Begin Source File

SOURCE=..\..\src\hid\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\TiEmu.ico
# End Source File
# Begin Source File

SOURCE=.\TiEMu.rc
# End Source File
# End Group
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\SDL-1.2.7\lib\SDL.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\glib-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gdk-win32-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gtk-win32-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gobject-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gdk_pixbuf-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gthread-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\gmodule-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\pango-1.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\libglade-2.0.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Gtk2Dev\lib\atk-1.0.lib"
# End Source File
# Begin Source File

SOURCE=.\libjpeg.lib
# End Source File
# End Group
# End Target
# End Project
