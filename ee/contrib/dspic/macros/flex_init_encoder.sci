function [x,y,typ] = flex_init_encoder(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    trigtime=exprs(1)
    offs=exprs(3)
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
      [ok,trigtime,defval,offs,exprs]=..
      getvalue('Init encoder parameters',..
      ['Trigger Time:';
       'Default output:';
       'Offset:'],..
      list('vec',-1,'vec',-1,'vec',-1),exprs)
      if ~ok then break,end
      if exists('inport') then in=ones(inport,1), else in=1, end
      if exists('outport') then out=ones(outport,1), else out=1, end
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[trigtime;defval;offs;0.0];
        model.ipar=[];
        model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    trigtime=1
    defval=0
    offs=0
    model=scicos_model()
    model.sim=list('rt_init_encoder',4)
    if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin=1
    model.rpar=[trigtime;defval;offs]
    model.ipar=[]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(trigtime),sci2exp(defval),sci2exp(offs)]
    gr_i=['xstringb(orig(1),orig(2),[''Init_encoder'' ; string(trigtime) ; string(offs)],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
