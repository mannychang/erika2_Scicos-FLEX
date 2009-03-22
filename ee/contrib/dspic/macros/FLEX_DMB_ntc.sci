function [x,y,typ] = FLEX_DMB_ntc(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    adc_pin=exprs(1)
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
      adc_pin = 7 ; //** put here the adc input used by the NTC sensor
      ok = %t     ; 
      in=[],
      if exists('outport') then
        out = ones(outport,1)
      else
        out=1
      end 
      [model,graphics,ok] = check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[adc_pin];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    adc_pin = 7 ; //** put here the adc input used by the NTC sensor
    model=scicos_model()
    model.sim=list('flex_dmb_ntc',4)
    model.in=[],
    if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin=1
    model.rpar=[]
    model.ipar=[adc_pin]
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(adc_pin)]
    gr_i=['xstringb(orig(1),orig(2),[''FLEX-DMB'' ; ''Temp.(�C)''],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
