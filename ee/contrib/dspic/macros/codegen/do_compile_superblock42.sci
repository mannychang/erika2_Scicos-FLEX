// Transforms a given Scicos discrete and continuous SuperBlock into a C defined Block
// Copyright INRIA
//

// Original file from Project Metalau - INRIA
// Modified for RT purposes by Roberto Bucher - RTAI Team
// roberto.bucher@supsi.ch

function  [ok,XX,alreadyran,flgcdgen,szclkINTemp,freof] = do_compile_superblock42(XX,all_scs_m,numk,alreadyran)

  scs_m = XX.model.rpar ; //** isolate the super block scs_m data structure 
  par = scs_m.props;
  
  //** SAFETY : terminate current simulation 
  if alreadyran then
    //** terminate current simulation
    do_terminate() ; 
    alreadyran  = %f ;
  end

  hname = scs_m.props.title(1);  //** super block name

  //***********************************************************
  //Check blocks properties and adapt them if necessary
  //***********************************************************


  //**  These blocks are not ALLOWED for Emb code generation 
  vorbidden_items=["CLKOUT_f","activation (events) output ports";
                   "IN_f","input ports";
                   "OUT_f","output ports";
                   "CLKOUTV_f","activation outputs";
                   "CLOCK_c","clocks";
                   "CLOCK_f","clocks";
                   "SampleCLK","clocks";
                   "RFILE_f","Read block";
                   "READC_f","Read_block";
                   "WFILE_f","Write block";
                   "WRITEC_f","Write block"]

  clkIN = [];
  
  //** scan 
  for i=1:size(scs_m.objs)

    //** BLOCKS 
    if typeof(scs_m.objs(i))=="Block" then
      ind=find(vorbidden_items==scs_m.objs(i).gui);
      if(ind~=[]) then
        ok = %f ;
        %cpr = list();
        message(vorbidden_items(ind(1),2)+" not allowed in Superblock");
        return; // EXIT point
      
      elseif scs_m.objs(i).gui=="CLKINV_f" then //** input clock from external diagram 
        //** replace event input ports by  fictious block
        scs_m.objs(i).gui="EVTGEN_f";
        scs_m.objs(i).model.sim(1)="bidon"
        if clkIN==[] then
          clkIN = 1;
        else
          ok = %f;
          %cpr = list();
          message("Only one activation block allowed!");
          return; // EXIT point
        end
      end
    end
  end

  
  szclkIN = size(clkIN,2);
  

  flgcdgen = szclkIN; //** pass the clock 
  
  //** 
  //** BEWARE : the Scicos compiler is called here ! 
  //**
  [bllst,connectmat,clkconnect,cor,corinv,ok,scs_m,flgcdgen,freof] = c_pass1(scs_m,flgcdgen);

  if ~ok then
    message("Sorry: problem in the pre-compilation step.")
    return ; //** EXIT point 
  end

  szclkINTemp = szclkIN;
  szclkIN = flgcdgen;


  for i=1:size(bllst)
    if (bllst(i).sim(1)=="bidon") then //** clock input 
      howclk = i;
    end
  end

  
  //** BEWARE : update to new graphics instructions ! 
  %windo = xget('window') ; 
  
  cpr = c_pass2(bllst,connectmat,clkconnect,cor,corinv)

  if cpr==list() then
      ok = %f
      return ; //** exit point 
  end

  //** Alan's patch 5/07/07: try to solve
  //   which blocks use work
  //** 
  funs_save = cpr.sim.funs           ;
  funtyp_save = cpr.sim.funtyp       ;
  with_work = zeros(cpr.sim.nblk,1)  ;


  //** ------ Windows cleaning for internal Scopes -------------------
  //**
  //**
  //** retrieve all open Scilab windows with winsid()
  //**

  BeforeCG_WinList = winsid(); 

  ierr=execstr('[state,t]=scicosim(cpr.state,0,0,cpr.sim,'+..
               '''start'',scs_m.props.tol)','errcatch')
  if ierr==0 then
    for i=1:cpr.sim.nblk
       if state.iz(i)<>0 then
          with_work(i)=%t
       end
    end
    ierr=execstr('[state,t]=scicosim(state,0,0,cpr.sim,'+..
                 '''finish'',scs_m.props.tol)','errcatch')
  end

  //**
  //** retrieve all open Scilab windows with winsid
  //** and close the additional windows open since first
  //** 
 
  //** This code does not cover 100% of the possible situations because the user can 
  //** destroy one or more Scicos wins manually during this intermediate phase
  //** This code is 100% functional if the the user does not close manually any win.
  //** TO BE updated in Scilab 5.0

  AfterCG_WinList = winsid();
  
  AfterCG_size = size(AfterCG_WinList); //** matrix
  AfterCG_size = AfterCG_size(2) ; //** vector lenght 

  BeforeCG_size = size(BeforeCG_WinList); //** matrix
  BeforeCG_size = BeforeCG_size(2) ; //** vector lenght

  if (or(AfterCG_WinList<>BeforeCG_WinList)) & (AfterCG_size>BeforeCG_size) then
     //** means that a scope or other scicos object has created some
     //** output window

     DiffCG_Winlist = AfterCG_WinList<>BeforeCG_WinList ; //** T/F mismatch 
     DiffCG_Index = find(DiffCG_Winlist); //** recover the mismatched indexes only 

     for win_idx = DiffCG_Index
         delete( scf( AfterCG_WinList(win_idx) ) ) ; //** clear the spurious windows   
     end 
     
  end  
  //**------------- end of windows cleaning for internal scopes -------------------------------

  cpr.sim.funs=funs_save;
  cpr.sim.funtyp=funtyp_save;

  //** BEWARE: replace this OLD graphics instruction !
  xset('window',%windo) ; 

  ///////////////////
  //** %cpr pointers 
  x = cpr.state.x;
  z = cpr.state.z;
  outtb = cpr.state.outtb;

  // RN
  zcptr = cpr.sim.zcptr;
  ozptr = cpr.sim.ozptr;
  rpptr = cpr.sim.rpptr;
  ipptr = cpr.sim.ipptr;
  opptr = cpr.sim.opptr;
  funs = cpr.sim.funs;
  xptr = cpr.sim.xptr;
  zptr = cpr.sim.zptr;
  inpptr = cpr.sim.inpptr;
  inplnk = cpr.sim.inplnk;
  outptr = cpr.sim.outptr;
  outlnk = cpr.sim.outlnk;
  
  // @l@n lnkptr = cpr.sim.lnkptr; ???
  
  ordclk = cpr.sim.ordclk;
  funtyp = cpr.sim.funtyp;
  cord   = cpr.sim.cord;
  ncord  = size(cord,1);
  nblk   = cpr.sim.nb;
  ztyp   = cpr.sim.ztyp;
  clkptr = cpr.sim.clkptr
  
  // taille totale de z : nztotal
  nztotal = size(z,1);

  //*******************************
  //Checking if superblock is valid
  //*******************************
  msg = []

  for i=1:length(funs)-1

    if funtyp(i)==3 then
      msg = [msg;"Type 3 block''s not allowed"] ;
   
