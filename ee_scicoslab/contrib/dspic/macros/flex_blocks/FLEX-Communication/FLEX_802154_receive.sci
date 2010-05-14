// Expected integer params 
// ipar[0] := data_id 
// ipar[1] := src_addr 

function [x,y,typ] = FLEX_802154_receive(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    data_id=exprs(1);
    src_addr=exprs(2);
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
      [ok, data_id, src_addr, exprs]=..
      getvalue('802.15.4 Receive Params',..
      ['Data ID [1...255]:';..
       'Source Address [1...255]'],..
      list('vec',-1,'vec',-1),exprs)
      if ~ok then break,end
      if(data_id<1 | data_id>255) then
        warning('Invalid range for Data ID. Keeping previous values.');
        break;
      end
      if(src_addr<1 | src_addr>255) then
        warning('Invalid range for Source Address. Keeping previous values.');
        break;
      end

      in=[],
      if exists('outport') then out=ones(outport,1), else out=1, end
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[data_id; src_addr];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    data_id=1; 
    src_addr=1; 
    model=scicos_model()
    model.sim=list('flex_simple802154_receive',4)
    model.in=[],
    if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin=1
    model.rpar=[]
    model.ipar=[data_id; src_addr];
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(data_id);..
           sci2exp(src_addr)]
    gr_i=['xstringb(orig(1),orig(2),..
		[''FLEX 802.15.4'';..
		''RECEIVE'';..
		''ID:''+string(data_id)+'' SRC:''+string(src_addr)],..
		sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
