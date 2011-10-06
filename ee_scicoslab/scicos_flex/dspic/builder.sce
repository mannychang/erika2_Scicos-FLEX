mode(-1);
// specific part
libname='dspic' // name of scilab function library [CUSTOM]
libname_fb='dspic_fb' // name of scilab function library [CUSTOM]
libname_cg='dspic_cg' // name of scilab function library [CUSTOM]
libname_fb_FLEX='fb_FLEX' 
libname_fb_communication='fb_communication'
libname_fb_DMB='fb_DMB'
libname_fb_MTB='fb_MTB'
libname_fb_pc='fb_pc'
libname_amazing='amazing'
libname_misc='misc'
libname_easylab='easylab'
libname_gif_icons = 'gif_icons'

//** It is a better function to recover the absolute path information 
DIR = get_absolute_file_path('builder.sce')

if ~MSDOS then // Unix Linux
  if part(DIR,1)<>'/' then DIR=getcwd()+'/'+DIR,end
  MACROS=DIR+'macros/' // Path of the macros directory
  CG_MACROS=MACROS+'codegen/'
  FB_MACROS=MACROS+'flex_blocks/'
  FB_FLEX=FB_MACROS+'FLEX/'
  FB_COMMUNICATION=FB_MACROS+'FLEX-Communication/'
  FB_DMB=FB_MACROS+'FLEX-DMB/'
  FB_MTB=FB_MACROS+'FLEX-MTB/'
  FB_PC=FB_MACROS+'FLEX-PC/'
  FB_PC=FB_MACROS+'FLEX-PC/'
  FB_AMAZING=FB_MACROS+'AMAZING/'
  FB_EASYLAB=FB_MACROS+'EASYLAB/'
  MISC=MACROS+'misc/'
  ROUTINES = DIR+'routines/' 
else  // windows- Visual C++
  if part(DIR,2)<>':' then DIR=getcwd()+'\'+DIR,end
  MACROS=DIR+'macros\' // Path of the macros directory
  CG_MACROS=MACROS+'codegen\'
  FB_MACROS=MACROS+'flex_blocks\' 
  FB_FLEX=FB_MACROS+'FLEX\'
  FB_COMMUNICATION=FB_MACROS+'FLEX-Communication\'
  FB_DMB=FB_MACROS+'FLEX-DMB\'
  FB_MTB=FB_MACROS+'FLEX-MTB\'
  FB_PC=FB_MACROS+'FLEX-PC\'
  FB_AMAZING=FB_MACROS+'AMAZING\'
  FB_EASYLAB=FB_MACROS+'EASYLAB/'
  MISC=MACROS+'\misc\'
  GIF_ICONS=MACROS+'\gif_icons\'
end

//compile sci files if necessary and build lib file
//genlib(libname,MACROS)
genlib(libname_cg,CG_MACROS)
genlib(libname_fb_FLEX,FB_FLEX)
genlib(libname_fb_communication,FB_COMMUNICATION)
genlib(libname_fb_DMB,FB_DMB)
genlib(libname_fb_MTB,FB_MTB)
genlib(libname_fb_pc,FB_PC)
genlib(libname_amazing,FB_AMAZING)
genlib(libname_misc,MISC)
genlib(libname_easylab,FB_EASYLAB)
genlib(libname_gif_icons,GIF_ICONS)