// Bubu 18.03.2008
    elseif funtyp(i)==0 & funs(i)~="bidon"  then
      msg = [msg;"Type 0 block''s"+" ''"+funs(i)+"''"+" not allowed"] ;
//end
   
    elseif (clkptr(i+1)-clkptr(i))<>0 &funtyp(i)>-1 &funs(i)~="bidon" then

      //Alan // msg=[msg;'Regular block generating activation not allowed yet']

    end

    if msg<>[] then 
         message(msg) ;
         ok = %f ;
         return ; //** 
    end

  end


  //** Find the clock connected to the SuperBlock and retreive
  //** the sampling time
  
  if XX.graphics.pein==[] | XX.graphics.pein(1)==0 then
    sTsamp="0.001"; //** default value is ONE millisecond 
  else
    o_ev = XX.graphics.pein(1);
    o_ev=all_scs_m.objs(o_ev).from(1);

    while (all_scs_m.objs(o_ev).gui~='CLOCK_c' & ...
           all_scs_m.objs(o_ev).gui~='CLOCK_f' & ...
           all_scs_m.objs(o_ev).gui~='SampleCLK')

               o_ev = all_scs_m.objs(o_ev).graphics.pein(1);
               o_ev = all_scs_m.objs(o_ev).from(1);

    end

    if all_scs_m.objs(o_ev).gui=='SampleCLK' then
      sTsamp=all_scs_m.objs(o_ev).model.rpar(1);
      sTsamp=sci2exp(sTsamp);
      Tsamp_delay=all_scs_m.objs(o_ev).model.rpar(2);
      Tsamp_delay=sci2exp(Tsamp_delay);
    else
      sTsamp=all_scs_m.objs(o_ev).model.rpar.objs(2).graphics.exprs(1);
      sTsamp=sci2exp(eval(sTsamp));
      Tsamp_delay=all_scs_m.objs(o_ev).model.rpar.objs(2).graphics.exprs(2);
      Tsamp_delay=sci2exp(eval(Tsamp_delay));
    end

  end

  //***********************************
  // Get the name of the file
  //***********************************
  foo = 3; //** probably this variable is never used ? 
  okk = %f; 
  rdnom='foo'; 
  rpat = getcwd(); 
  archname=''; 
  Tsamp = sci2exp(eval(sTsamp));
  
  template = 'board_flex'; //** default values for this version 
  
  if XX.model.rpar.props.void3 == [] then
	target = 'dspic'; //** default compilation chain 
	odefun = 'ode4';  //** default solver 
	odestep = '10';   //** default continous step size 
  else
	target  = XX.model.rpar.props.void3(1); //** user defined parameters 
	odefun  = XX.model.rpar.props.void3(2);
	odestep = XX.model.rpar.props.void3(3);
  end

  libs='';

  //** dialog box default variables 
  label1=[hname;getcwd()+'/'+hname+"_scig";target;template];
  label2=[hname;getcwd()+'/'+hname+"_scig";target;template;odefun;odestep];
  
  ode_x=['ode1';'ode2';'ode4']; //** available continous solver 
  
  //** Open a dialog box 
  while %t do
    ok = %t ;
    if x==[] then
      //** Pure discrete system NO CONTINOUS blocks 
     
      [okk, rdnom, rpat,target,template,label1] = getvalue(..
	            'Embedded Code Generation',..
		        ['New block''s name :';
		         'Created files Path:';
			     'Toolchain: ';
			     'Target Board: '],..
		         list('str',1,'str',1,'str',1,'str',1),label1);
    else
      //** continous blocks are presents
      [okk,rdnom,rpat,target,template,odefun,odestep,label2] = getvalue(..
	            "Embedded Code Generation",..
		        ["New block''s name: "  ;
		         "Created files Path: " ;
			     "Toolchain: "          ;
			     "Target Board: "       ;
		         "ODE solver type: "       ;
		         "ODE solver steps betw. samples: "],..
		         list('str',1,'str',1,'str',1,'str',1,'str',1,'str',1),label2);
    end
  
    if okk==%f then
      ok = %f
      return ; //** EXIT point 
    end
    rpat = stripblanks(rpat);


    //** I put a warning here in order to inform the user
    //** that the name of the superblock will change
    //** because the space char in name isn't allowed.
    if grep(rdnom," ")<>[] then
      message(['Superblock name cannot contains space characters.';
               'space chars will be automatically substituted by ""_"" '])
    end
    rdnom = strsubst(rdnom,' ','_');

    //** Put a warning here in order to inform the user
    //** that the name of the superblock will change
    //** because the "-" char could generate GCC problems
    //** (the C functions contains the name of the superblock).
    if grep(rdnom,"-")<>[] then
      message(['For full C compiler compatibility ';
               'Superblock name cannot contains ""-"" characters';
               '""-"" chars will be automatically substituted by ""_"" '])
    end

    rdnom = strsubst(rdnom,'-','_'); 

    dirinfo = fileinfo(rpat)
    
    if dirinfo==[] then
      [pathrp, fnamerp, extensionrp] = fileparts(rpat); 
      ok = mkdir(pathrp, fnamerp+extensionrp) ; 
      if ~ok then 
        message("Directory '+rpat+' cannot be created");
      end
    elseif filetype(dirinfo(2))<>'Directory' then
      ok = %f;
      message(rpat+" is not a directory");
    end

    if stripblanks(rdnom)==emptystr() then 
      ok = %f;
      message("Sorry: C file name not defined");
    end


    //** This comments will be moved in the documentation 

    //** /contrib/RT_templates/pippo.gen

    //** 1: pippo.mak 
    //** 2: pippo.cmd

    //** pippo.mak : scheletro del Makefile 
    //**             - GNU/Linux : Makefile template
    //**             - Windows/Erika : conf.oil
    //**                               erika.cmd
 
    //** pippo.cmd : sequenza di comandi Scilab 


    TARGETDIR = SCI+"/contrib/RT_templates";


    [fd,ierr] = mopen(TARGETDIR+'/'+target+'.gen','r');

    if ierr==0 then
      mclose(fd);
    else
      ok = %f;
      message("Target not valid " + target + ".gen");
    end
    
    if ok then
      target_t = mgetl(TARGETDIR+'/'+target+'.gen');
      makfil = target_t(1);
      cmdfil = target_t(2);

      [fd,ierr]=mopen(TARGETDIR+'/'+makfil,'r');
      if ierr==0 then
        mclose(fd);
      else
        ok = %f ;
        message("Makefile not valid " + makfil);
      end
    end

    if x ~= [] then
      if grep(odefun,ode_x) == [] then
         message("Ode function not valid");
         ok = %f;
      end
    end

    if ok then break,end
  end

  //////////////////////////////////////////////////
  maxnrpar=max(rpptr(2:$)-rpptr(1:$-1))
  maxnipar=max(ipptr(2:$)-ipptr(1:$-1))
  maxnx=max(xptr(2:$)-xptr(1:$-1))
  maxnz=max(zptr(2:$)-zptr(1:$-1))
  maxnin=max(inpptr(2:$)-inpptr(1:$-1))
  maxnout=max(outptr(2:$)-outptr(1:$-1))
  maxdim=[];
  for i=1:lstsize(cpr.state.outtb)
    maxdim=max(size(cpr.state.outtb(i)))
  end
  maxtotal=max([maxnrpar;maxnipar;maxnx;maxnz;maxnin;maxnout;maxdim]);

//------------------ The real code generation is here ------------------------------------

  //************************************************************************
  //generate the call to the blocks and blocs simulation function prototypes
  //************************************************************************
  wfunclist = list();
  nbcap = 0;
  nbact = 0;
  capt  = [];
  actt  = [];
  Protostalone = [];
  Protos       = [];
  dfuns        = [] ;
  


  //** scan the data structure and call the generating functions 
  //** Substitute previous code!!!!
  
  for i=1:length(funs)
    ki= find(funs(i) == dfuns) ; //** 
    dfuns = [dfuns; funs(i)] ; 
      
    if ki==[] then
      Protostalone=[Protostalone;'';BlockProto(i)];
    end
  end


  //***********************************
  // Scilab and C files generation
  //***********************************

  cmdseq = mgetl(TARGETDIR+'/' + cmdfil);
  n_cmd = size(cmdseq,1);

  for i=1:n_cmd
  
    if (cmdseq(i)~="") then
         disp("Executing " + """" +cmdseq(i)+ """" + '...'); 
    end;
    
    execstr(cmdseq(i));
  
  end

  disp("----> Target generation terminated!");

endfunction
