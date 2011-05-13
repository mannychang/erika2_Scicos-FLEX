
//==========================================================================
//CallBlock : generate C calling sequence
//            of a scicos block
//
//inputs : bk   : bloc index
//         pt   :
//         flag :block_'+rdnom+'[
//
//output : txt  :
//
//16/06/07 Authors : R.Nikoukhah, A.Layec
//Copyright INRIA

function txt = call_block4(bk)

//** Code execution tracing and profiling 
global %tb ;
%tb = [%tb;"call_block4"];
//%tb = tb+"2s"; 
//** 

  //**
  nx = xptr(bk+1)-xptr(bk);
  nz = zptr(bk+1)-zptr(bk);
  nrpar = rpptr(bk+1)-rpptr(bk);
  nipar = ipptr(bk+1)-ipptr(bk);
  nin  = inpptr(bk+1)-inpptr(bk);  //* number of input ports */
  nout = outptr(bk+1)-outptr(bk); //* number of output ports */

  //**
  //l'adresse du pointeur de ipar
  if nipar<>0 then ipar=ipptr(bk), else ipar=1;end
  
  //l'adresse du pointeur de rpar
  if nrpar<>0 then rpar=rpptr(bk), else rpar=1; end
  
  //l'adresse du pointeur de z attention -1 pas sur
  if nz<>0 then z=zptr(bk)-1, else z=0;end
  
  //l'adresse du pointeur de x
  if nx<>0 then x=xptr(bk)-1, else x=0;end


  //** BEWARE :
  //** 1) obsolete and recent fortran computational functions can be compiled using "f2c" (fortran to c) translator
  //**    but we strongly discourage this procedure 
  //** 2) Scilab script language computational functions are not supported in this version of CG; Support for them
  //**    will follow with "S2C" ("Scilab to C" translator, hArtes project);  

  //** Scicos block computational function type 
  //** 0 : "obsolete fortran" ?
  //** 1 : "obsolete Scilab"  ? 
  //** 2 : "FORTRAN"
  //** 3 : "Scilab" script language ; slow but easy to debug 
  //** 4 : "C" language code; the most used computational function   

  ftyp = funtyp(bk) ; //** classify block type : | 0 | 1 | 2 |3 | 4 |
    


  if ftyp>2000 then ftyp=ftyp-2000,end // ?
  if ftyp>1000 then ftyp=ftyp-1000,end

  //** check function type
  if ftyp < 0 then //** if then else eselect blocks
      txt = [];
      return;
  else
  
    if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4) then
      message ("types other than 0,1,2,3 or 4 are not supported.")
      txt = [];
      return;
    end
  
  end

  //** add comment
  txt = [get_comment("call_blk",list(funs(bk),funtyp(bk),bk));]

  //** write nevprt activation
  //   nclock=abs(pt);
  txt = [txt;
         'block_'+rdnom+'['+string(bk-1)+'].nevprt=nevprt;']

  select ftyp
    
    //** ----------------------------- zero funtyp
    case 0 then
      //**** input/output addresses definition ****//
      if nin>1 then
        for k=1:nin
          uk = inplnk(inpptr(bk)-1+k);
          nuk = size(outtb(uk),'*');
          TYPE = mat2scs_c_ptr(outtb(uk)); // Scilab index start from 1
          txt  = [txt;
                  'rdouttb['+string(k-1)+']=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(uk-1)+'];']
        end
        txt = [txt;
               'args[0]=&(rdouttb[0]);']
      
      elseif nin==0
        uk=0;
        nuk=0;
        txt=[txt;
             'args[0]=(SCSREAL_COP *)'+rdnom+'_block_outtbptr[0];']
      else
        uk=inplnk(inpptr(bk));
        nuk=size(outtb(uk),'*');
        TYPE=mat2scs_c_ptr(outtb(uk)); // Scilab index start from 1
        txt=[txt;
             'args[0]=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(uk-1)+'];']
      end

      if nout>1 then
        for k=1:nout
          yk=outlnk(outptr(bk)-1+k);
          nyk=size(outtb(yk),'*');
          TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
          txt=[txt;
               'rdouttb['+string(k+nin-1)+']=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(yk-1)+'];'];
        end
        txt=[txt;
             'args[1]=&(rdouttb['+string(nin)+']);'];
      elseif nout==0
        yk=0;
        nyk=0;
        txt=[txt;
             'args[1]=(SCSREAL_COP *)'+rdnom+'_block_outtbptr[0];'];
      else
        yk=outlnk(outptr(bk));
        nyk=size(outtb(yk),'*'),;
        TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
        txt=[txt;
             'args[1]=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(yk-1)+'];'];
      end
      //*******************************************//

      //*********** call seq definition ***********//
      txtc=['(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double *)args[0],&nrd_'+string(nuk)+',(double *)args[1],&nrd_'+string(nyk)+');'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//


    //** --------------------------- UNO function type 
    case 1 then

      //*********** call seq definition ***********//
      txtc=['(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      if nin>=1 | nout>=1 then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'']
        if nin>=1 then
          for k=1:nin
            uk=inplnk(inpptr(bk)-1+k);
            nuk=size(outtb(uk),'*');
            txtc($)=txtc($)+'(SCSREAL_COP *)'+rdnom+'_block_outtbptr['+string(uk-1)+'],&nrd_'+string(nuk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
        if nout>=1 then
          if nin>=1 then
            txtc($)=txtc($)+', \'
            txtc=[txtc;'']
          end
          for k=1:nout
            yk=outlnk(outptr(bk)-1+k);
            nyk=size(outtb(yk),'*');
            txtc($)=txtc($)+'(SCSREAL_COP *)'+rdnom+'_block_outtbptr['+string(yk-1)+'],&nrd_'+string(nyk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'w,&nrd_0);'];
      else
        txtc($)=txtc($)+');';
      end

      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//


    //** DUE function type 
    case 2 then

      //*********** call seq definition ***********//
      txtc=[funs(bk)+'(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].inptr,block_'+rdnom+'['+string(bk-1)+'].insz,&block_'+rdnom+'['+string(bk-1)+'].nin, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].outptr,block_'+rdnom+'['+string(bk-1)+'].outsz, &block_'+rdnom+'['+string(bk-1)+'].nout'];
      if ~ztyp(bk) then
        txtc($)=txtc($)+');';
      else
        txtc($)=txtc($)+', \';
        txtc=[txtc;
              'block_'+rdnom+'['+string(bk-1)+'].g,&block_'+rdnom+'['+string(bk-1)+'].ng);']
      end
      blank = get_blank(funs(bk)+'( ');
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    case 3 then
    message("Case 3 is not allowed in call_block4.sci :( sorry )"); 

    //**
    case 4 then
      txt = [txt;
             funs(bk)+'(&block_'+rdnom+'['+string(bk-1)+'],flag);'];

  end

endfunction
