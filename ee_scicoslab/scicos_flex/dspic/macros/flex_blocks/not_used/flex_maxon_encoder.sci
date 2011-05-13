function [x,y,typ] = flex_maxon_encoder(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    maxon_id=exprs(1)
    maxon_res=exprs(3)
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
      [ok,maxon_id,interf,maxon_res,exprs]=..
      getvalue('Set MAXON driver data block parameters',..
      ['CAN Id:';
       'Interface:';
       'Resolution:'],..
      list('str',1,'vec',-1,'vec',-1),exprs)
      if ~ok then break,end
      in=[],
      if exists('outport') then out=ones(outport,1), else out=1, end
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[4*maxon_res];
        model.ipar=[hex2dec(strsubst(maxon_id,'0x',''));interf];
        model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    maxon_id='0x601'
    interf=1
    maxon_res=500
    model=scicos_model()
    model.sim=list('rt_maxon_encoder',4)
    model.in=[],
    if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin=1
    model.rpar=[4*maxon_res]
    model.ipar=[hex2dec(strsubst(maxon_id,'0x',''));interf]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[maxon_id,sci2exp(interf),sci2exp(maxon_res)]
    gr_i=['xstringb(orig(1),orig(2),[''Maxon_encoder'' ; maxon_id ; string(maxon_res)],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
