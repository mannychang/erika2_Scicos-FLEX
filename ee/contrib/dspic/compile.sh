export EE_INSTALLDIR=`cygpath -ms \`reg query HKEY_LOCAL_MACHINE\\\\\\\\SOFTWARE\\\\\\\\Evidence /v EE_InstallPath | awk  '/EE_InstallPath/{ print $3 }'\` | xargs cygpath`

echo --------------------------------------------------------
echo Step 1: Template Generation
echo
echo RT-Druid is generating the scheleton application which 
echo will be compiled together with the Scicos generated code
echo --------------------------------------------------------

${EE_INSTALLDIR}/bin/template_launcher.bat --template $1 --output .

cat conf_scicos.oil | gcc -c - -E -P -o conf.oil

echo --------------------------------------------------------
echo Step 2: Parsing OIL File
echo
echo RT-Druid is parsing the OIL file to generate the 
echo makefiles used for the compilation 
echo --------------------------------------------------------
${EE_INSTALLDIR}/bin/rtdruid_launcher.bat --oil conf.oil

echo --------------------------------------------------------
echo Step 3: Compiling the application
echo
echo The application is now compiled 
echo --------------------------------------------------------
cd Debug
make

echo --------------------------------------------------------
echo DONE... Application successfully compiled!
echo
echo The ELF file to be programmed in MPLABIDE is the file
echo Debug/pic30.elf
echo --------------------------------------------------------
