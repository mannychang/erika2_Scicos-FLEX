mode(-1);
// specific part
libname='dspic' // name of scilab function library [CUSTOM]
libname_fb='dspic_fb' // name of scilab function library [CUSTOM]
libname_cg='dspic_cg' // name of scilab function library [CUSTOM]
libname_fb_sources='fb_sources' 
libname_fb_sinks='fb_sinks'
libname_fb_pc='fb_pc'
libname_misc='misc'

//** It is a better function to recover the absolute path information 
DIR = get_absolute_file_path('builder.sce')

if ~MSDOS then // Unix Linux
  if part(DIR,1)<>'/' then DIR=getcwd()+'/'+DIR,end
  MACROS=DIR+'macros/' // Path of the macros directory
  CG_MACROS=MACROS+'codegen/'
  FB_MACROS=MACROS+'flex_blocks/'
  FB_SOURCES=FB_MACROS+'/FLEX-Sources/'
  FB_SINKS=FB_MACROS+'/FLEX-Sinks/'
  FB_PC=FB_MACROS+'/FLEX-PC/'
  MISC=MACROS+'/misc/'
  ROUTINES = DIR+'routines/' 
else  // windows- Visual C++
  if part(DIR,2)<>':' then DIR=getcwd()+'\'+DIR,end
  MACROS=DIR+'macros\' // Path of the macros directory
  CG_MACROS=MACROS+'codegen\'
  FB_MACROS=MACROS+'flex_blocks\' 
  FB_SOURCES=FB_MACROS+'\FLEX-Sources\'
  FB_SINKS=FB_MACROS+'\FLEX-Sinks\'
  FB_PC=FB_MACROS+'\FLEX-PC\'
  MISC=MACROS+'\misc\'
end

//compile sci files if necessary and build lib file
//genlib(libname,MACROS)
genlib(libname_fb,FB_MACROS)
genlib(libname_cg,CG_MACROS)
genlib(libname_fb_sources,FB_SOURCES)
genlib(libname_fb_sinks,FB_SINKS)
genlib(libname_fb_pc,FB_PC)
genlib(libname_misc,MISC)

