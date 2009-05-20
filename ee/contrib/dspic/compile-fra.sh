#export EE_INSTALLDIR=`cygpath -ms \`reg query HKEY_LOCAL_MACHINE\\\\\\\\SOFTWARE\\\\\\\\Evidence /v EE_InstallPath | awk  '/EE_InstallPath/{ print $3 }'\` | xargs cygpath`

export EE_INSTALLDIR=$(cygpath -ms "`regtool get /HKLM/SOFTWARE/Evidence/EE_InstallPath`" | xargs cygpath)

export EE_MCHPDIR=$(cygpath -ms "`regtool get /HKLM/SOFTWARE/Microchip/MPLAB\ IDE/InstallDir`" | xargs cygpath)

echo --------------------------------------------------------
echo Step 1: Template Generation
echo
echo RT-Druid is generating the scheleton application which 
echo will be compiled together with the Scicos generated code
echo --------------------------------------------------------

${EE_INSTALLDIR}/bin/template_launcher.bat --template $1 --output .

#cat conf_scicos.oil | gcc -c - -E -P -o conf.oil
cat conf_scicos.oil | gcc -c - $(cat scicos_symbols) -E -P -o conf.oil

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
echo The COF file being programmed automatically though 
echo Microchip ICD3 is Debug/pic30.cof
echo --------------------------------------------------------

${EE_MCHPDIR}/Programmer\ Utilities/ICD3/ICD3CMD -P33FJ256MC710 -Fpic30.cof -M

cd ..