function [x,y,typ] = FLEX_canin(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    can_msg_id = exprs(1);
    n_channels = exprs(2);
    standard_draw(arg1)
  case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1);
  case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1);
  case 'getorigin' then
    [x,y]=standard_origin(arg1);
  case 'set' then
    x=arg1;
    model=arg1.model;graphics=arg1.graphics;
    exprs=graphics.exprs;
    while %t do
      [ok,can_msg_id,n_channels,exprs]=..
      getvalue('Set parameters for block FLEX-CAN OUT',..
      ['CAN Message ID:';..
             'Number of elements to receive (1 or 2):'],..
      list('vec',-1,'vec',-1),exprs);
      if ~ok then break,end
      if(can_msg_id<0 | can_msg_id>4294967281) then
        warning('Accepted values for id are in [0,4294967281]. Keeping previous values.');
        break;
      end
      if(n_channels<1 | n_channels>2) then
        warning('Accepted values for channels are in [1,2]. Keeping previous values.');
        break;
      end
      in = [];
      // if exists('outport') then out=ones(outport,1), else out=1, end
      out = ones(n_channels,1);
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar = [];
        model.ipar = [can_msg_id, n_channels];
        model.dstate=[];
        x.graphics=graphics;
        x.model=model;
        break
      end
    end
  case 'define' then
    can_msg_id = 1;
    n_channels = 2;
    model=scicos_model();
    model.sim=list('flex_can_in',4);
    model.in=[];
    // if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.out = ones(n_channels,1);
    model.evtin=1
    model.rpar=[];
    model.ipar=[can_msg_id, n_channels];
    model.dstate=[];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(can_msg_id);..
                     sci2exp(n_channels)]
    gr_i=['xstringb(orig(1),orig(2),[''FLEX CAN IN '' ; ''MSG ID: ''+string(can_msg_id); ''MSG DIM: ''+string(n_channels)],sz(1),sz(2),''fill'');']
    x=standard_define([5 4],model,exprs,gr_i)
  end
endfunction
