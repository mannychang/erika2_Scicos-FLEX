export EE_INSTALLDIR=`cygpath -ms \`reg query HKEY_LOCAL_MACHINE\\\\\\\\SOFTWARE\\\\\\\\Evidence /v EE_InstallPath | awk  '/EE_InstallPath/{ print $3 }'\` | xargs cygpath`

echo Template Generation
${EE_INSTALLDIR}/bin/template_launcher.bat --template pic30_empty_scicos --output .

echo Parsing OIL File
${EE_INSTALLDIR}/bin/rtdruid_launcher.bat --oil conf.oil

echo Compiling the application
cd Debug
make
