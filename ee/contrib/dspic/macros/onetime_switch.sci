function [x,y,typ] = onetime_switch(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    cmpr=exprs(1)
    delay=exprs(3)
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
      [ok,cmpr,lim,delay,exprs]=..
      getvalue('Edit block parameters',..
      ['0 (greater) or 1 (less)';
       'Switch value';
       'Delay time:'],..
      list('vec',-1,'vec',-1,'vec',-1),exprs)
      if ~ok then break,end
      in=[1;1;1]
      out=1
      flg=0;
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[lim;delay];
        model.ipar=[cmpr;flg];
        model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    cmpr=0
    lim=0.5
    delay=1
    flg=0;
    model=scicos_model()
    model.sim=list('rt_switch',4)
    model.in=[1;1;1]
    model.out=1
    model.evtin=1
    model.rpar=[lim;delay]
    model.ipar=[cmpr;flg]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(cmpr),sci2exp(lim),sci2exp(delay)]
    gr_i=['xstringb(orig(1),orig(2),[''Switch'' ; string(cmpr) ; string(delay)],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
