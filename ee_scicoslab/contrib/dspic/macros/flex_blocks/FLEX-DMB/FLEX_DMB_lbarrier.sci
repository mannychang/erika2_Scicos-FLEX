function [x,y,typ] = FLEX_DMB_lbarrier(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
		led_threshold=exprs(1)
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
      [ok,led_threshold,exprs]=..
      getvalue('Demo Board Led Barrier Parameters',..
      ['Threshold: [0.01..0.99]'],..
      list('vec',-1),exprs)
      if ~ok then break,end
			in=ones(8,1)
      out=[]
      [model,graphics,ok]=check_io(model,graphics,in,out,8,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[led_threshold];
        model.ipar=[];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    gpout_pin=1
    led_threshold=0.5
    model=scicos_model()
    model.sim=list('flex_daughter_leds_barrier',4)
		model.in=ones(8,1)
    out=[]
    model.out=[]
    model.evtin=1
    model.rpar=[led_threshold]
    model.ipar=[]
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(led_threshold)]
    gr_i=['xstringb(orig(1),orig(2),[''FLEX DMB LED'' ; ''BARRIER Th: ''+string(led_threshold)],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
