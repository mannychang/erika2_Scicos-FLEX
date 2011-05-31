@echo off

rem ***************************************************************
rem * Are you here because the script can not find the bash shell?
rem * Please change the following line!
rem ***************************************************************
set EE_BASH_PATH=C:\cygwin\bin\bash.exe
if not exist %EE_BASH_PATH% goto error

if [%1]==[] goto help
if [%2]==[] goto help

:ok
set SHELLOPTS=igncr
%EE_BASH_PATH% -c "/bin/bash --login -c \"cd `/bin/cygpath/ -ms \"$PWD\"`; ./compile.sh %1 %2\""
goto end

:error
echo *** ERROR:
echo ***  The compile.bat script can not find the Cygwin BASH shell.
echo ***  Please change the EE_BASH_PATH environment variable inside compile.bat
pause
goto end

:help
echo *** HELP:
echo ***  Launch the compilation of the project.
echo ***  Usage: compile.bat RT_DRUID_PATH PROJECT_TEMPLATE
echo ***  example: compile.bat "C:/PROGRA~1/scicoslab/SCICOS~1.1/contrib/scicos_ee/RT-Druid" board_flex
pause
goto end

:end
