

//==========================================================================

function Makename=rt_gen_make(name,files,libs)

//** Code execution tracing and profiling
global %tb ;
%tb = [%tb;"rt_gen_make"];
//**

  Makename=rpat+'/Makefile';

  T=mgetl(TARGETDIR+'/'+makfil);
  T=strsubst(T,'$$MODEL$$',name);
  T=strsubst(T,'$$OBJ$$',strcat(files+'.o',' '));
  T=strsubst(T,'$$SCILAB_DIR$$',SCI);
  mputl(T,Makename)

endfunction


