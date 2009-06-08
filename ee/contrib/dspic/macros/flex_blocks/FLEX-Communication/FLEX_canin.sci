function [x,y,typ] = FLEX_canin(job,arg1,arg2)
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
      in=[],
      if exists('outport') then out=ones(outport,1), else out=1, end
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[can_msg_id;..
										n_bytes];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    can_msg_id=1
    n_bytes=10
    model=scicos_model()
    model.sim=list('flex_canin',4)
    model.in=[],
    if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin=1
    model.rpar=[]
    model.ipar=[can_msg_id;..
								n_bytes];
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(can_msg_id);..
					 sci2exp(n_bytes)]
    gr_i=['xstringb(orig(1),orig(2),[''FLEX CAN IN'' ; ''PIN: ''+string(can_msg_id)],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
