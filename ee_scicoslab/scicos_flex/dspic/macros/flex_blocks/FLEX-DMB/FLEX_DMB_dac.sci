function [x,y,typ] = FLEX_DMB_dac(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    pwm_pin=exprs(1)
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
    while %t do
      [ok,pwm_pin,exprs]=..
      getvalue('Select DAC Output',..
      ['DAC Anolog Out [1..2] :'],..
      list('vec',-1),exprs)
      if ~ok then break,end
	  if((pwm_pin<1) | (pwm_pin>2)) then
		warning('Accepted values for out are in [1,2]. Keeping previous values.');
		break;
	  end
      if exists('inport') then in=ones(inport,1), else in=1, end
      out=[]
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[pwm_pin];
        model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    pwm_pin=1
    model=scicos_model()
    model.sim=list('flex_daughter_dac',4)
    if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.out=[]
    model.evtin=1
    model.rpar=[]
    model.ipar=[pwm_pin]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(pwm_pin)]
    gr_i=['xstringb(orig(1),orig(2),[''FLEX-DMB'' ; ''DAC.Out.''+string(pwm_pin)],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
