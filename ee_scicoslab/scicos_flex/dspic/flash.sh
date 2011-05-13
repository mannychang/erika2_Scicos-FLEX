export EE_MCHPDIR=$(cygpath -ms "`regtool get /HKLM/SOFTWARE/Microchip/MPLAB\ IDE/InstallDir`" | xargs cygpath)

${EE_MCHPDIR}/Programmer\ Utilities/ICD3/ICD3CMD -P33FJ256MC710 -FDebug/pic30.cof -M