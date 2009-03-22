
function [files]=write_code(Code,CCode,FCode)

//** Code execution tracing and profiling
global %tb ;
%tb = [%tb;"write_code"];
//**
  
 ierr=execstr('mputl(Code,rpat+''/''+rdnom+''.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

  if FCode<>[] then
    ierr=execstr('mputl(FCode,rpat+''/''+rdnom+''f.f'')','errcatch')
    if ierr<>0 then
      message(lasterror())
      ok=%f
      return
    end
  end

  if CCode<>[] then
    CCode = [
          '#include <math.h>';
          '#include <stdlib.h>';
          '#include <scicos/scicos_block4.h>';
	  '';
	  CCode];
    ierr=execstr('mputl(CCode,rpat+''/''+rdnom+''_Cblocks.c'')','errcatch')
    if ierr<>0 then
      message(lasterror())
      ok=%f
      return
    end
  end

  files=[]
  [fd,ierr]=mopen(rpat+'/'+rdnom+'f.f','r')
  if ierr==0 then mclose(fd),files=[files,rdnom+'f'],end
  [fd,ierr]=mopen(rpat+'/'+rdnom+'_Cblocks.c','r')
  if ierr==0 then mclose(fd),files=[files,rdnom+'_Cblocks'],end

  //** copy source code of machine.h/scicos_block4.h
  //   in target path
  txt=mgetl(SCI+'/routines/machine.h');
  ierr=execstr('mputl(txt,rpat+''/machine.h'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

  txt=mgetl(SCI+'/routines/scicos/scicos_block4.h');
  ierr=execstr('mputl(txt,rpat+''/scicos_block4.h'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

endfunction

//==========================================================================
