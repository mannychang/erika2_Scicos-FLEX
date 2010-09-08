function [x,y,typ] = AMAZING_pwm(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  
  case 'plot' then
    exprs=arg1.graphics.exprs;
    standard_draw(arg1)
	
  case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
	
  case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
	
  case 'getorigin' then
    [x,y]=standard_origin(arg1)
	
  case 'set' then
    x=arg1
    model=arg1.model;graphics=arg1.graphics;
    exprs=graphics.exprs;
    exprs=[];
    in=ones(2,1)
    out=[]
    [model,graphics,ok]=check_io(model,graphics,in,out,2,[])
    graphics.exprs=exprs;
    model.evtin=1;
    model.rpar=[];
    model.ipar=[];
    model.dstate=[1];
    x.graphics=graphics;x.model=model
	
  case 'define' then
    model=scicos_model()
    model.sim=list('amazing_pwm',4)
    model.in=ones(2,1)
    model.out=[]
    model.evtin=1
    model.rpar=[]
    model.ipar=[]
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[]
    gr_i=['xstringb(orig(1),orig(2),[''AMAZING'' ; ''PWM outputs''],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
