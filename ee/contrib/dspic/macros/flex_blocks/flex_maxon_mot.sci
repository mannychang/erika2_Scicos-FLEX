function [x,y,typ] = flex_maxon_mot(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    maxon_id=exprs(1)
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
      [ok,maxon_id,interf,kp,ki,exprs]=..
      getvalue('Set MAXON data block parameters',..
      ['CAN Id:';
       'Interface:';
       'Proportional Gain:';
       'Integral Gain:'],..
      list('str',1,'vec',-1,'vec',-1,'vec',-1),exprs)
      if ~ok then break,end
      if exists('inport') then in=ones(inport,1), else in=1, end
      out=[]
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[kp;ki];
        model.ipar=[hex2dec(strsubst(maxon_id,'0x',''));interf];
        model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    maxon_id='0x601'
    interf=1
    kp=8000
    ki=4000
    model=scicos_model()
    model.sim=list('rt_maxon_mot',4)
    if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.out=[]
    model.evtin=1
    model.rpar=[kp;ki]
    model.ipar=[hex2dec(strsubst(maxon_id,'0x',''));interf]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[maxon_id,sci2exp(interf),sci2exp(kp),sci2exp(ki)]
    gr_i=['xstringb(orig(1),orig(2),[''Maxon_mot'' ; maxon_id],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
