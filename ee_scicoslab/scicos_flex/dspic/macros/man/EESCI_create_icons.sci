// ***********************************************************
// * ScicosLab Pack Installer                                *
// * Requires: Scicoslab v4.4.1                              *
// * Built/Tested On: XP, Windows 7                          *
// * Description: GIF icons creation                         *
// * Copyright (c) 2011 Evidence Srl                         *
// * Author: Dario Di Stefano, Evidence                      *
// ***********************************************************

function EESCI_create_icons()

OLDDIR = pwd();
SCIDIR = strsubst(SCI,'/','\');
MYDIR = SCIDIR + '\contrib\scicos_ee\scicos_flex\dspic';
GIF_DIR = SCIDIR + '\contrib\scicos_ee\scicos_flex\dspic\macros\man\';

// MISC
txt = MYDIR + '\macros\misc\FLEX_randm.sci';
txt = [txt; MYDIR + '\macros\misc\FLEX_square.sci'];
txt = [txt; MYDIR + '\macros\misc\FLEX_sinus.sci'];
txt = [txt; MYDIR + '\macros\misc\FLEX_step.sci'];
EESCI_gen_gif_selected( txt, GIF_DIR+'misc' );

// NativeInteger
txt = MYDIR + '\NativeInteger\NAT_GAINBLK.sci';
txt = [txt; MYDIR + '\NativeInteger\NAT_SUMMATION.sci'];
EESCI_gen_gif_selected( txt, GIF_DIR+'NativeInteger' );

cd(OLDDIR)
return

// flex_blocks: AMAZING
fpath = MYDIR + '\macros\flex_blocks\AMAZING';
cd(fpath);
EESCI_gen_gif(GIF_DIR+'amazing');

// flex_blocks: EASYLAB
fpath = MYDIR + '\macros\flex_blocks\EASYLAB';
cd(fpath);
EESCI_gen_gif(GIF_DIR+'easylab');

// flex_blocks: FLEX
fpath = MYDIR + '\macros\flex_blocks\FLEX';
cd(fpath);
EESCI_gen_gif(GIF_DIR+'flex');

// flex_blocks: FLEX-Communication
fpath = MYDIR + '\macros\flex_blocks\FLEX-Communication';
cd(fpath);
EESCI_gen_gif(GIF_DIR+'flex_com');

// flex_blocks: FLEX-DMB
fpath= MYDIR + '\macros\flex_blocks\FLEX-DMB';
cd(fpath);
EESCI_gen_gif(GIF_DIR+'flex_dmb');

// flex_blocks: FLEX-MTB
fpath = MYDIR + '\macros\flex_blocks\FLEX-MTB';
cd(fpath);
EESCI_gen_gif(GIF_DIR+'flex_mtb');

// flex_blocks: FLEX-PC
fpath = MYDIR + '\macros\flex_blocks\FLEX-PC';
cd(fpath);
EESCI_gen_gif(GIF_DIR+'flex_pc');

cd(OLDDIR)

endfunction

function EESCI_gen_gif( pth )
	res = isdir(pth);
	if res==%F
		unix('mkdir '+pth);
	end
	if ~MSDOS then // Unix Linux
		disp('linux')
		unix('ls -1 *.sci > x.x');
	else
		disp('win32')
		unix('dir /B *.sci > x.x');
	end
	txt = mgetl('x.x');
	create_gif(txt, pth);
	mdelete x.x;
endfunction

function EESCI_gen_gif_selected( txt, pth )
	res = isdir(pth);
	if res==%F
		unix('mkdir '+pth);
	end
	create_gif(txt, pth);
endfunction

