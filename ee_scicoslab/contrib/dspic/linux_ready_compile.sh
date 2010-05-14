#
# PLEASE CHANGE THIS VARIABLE BELOW TO THE CORRECT INSTALLATION PATH
#
if test `uname|cut -c 1-6` == "CYGWIN"; then
# Cygwin environment
export ECLIPSE_HOME=`cygpath -ms "C:\Evidence\Evidence\eclipse"`
else
# Linux environment
export ECLIPSE_HOME="~/eclipse"
fi
#########################################################################################
#
# NO CHANGES BELOW THIS LINE!
#
#########################################################################################

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

#`cygpath -ms ${ECLIPSE_HOME}`/Evidence/template_launcher.bat --template $1 --output .
${ECLIPSE_HOME}/Evidence/start_ant.sh -buildfile ${ECLIPSE_HOME}/Evidence/build.xml -Dexample_id=$1 -Dexamples_output_dir=. an_example

echo --------------------------------------------------------
echo Step 2: Parsing OIL File
echo
echo RT-Druid is parsing the OIL file to generate the 
echo makefiles used for the compilation 
echo --------------------------------------------------------

cat conf_scicos.oil | gcc -c - $(cat scicos_symbols) -E -P -o conf.oil

#`cygpath -ms ${ECLIPSE_HOME}`/Evidence/rtd_launcher.bat --oil conf.oil
${ECLIPSE_HOME}/Evidence/start_ant.sh -buildfile ${ECLIPSE_HOME}/Evidence/build.xml -Din_conf_oil=conf.oil -Dconf_output_dir=Debug compile

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
if test `uname|cut -c 1-6` == "CYGWIN"; then
# Cygwin environment
echo use MPLAB IDE from Microchip 
echo or
echo run the flash.bat script [ICD3 only].
else
# Linux environment
echo use MPLAB IDE from Microchip on Windows
echo or
echo pray ad libitum.
fi
echo --------------------------------------------------------

cd ..
