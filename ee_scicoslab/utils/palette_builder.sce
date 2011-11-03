// ***********************************************************
// * ScicosLab Pack Installer                                *
// * Requires: Scicoslab v4.4.1                              *
// * Built/Tested On: XP, Windows 7                          *
// * Description: Palettes builder                           *
// * Copyright (c) 2011 Evidence Srl                         *
// * Author: Dario Di Stefano, Evidence                      *
// ***********************************************************

// Set global variables
OLDDIR = pwd();
SCIDIR = strsubst(SCI,'/','\');

cd(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic");

exec builder.sce
// Create and move Scicos EE pack palettes
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\AMAZING");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-Communication");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-DMB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-MTB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\RT-Data-Exchange");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\EASYLAB");

cd(SCIDIR+"\contrib\scicos_ee\utils");

cmd = 'start cp_cosf.bat ' + ascii(34)+SCIDIR+ascii(34);
unix(cmd);

cd(OLDDIR);





