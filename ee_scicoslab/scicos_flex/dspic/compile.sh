#
# PLEASE CHANGE THIS VARIABLE BELOW TO THE CORRECT INSTALLATION PATH
#
export RTDRUID_BASEDIR=$1/evidence

#########################################################################################
#
# NO CHANGES BELOW THIS LINE!
#
#########################################################################################

#export RTDRUID_BASEDIR_MS=`cygpath -ms ${RTDRUID_BASEDIR}`

if test -d ${RTDRUID_BASEDIR}; then
  echo RTDRUID_BASEDIR directory found.
else
  echo ---------------------------------------------------------------
  echo WARNING 
  echo
  echo Compilation not performed!
  echo
  echo Please change the compile.sh script into the
  echo contrib/scicos_ee/scicos_flex/dspic of the Scilab/Scicos installation 
  echo directory, providing the right value for RTDRUID_BASEDIR.
  echo ---------------------------------------------------------------
  exit 1;
fi

echo --------------------------------------------------------
echo Step 1: Template Generation
echo
echo RT-Druid is generating the scheleton application which 
echo will be compiled together with the Scicos generated code
echo --------------------------------------------------------
`cygpath -ms ${RTDRUID_BASEDIR}`/instantiate_template.bat `cygpath -d $1` $2 .
cat conf_scicos.oil | gcc -c - $(cat scicos_symbols) -E -P -o conf.oil

echo --------------------------------------------------------
echo Step 2: Parsing OIL File
echo
echo RT-Druid is parsing the OIL file to generate the 
echo makefiles used for the compilation 
echo --------------------------------------------------------
`cygpath -ms ${RTDRUID_BASEDIR}`/generate_code.bat `cygpath -d $1` conf.oil Debug

echo --------------------------------------------------------
echo Step 3: Compiling the application
echo
echo The application is now compiled 
echo --------------------------------------------------------
cd Debug
if [ "$EEBASE" ]; then
    export EEBASE="`cygpath -u "$EEBASE"`"
fi
make 2>&1
#Make error check
if [ "$?" !=  "0" ]; then
  echo --------------------------------------------------------   
  echo \!\!\! Compiling ERROR \!\!\!
  echo --------------------------------------------------------
  cd ..
  exit 1
fi

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