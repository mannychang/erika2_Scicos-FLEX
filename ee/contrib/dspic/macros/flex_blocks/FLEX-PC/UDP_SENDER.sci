function [x,y,typ] = UDP_SENDER(job,arg1,arg2)

//** 29 June 2009
//** ------------------------------------ INPUT ---------------------------------


x=[];y=[];typ=[];

select job //** main state machine switch

case 'plot' then  //** plot the object
  graphics = arg1.graphics;
  exprs = graphics.exprs;
  port   = exprs(1)(1); 
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

  sadd0 = string(model.ipar(1));
  sadd1 = string(model.ipar(2));
  sadd2 = string(model.ipar(3));
  sadd3 = string(model.ipar(4));
  sport = string(model.ipar(5));

  while %t do
    
    dialog_box_banner = "FLEX USB Communication Parameters";
 
    [ok, add0, add1, add2, add3, port, name, lab] = getvalue(dialog_box_banner, ['Add.MSB';'Add.ISB';'Add.ISB';'Add.LSB';'Port';'Device'],...
                                  list('vec',1, 'vec',1,'vec',1,'vec',1,'vec',1,'str',1), [sadd0;sadd1;sadd2;sadd3;sport;"UDP_USB Sender"]);
                                                 
    if ~ok then break, end //** in case of error

    gr_i=['xstringb(orig(1),orig(2),[name; '' Port:''+string(model.ipar(5))],sz(1),sz(2),''fill'');']
    
    ng = [];
    z  = 0;
    nx = 0;
    nz = 0;
      o  = [];
      i   = [15];
      nin = 1;
      ci  = 1;
      nevin = 1;
      co = [];
    nevout = 0;
    
    depu = %t; dept = %f;
    dep_ut = [depu dept];

    funam = 'udp_sender';
    funtyp = 4 ;
    
    if ~ok then break,end
         [model,graphics,ok] = check_io(model,graphics,i,o,ci,co)
    
    if ok then
      model.sim = list(funam,funtyp) ; //** computation function
      model.in  = i ;
      model.out = [];
      model.evtin = ci;
      model.evtout = [];
      model.state = [];
      model.dstate = 0 ;
      model.rpar = [];
      model.ipar = [add0 add1 add2 add3 port];
      model.firing = [];
      model.dep_ut = dep_ut;
      model.nzcross = 0 ;
      x.model = model ;
      graphics.exprs = label ;
      graphics.gr_i = gr_i ;
      x.graphics = graphics ;
      break
    end
 
  end

case 'define' then      //** the standard define  
  add0 = 127;           //**
  add1 = 0   ;
  add2 = 0   ;
  add3 = 1   ;
  port  =  50001 ;
  name = 'UDP_USB Sender'; //** default name

  model = scicos_model();
  funam = 'udp_sender';
  funtyp = 4 ;
  model.sim=list(funam, funtyp) //** simulating function

  model.in = [15] ; 
  model.out = []; //** no output
  model.evtin = 1 ;
  model.evtout = [] ;
  model.state = [] ;
  model.dstate = [] ;
  model.rpar = [];
  model.ipar = [add0 add1 add2 add3 port];
  model.blocktype = 'd';  //'c';
  model.firing = [] ;
  model.dep_ut = [%t %f];
  model.nzcross = 0 ;

  label = list( [sci2exp(port), name], [] ) ;
 
  gr_i=['xstringb(orig(1),orig(2),[name; '' Port:''+string(port)],sz(1),sz(2),''fill'');']
  x = standard_define([4 2],model,label,gr_i)
end

endfunction

