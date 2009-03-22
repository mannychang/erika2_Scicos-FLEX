
//==========================================================================
//write_code_odoit : generate body of the code for
//                   ordering calls of blocks before
//                   continuous time integration
//
//input : flag : flag number for block's call
//
//output : txt for flag 0
//
//12/07/07 Alan Layec
//Copyright INRIA
function [txt]=write_code_odoit(flag)

//** Code execution tracing and profiling
global %tb ;
%tb = [%tb;"write_code_odoit"];
//**
 
  txt=[];

  for j=1:noord
    bk=oord(j,1);
    pt=oord(j,2);
    //** blk
    if funtyp(bk)>-1 then
      txt2=call_block42(bk,pt,flag);
      if txt2<>[] then
        txt=[txt;
             '    '+txt2
             ''];
      end
    //** ifthenelse blk
    elseif funtyp(bk)==-1 then
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      thentxt=write_code_ozdoit(clkptr(bk),flag);
      elsetxt=write_code_ozdoit(clkptr(bk)+1,flag);
      if thentxt<>[] | elsetxt<>[] then
        txt=[txt;
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if ((block_'+rdnom+'['+string(bk-1)+'].nmode<0'+...
              ' && '+tmp_+'>0)'+...
              ' || \'
             '        (block_'+rdnom+'['+string(bk-1)+'].nmode>0'+...
              ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==1)) {']
        //*******//
        txt=[txt;
             Indent+thentxt]
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
        if elsetxt<>[] then
          //** C **//
          txt=[txt;
               '    else if  ((block_'+rdnom+'['+string(bk-1)+'].nmode<0'+...
                ' && '+tmp_+'<=0)'+...
                ' || \'
               '              (block_'+rdnom+'['+string(bk-1)+'].nmode>0'+...
                ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==2)) {';]
          //*******//
          txt=[txt;
               Indent+elsetxt]
          //** C **//
          txt=[txt;
               '    }'];
          //*******//
        end
      end
    //** eventselect blk
    elseif funtyp(bk)==-2 then
      Noutport=clkptr(bk+1)-clkptr(bk);
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      II=[];
      switchtxt=list()
      for i=1: Noutport
        switchtxt(i)=write_code_ozdoit(clkptr(bk)+i-1,flag);
        if switchtxt(i)<>[] then II=[II i];end
      end
      if II<>[] then
        txt=[txt;
             '    '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if (block_'+rdnom+'['+string(bk-1)+'].nmode<0) {';
             '      i=max(min((integer) '+...
                tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);'
             '    }'
             '    else {'
             '      i=block_'+rdnom+'['+string(bk-1)+'].mode[0];'
             '    }']
        txt=[txt;
             '    switch(i)'
             '    {'];
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '     case '+string(i)+' :';]
          //*******//
          txt=[txt;
               BigIndent+write_code_ozdoit(clkptr(bk)+i-1,flag);]
          //** C **//
          txt=[txt;
               BigIndent+'break;']
          //*******//
        end
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
    end
  end

endfunction
