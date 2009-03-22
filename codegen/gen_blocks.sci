
//==========================================================================
//Generates Code for dynamically linked Fortran and C Blocks

// Original file from Project Metalau - INRIA
// Modified for RT purposes by Roberto Bucher - RTAI Team
// roberto.bucher@supsi.ch

function [CCode,FCode]=gen_blocks()

  CCode=[]
  FCode=[]

  kdyn=find(funtyp>1000) //dynamically linked blocs
                         //100X : Fortran blocks
                         //200X : C blocks

  if (size(kdyn,'*') >1)
    kfuns=[]; 
    //get the block data structure in the initial scs_m structure
    if size(corinv(kdyn(1)),'*')==1 then
      O=scs_m.objs(corinv(kdyn(1)));
    else
      path=list('objs');
      for l=corinv(kdyn(1))(1:$-1)
        path($+1)=l;
        path($+1)='model';
        path($+1)='rpar';
        path($+1)='objs';
      end
      path($+1)=corinv(kdyn(1))($);
      O=scs_m(path);
    end
    if funtyp(kdyn(1))>2000 then
      //C block
      CCode=[CCode;O.graphics.exprs(2)]
    else
      FCode=[FCode;O.graphics.exprs(2)]
    end
    kfuns=funs(kdyn(1));
    for i=2:size(kdyn,'*')
      //get the block data structure in the initial scs_m structure
      if size(corinv(kdyn(i)),'*')==1 then
        O=scs_m.objs(corinv(kdyn(i)));
      else
        path=list('objs');
         for l=corinv(kdyn(i))(1:$-1)
           path($+1)=l;
           path($+1)='model';
           path($+1)='rpar';
           path($+1)='objs';
        end
        path($+1)=corinv(kdyn(i))($);
        O=scs_m(path);
      end
      if (find(kfuns==funs(kdyn(i))) == [])
        kfuns=[kfuns;funs(kdyn(i))];
        if funtyp(kdyn(i))>2000  then
          //C block
          CCode=[CCode;O.graphics.exprs(2)]
        else
          FCode=[FCode;O.graphics.exprs(2)]
        end
      end
    end
  elseif (size(kdyn,'*')==1)
    //get the block data structure in the initial scs_m structure
    if size(corinv(kdyn),'*')==1 then
      O=scs_m.objs(corinv(kdyn));
    else
      path=list('objs');
      for l=corinv(kdyn)(1:$-1)
        path($+1)=l;
        path($+1)='model';
        path($+1)='rpar';
        path($+1)='objs';
      end
      path($+1)=corinv(kdyn)($);
      O=scs_m(path);
    end
    if funtyp(kdyn)>2000 then
      //C block
      CCode=[CCode;O.graphics.exprs(2)]
    else
      FCode=[FCode;O.graphics.exprs(2)]
    end
  end
  if CCode==[]
    CCode=['void no_ccode()'
           '{'
           '  return;'
           '}']
  end
endfunction
