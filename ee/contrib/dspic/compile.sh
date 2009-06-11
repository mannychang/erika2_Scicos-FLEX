#
# PLEASE CHANGE THIS VARIABLE BELOW TO THE CORRECT INSTALLATION PATH
#
export ECLIPSE_HOME="C:\Evidence\Evidence\eclipse"

#########################################################################################
#
# NO CHANGES BELOW THIS LINE!
#
#########################################################################################

#export ECLIPSE_HOME_MS=`cygpath -ms ${ECLIPSE_HOME}`

if test -d ${ECLIPSE_HOME}; then
  echo ECLIPSE_HOME directory found.
else
  echo ---------------------------------------------------------------
  echo WARNING 
  echo
  echo Compilation not performed!
  echo
  echo Please change the compile.sh script into the
  echo contrib/dspic of the Scilab/Scicos installation 
  echo directory, providing the right value for ECLIPSE_HOME.
  echo ---------------------------------------------------------------
  exit 1;
fi

echo --------------------------------------------------------
echo Step 1: Template Generation
echo
echo RT-Druid is generating the scheleton application which 
echo will be compiled together with the Scicos generated code
echo --------------------------------------------------------

`cygpath -ms ${ECLIPSE_HOME}`/Evidence/template_launcher.bat --template $1 --output .

#cat conf_scicos.oil | gcc -c - -E -P -o conf.oil
cat conf_scicos.oil | gcc -c - $(cat scicos_symbols) -E -P -o conf.oil

echo --------------------------------------------------------
echo Step 2: Parsing OIL File
echo
echo RT-Druid is parsing the OIL file to generate the 
echo makefiles used for the compilation 
echo --------------------------------------------------------
`cygpath -ms ${ECLIPSE_HOME}`/Evidence/rtd_launcher.bat --oil conf.oil

echo --------------------------------------------------------
echo Step 3: Compiling the application
echo
echo The application is now compiled 
echo --------------------------------------------------------
cd Debug
make

echo --------------------------------------------------------
echo --------------------------------------------------------
echo DONE... Application successfully compiled!
echo --------------------------------------------------------
echo --------------------------------------------------------
echo 
echo To program your FLEX board with the COF file
echo Debug/pic30.cof you can:
echo
echo 1- use MPLAB IDE from Microchip 
echo
echo 2- run the flash.bat script [ICD3 only]
echo --------------------------------------------------------

cd ..
