
//==========================================================================
//BlockProto : generate prototype
//            of a scicos block
//
//
//16/06/07 Author : A.Layec
//Copyright INRIA

// Modified for RT purposes by Roberto Bucher - RTAI Team
// roberto.bucher@supsi.ch

function [txt]=BlockProto(bk)

  nin=inpptr(bk+1)-inpptr(bk);  //* number of input ports */
  nout=outptr(bk+1)-outptr(bk); //* number of output ports */

  //**
  ftyp=funtyp(bk)
  if ftyp>2000 then ftyp=ftyp-2000,end
  if ftyp>1000 then ftyp=ftyp-1000,end

  //** check function type
  if ftyp < 0 then //** ifthenelse eselect blocks
      txt = [];
      return;
  else
    if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4) then
      disp("types other than 0,1,2,3 or 4 are not yet supported.")
      txt = [];
      return;
    end
  end

  //** add comment
  txt=[get_comment('proto_blk',list(funs(bk),funtyp(bk),bk));]

  select ftyp
    //** zero funtyp
    case 0 then

      //*********** prototype definition ***********//
      txtp=['(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *,int *, int *, \';
            ' double *, int *, double *, int *);'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank('void '+funs(bk)+'(');
        txtp(1) = 'void '+funs(bk)+txtp(1);
      elseif (funtyp(bk)<2000)
        txtp(1) = 'void C2F('+funs(bk)+')'+txtp(1);
        blank = get_blank('void C2F('+funs(bk)+')');
      end
      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //*******************************************//


    //**
    case 1 then

      //*********** prototype definition ***********//
      txtp=['(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *,int *, int *';]
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank('void '+funs(bk)+'(');
        txtp(1) = 'void '+funs(bk)+txtp(1);
      elseif (funtyp(bk)<2000)
        txtp(1) = 'void C2F('+funs(bk)+')'+txtp(1);
        blank = get_blank('void C2F('+funs(bk)+')');
      end
      if nin>=1 | nout>=1 then
        txtp($)=txtp($)+', \'
        txtp=[txtp;'']
        if nin>=1 then
          for k=1:nin
            txtp($)=txtp($)+' double *, int * ,'
          end
          txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
        end
        if nout>=1 then
          if nin>=1 then
            txtp($)=txtp($)+', \'
            txtp=[txtp;'']
          end
          for k=1:nout
            txtp($)=txtp($)+' double *, int * ,'
          end
          txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtp($)=txtp($)+', \'
        txtp=[txtp;' double *,int *);'];
      else
        txtp($)=txtp($)+');';
      end

      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //*******************************************//

    //**
    case 2 then

      //*********** prototype definition ***********//

      txtp=['void '+funs(bk)+...
            '(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *, int *, int *, \'
            ' double **, int *, int *, double **,int *, int *'];
      if ~ztyp(bk) then
        txtp($)=txtp($)+');';
      else
        txtp($)=txtp($)+', \';
        txtp=[txtp;
              ' double *,int *);']
      end
      blank = get_blank('void '+funs(bk));
      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //********************************************//

    //**
    case 4 then
      txt=[txt;
           'void '+funs(bk)+'(scicos_block *, int );'];

  end
endfunction
