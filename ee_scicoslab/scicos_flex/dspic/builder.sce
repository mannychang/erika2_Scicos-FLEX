mode(-1);
// specific part
libname='dspic' // name of scilab function library [CUSTOM]
libname_fb='dspic_fb' // name of scilab function library [CUSTOM]
libname_cg='dspic_cg' // name of scilab function library [CUSTOM]
libname_fb_FLEX='fb_FLEX' 
libname_fb_communication='fb_communication'
libname_fb_DMB='fb_DMB'
libname_fb_MTB='fb_MTB'
libname_RT_Data_Exchange='RT_Data_Exchange'
libname_amazing='amazing'
libname_misc='misc'
libname_fb_misc='fb_misc'
libname_easylab='easylab'
libname_gif_icons = 'gif_icons'
libname_cg_sources = 'cg_sources'

//** It is a better function to recover the absolute path information 
DIR = get_absolute_file_path('builder.sce')

if ~MSDOS then // Unix Linux
  if part(DIR,1)<>'/' then DIR=getcwd()+'/'+DIR,end
  ROUTINES = DIR+'routines/' 
else  // windows- Visual C++
  if part(DIR,2)<>':' then DIR=getcwd()+'\'+DIR,end
end

MACROS =           DIR+'macros/';       
FB_MACROS =        MACROS+'flex_blocks/'          

//compile sci files if necessary and build lib file
genlib(libname_cg,MACROS+'codegen/')
genlib(libname_fb_FLEX,FB_MACROS+'FLEX/')
genlib(libname_fb_communication,FB_MACROS+'FLEX-Communication/')
genlib(libname_fb_DMB,FB_MACROS+'FLEX-DMB/')
genlib(libname_fb_MTB,FB_MACROS+'FLEX-MTB/')
genlib(libname_RT_Data_Exchange,FB_MACROS+'RT-Data-Exchange/')
genlib(libname_amazing,FB_MACROS+'AMAZING/')
genlib(libname_misc,MACROS+'misc/')
genlib(libname_fb_misc,FB_MACROS+'MISC/')
genlib(libname_easylab,FB_MACROS+'EASYLAB/')
genlib(libname_gif_icons,MACROS+'man/')
genlib(libname_cg_sources,FB_MACROS+'CG-Sources/')

// Flex Demo Board simulation (QT executable)
[x_x_x,dmbexe_err] = fileinfo(SCI + '/contrib/scicos_ee/bin/FlexDemoBoard.exe');
if dmbexe_err == 0
	setenv('FLEXPATH',   SCI + '/contrib/scicos_ee/bin/FlexDemoBoard.exe');
end

// SMCube
libname_smcube = 'smcube'
genlib(libname_smcube,FB_MACROS+'SMCube/')
[x_x_x,smcexe_err] = fileinfo(SCI + "/contrib/scicos_ee/bin/SMCube.exe");
if smcexe_err == 0
	setenv('SMCUBEPATH', SCI + '/contrib/scicos_ee/bin');
end

clear x_x_x smcexe_err FB_MACROS MACROS;
