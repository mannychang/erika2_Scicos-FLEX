// Expected integer params 
// ipar[0] := data_id 
// ipar[1] := dst_addr 
// ipar[2] := use_gts 

function [x,y,typ] = FLEX_802154d_send(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    data_id=exprs(1);
    dst_addr=exprs(2);
    use_gts=exprs(3);
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
      [ok, data_id, dst_addr, use_gts, exprs]=..
      getvalue('802.15.4 Device Send Params',..
      ['Data ID [1...255]:';..
       'Destination Address [1...255]';..
       'Use GTS [0,1]'],..
      list('vec',-1,'vec',-1,'vec',-1),exprs)
      if ~ok then break,end
      if(data_id<1 | data_id>255) then
        warning('Invalid range for Data ID. Keeping previous values.');
        break;
      end
      if(dst_addr<1 | dst_addr>255) then
        warning('Invalid range for Destination Address. Keeping previous values.');
        break;
      end
      if(use_gts<0 | use_gts>1) then
        warning('Invalid range for Use GTS. Keeping previous values.');
        break;
      end

      if exists('inport') then in=ones(inport,1), else in=1, end
      out=[],
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[data_id; dst_addr; use_gts];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    data_id=1; 
    dst_addr=1; 
    use_gts=0; 
    model=scicos_model()
    model.sim=list('flex_simple802154_device_send',4)
    if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.out=[],
    model.evtin=1
    model.rpar=[]
    model.ipar=[data_id; dst_addr; use_gts];
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(data_id);..
           sci2exp(dst_addr);..
           sci2exp(use_gts)]
    gr_i=['xstringb(orig(1),orig(2),..
		[''FLEX 802.15.4'';..
		''D-SEND'';..
		''ID:''+string(data_id)+'' DST:''+string(dst_addr)],..
		sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
