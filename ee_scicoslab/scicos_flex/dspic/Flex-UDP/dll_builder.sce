// Script to build the UDP dynamic library.
// Author: Dario Di Stefano, Evidence Srl.

VC_BUILD_EXE = ascii(34) + 'vcbuild.exe' + ascii(34) + ' ';
VC_BUILD_OPT = '/r ';
VC_BUILD_PROJ = get_absolute_file_path('dll_builder.sce') + 'FlexUDPComms.vcproj ';
VC_BUILD_PERSPECTIVE = 'release ';

cmd = VC_BUILD_EXE + VC_BUILD_OPT + VC_BUILD_PROJ + VC_BUILD_PERSPECTIVE;
unix( cmd );
clear cmd;
