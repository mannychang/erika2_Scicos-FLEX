// Script to build the Flex-Sim dynamic library.
// Author: Dario Di Stefano, Evidence Srl.

VC_BUILD_EXE = ascii(34) + 'C:\Programmi\Microsoft Visual Studio 9.0\VC\vcpackages\vcbuild.exe' + ascii(34) + ' ';
VC_BUILD_OPT = '/r ';
VC_BUILD_PROJ = SCI + '/contrib/scicos_ee/scicos_flex/dspic/Flex-UDP/FlexUDPComms.vcproj ';
VC_BUILD_PERSPECTIVE = 'release ';

cmd = VC_BUILD_EXE + VC_BUILD_OPT + VC_BUILD_PROJ + VC_BUILD_PERSPECTIVE;
unix( cmd );
