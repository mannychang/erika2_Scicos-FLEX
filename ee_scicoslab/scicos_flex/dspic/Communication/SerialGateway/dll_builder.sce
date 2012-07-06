// Script to build the SerialGateway dynamic library.
// Author: Errico Guidieri, Evidence Srl.

VC_BUILD_EXE = ascii(34) + 'vcbuild.exe' + ascii(34) + ' ';
VC_BUILD_OPT = '/r ';
VC_BUILD_PROJ = get_absolute_file_path('dll_builder.sce') + 'SerialGateway.vcproj ';
VC_BUILD_PERSPECTIVE = 'release ';

cmd = VC_BUILD_EXE + VC_BUILD_OPT + VC_BUILD_PROJ + VC_BUILD_PERSPECTIVE;
unix( cmd );
