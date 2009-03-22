
//==========================================================================

function ok = compile_standalone()
//compile rt standalone executable for standalone
// 22.01.2004
//Author : Roberto Bucher (roberto.bucher@die.supsi.ch)


//** Code execution tracing and profiling
global %tb ;
%tb = [%tb;"compile_standalone"];
//**

  xinfo('Compiling standalone');
  wd = pwd();
  chdir(rpat);

  if getenv('WIN32','NO')=='OK' then
     unix_w('nmake -f Makefile.mak');
  else
     unix_w('make')
  end
  chdir(wd);
  ok = %t;
endfunction	

//==========================================================================
