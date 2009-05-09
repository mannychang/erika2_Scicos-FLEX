mode(-1);
// specific part
libname='rtai' // name of scilab function library [CUSTOM]
DIR = get_absolute_file_path('loader.sce');

// You should also modify the  ROUTINES/loader.sce with the new 
// Scilab primitive for the path

dspiclib = lib(DIR+'/macros/')
dspiclib_fb = lib(DIR+'/macros/flex_blocks/')
dspiclib_cg = lib(DIR+'/macros/codegen/')
