@echo off

rem 

rem ***************************************************************
rem * 
rem * 
rem ***************************************************************
set EE_BASH_PATH=C:\cygwin\bin\bash.exe

if not exist %EE_BASH_PATH% goto error

:ok

%EE_BASH_PATH% -c "/bin/bash --login -c \"cd `/bin/cygpath/ -ms \"$PWD\"`; ./flash.sh\""

goto end

:error
echo *** ERROR:
echo ***  The flash.bat script can not find the Cygwin BASH shell.
echo ***  Please change the EE_BASH_PATH environment variable inside
echo ***  flash.bat
pause

:end
