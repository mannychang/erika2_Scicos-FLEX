//==========================================================================

// Modified for RT purposes by Roberto Bucher - RTAI Team
// roberto.bucher@supsi.ch

function Makename=rt_gen_make(name,files,libs)

  Makename=rpat+'/Makefile';

  T=mgetl(TARGETDIR+'/'+makfil);
  T=strsubst(T,'$$MODEL$$',name);
  T=strsubst(T,'$$OBJ$$',strcat(files+'.o',' '));
  T=strsubst(T,'$$SCILAB_DIR$$',SCI);
  mputl(T,Makename)

endfunction


