
//==========================================================================
//write_code_idoit : generate body of the code for
//                   ordering calls of initial
//                   called blocks
//
//input : nothing (blocks are called with flag 1)
//
//output : txt for iord
//
//15/07/07 Alan Layec
//Copyright INRIA

// Modified for RT purposes by Roberto Bucher - RTAI Team
// roberto.bucher@supsi.ch

function [txt]=write_code_idoit()

  txt=[];

  for j=1:niord
    bk=iord(j,1);
    pt=iord(j,2);
    //** blk
    if funtyp(bk)>-1 then
//      if or(bk==act) then
//        if stalone then
//          txt2=call_block42(bk,pt,1);
//          if txt2<>[] then
//            txt=[txt;
//                 '  '+txt2
//                 ''];
//          end
//        end
//      else
        txt2=call_block42(bk,pt,1);
        if txt2<>[] then
          txt=[txt;
               '  '+txt2
               ''];
        end
//      end
    //** ifthenelse blk
    elseif funtyp(bk)==-1 then
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      thentxt=write_code_doit(clkptr(bk),1);
      elsetxt=write_code_doit(clkptr(bk)+1,1);
      if thentxt<>[] | elsetxt<>[] then
        txt=[txt;
             '  '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_ = '*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '  if('+tmp_+'>0) {']
        //*******//
        txt=[txt;
             Indent+thentxt];
        if elsetxt<>[] then
           //** C **//
           txt=[txt;
                '  }';
                '  else {';]
           //*******//
           txt=[txt;
                Indent+elsetxt];
        end
        //** C **//
        txt=[txt;
             '  }']
        //*******//
      end
    //** eventselect blk
    elseif funtyp(bk)==-2 then
      Noutport=clkptr(bk+1)-clkptr(bk);
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      II=[];
      switchtxt=list()
      for i=1: Noutport
        switchtxt(i)=write_code_doit(clkptr(bk)+i-1,1);
        if switchtxt(i)<>[] then II=[II i];end
      end
      if II<>[] then
        txt=[txt;
             '  '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '  i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);']
        txt=[txt;
             '  switch(i)'
             '  {']
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '   case '+string(i)+' :';]
          //*******//
          txt=[txt;
               BigIndent+write_code_doit(clkptr(bk)+i-1,1);]
          //** C **//
          txt=[txt;
               BigIndent+'break;']
          //*******//
        end
        //** C **//
        txt=[txt;
             '  }'];
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
    end
  end

endfunction
