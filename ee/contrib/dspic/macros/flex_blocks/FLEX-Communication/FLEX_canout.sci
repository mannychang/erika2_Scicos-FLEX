function [x,y,typ] = FLEX_canout(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    can_msg_id=exprs(1)
    n_bytes=exprs(2)
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
      [ok,can_msg_id,n_bytes,exprs]=..
      getvalue('Set parameters for block FLEX-CAN OUT',..
      ['CAN Messages ID:';..
			 'Number of bytes to send:'],..
      list('vec',-1,'vec',-1),exprs)
      if ~ok then break,end
      if exists('inport') then in=ones(inport,1), else in=1, end
      out=[]
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[can_msg_id];
        model.ipar=[n_bytes];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    can_msg_id=1
    n_bytes=10
    model=scicos_model()
    model.sim=list('flex_canout',4)
    if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.out=[]
    model.evtin=1
    model.rpar=[can_msg_id];
    model.ipar=[n_bytes];
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(can_msg_id);..
					 sci2exp(n_bytes)]
    gr_i=['xstringb(orig(1),orig(2),[''FLEX CAN OUT'' ; ''MSG ID: ''+string(can_msg_id)],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
