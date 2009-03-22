//** 07 June 2007
//** ------------------------------------ INPUT ---------------------------------
//** SCICOS communications using USB 
//** 
//** This code is a macro "interfacing function" for a dummy WRITE

function [x,y,typ] = flex_usb_dataout(job,arg1,arg2)

x=[];y=[];typ=[];

select job //** main state machine switch

case 'plot' then  //** plot the object 
  graphics = arg1.graphics; 
  exprs = graphics.exprs;
  ch    = exprs(1)(1);
  name  = exprs(1)(2);
  standard_draw(arg1)

case 'getinputs' then //** inputs 
  [x,y,typ] = standard_inputs(arg1) //** 

case 'getoutputs' then //** 
  [x,y,typ] = standard_outputs(arg1)

case 'getorigin' then  //** 
  [x,y]=standard_origin(arg1)

case 'set' then //** set parameters 
  x = arg1 ; //** 
  model    = arg1.model;
  graphics = arg1.graphics;
  label    = graphics.exprs;
  while %t do
    
    dialog_box_banner = "FLEX USB Communication Parameters"; 
    [ok, ch, name, lab]=getvalue(dialog_box_banner, ['Channel';'Device'], list('vec',1,'str',1), label(1));     
    //** Channel->ch  Device->name   

    if ~ok then break,end
    
    label(1)=lab
    
    funam  = 'flex_usb_dummy_write';
    funtyp = 4 ; //** valid numbers are 4->(C) 5->(Scilab)  
    
    ng = [];
    z  = 0;
    nx = 0;
    nz = 0;
    o  = [];
    i  = 1;
    nin = 1;
    ci  = 1;
    nevin = 1;
    co = [];
    nevout = 0;
    
    depu = %t; dept = %f;
    dep_ut = [depu dept];
    
    if ~ok then break,end
         [model,graphics,ok] = check_io(model,graphics,i,o,ci,co)
    
    if ok then
      model.sim = list(funam,funtyp) ; //** compuatation function 
      model.in  = i ;
      model.out = [];
      model.evtin = ci; 
      model.evtout = [];
      model.state = [];
      model.dstate = 0 ;
      model.rpar = []; 
      model.ipar = [ch]; 
      model.firing = [];
      model.dep_ut = dep_ut; 
      model.nzcross = 0 ;
      x.model = model ;
      graphics.exprs = label ;
      x.graphics = graphics ;
      break
    end
  
  end

case 'define' then      //** the standard define  
  ch   = 0;           //** default channel  
  name = 'USB-Dummy'; //** default name 

  model = scicos_model(); 
  funam = 'flex_usb_dummy_write';
  funtyp = 4 ;
  model.sim=list(funam, funtyp) //** simulating function

  model.in = 1
  model.out = []
  model.evtin = 1
  model.evtout = []
  model.state = []
  model.dstate = []
  model.rpar = []
  model.ipar = [ch]; 
  model.blocktype = 'c'; 
  model.firing = [] ; 
  model.dep_ut = [%t %f]; 
  model.nzcross = 0 ; 

  label = list( [sci2exp(ch), name], [] ) ;
  
  gr_i=['xstringb(orig(1),orig(2),[''USB Send Data'';name+'' CH-''+string(ch)],sz(1),sz(2),''fill'');']
  x = standard_define([4 2],model,label,gr_i)
end

endfunction


