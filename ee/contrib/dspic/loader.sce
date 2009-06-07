mode(-1);
// specific part
libname='rtai' // name of scilab function library [CUSTOM]
DIR = get_absolute_file_path('loader.sce');

// You should also modify the  ROUTINES/loader.sce with the new 
// Scilab primitive for the path

//dspiclib = lib(DIR+'/macros/')
dspiclib_cg = lib(DIR+'/macros/codegen/')
dspiclib_fb = lib(DIR+'/macros/flex_blocks/')
dspiclib_fb_sources = lib(DIR+'/macros/flex_blocks/FLEX-Sources/')
dspiclib_fb_sinks = lib(DIR+'/macros/flex_blocks/FLEX-Sinks/')
dspiclib_fb_comm = lib(DIR+'/macros/flex_blocks/FLEX-Communication/')
dspiclib_fb_pc = lib(DIR+'/macros/flex_blocks/FLEX-PC/')
dspiclib_misc = lib(DIR+'/macros/misc/')
