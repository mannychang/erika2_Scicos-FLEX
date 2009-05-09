mode(-1);
// specific part
libname='dspic' // name of scilab function library [CUSTOM]
libname_fb='dspic_fb' // name of scilab function library [CUSTOM]
libname_cg='dspic_cg' // name of scilab function library [CUSTOM]

//** It is a better function to recover the absolute path information 
DIR = get_absolute_file_path('builder.sce')

if ~MSDOS then // Unix Linux
  if part(DIR,1)<>'/' then DIR=getcwd()+'/'+DIR,end
  MACROS=DIR+'macros/' // Path of the macros directory
  CG_MACROS=MACROS+'codegen/' // Path of the macros directory
  FB_MACROS=MACROS+'flex_blocks/' // Path of the macros directory
  ROUTINES = DIR+'routines/' 
else  // windows- Visual C++
  if part(DIR,2)<>':' then DIR=getcwd()+'\'+DIR,end
  MACROS=DIR+'macros\' // Path of the macros directory
  CG_MACROS=MACROS+'codegen\' // Path of the macros directory
  FB_MACROS=MACROS+'flex_blocks\' // Path of the macros directory
end

//compile sci files if necessary and build lib file
genlib(libname,MACROS)
genlib(libname_fb,FB_MACROS)
genlib(libname_cg,CG_MACROS)
