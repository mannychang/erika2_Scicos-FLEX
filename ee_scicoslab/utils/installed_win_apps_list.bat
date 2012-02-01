@echo off
PATH=%PATH%;C:\Programmi\gnuwin32\bin;c:\cygwin\bin
C:\WINDOWS\regedit /a %TEMP%\install.reg "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
sed -f cmd.sed %temp%\install.reg