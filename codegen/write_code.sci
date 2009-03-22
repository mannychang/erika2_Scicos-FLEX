
function [files]=write_code(Code,CCode,FCode,Code_common)

// Original file from Project Metalau - INRIA
// Modified for RT purposes by Roberto Bucher - RTAI Team
// roberto.bucher@supsi.ch

 ierr=execstr('mputl(Code,rpat+''/''+rdnom+''.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

 ierr=execstr('mputl(Code_common,rpat+''/common.c'')','errcatch')
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
          '#include <scicos_block4.h>';
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

endfunction

//==========================================================================
