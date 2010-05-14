// Expected integer params 
// ipar[0] := local_address 
// ipar[1] := pan_id 
// ipar[2] := channel_id 
// ipar[3] := coord_addr 

function [x,y,typ] = FLEX_802154d_config(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    local_address=exprs(1);
    pan_id=exprs(2);
    channel_id=exprs(3);
    coord_addr=exprs(4);
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
      [ok, local_address, pan_id, channel_id, coord_addr, exprs]=..
      getvalue('802.15.4 Device Params',..
      ['Local Address [1...255]:';..
       'PanID [1...255]';..
       'Radio Channel [11...26]';..
       'Coordinator Address [1...255]'],..
      list('vec',-1,'vec',-1,'vec',-1,'vec',-1),exprs)
      if ~ok then break,end
      if(local_address<1 | local_address>255) then
        warning('Invalid range for Local Address. Keeping previous values.');
        break;
      end
      if(pan_id<1 | pan_id>255) then
        warning('Invalid range for PanID. Keeping previous values.');
        break;
      end
      if(channel_id<11 | channel_id>26) then
        warning('Invalid range for Radio Channel. Keeping previous values.');
        break;
      end
      if(coord_addr<1 | coord_addr>255) then
        warning('Invalid range for Coordinator Address. Keeping previous values.');
        break;
      end

      in=[],
      out=[],
//      if exists('outport') then out=ones(outport,1), else out=1, end
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[local_address; pan_id; channel_id; coord_addr];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    local_address=2; 
    pan_id=1; 
    channel_id=24; 
    coord_addr=1; 
    model=scicos_model()
    model.sim=list('flex_simple802154_device_config',4)
    model.in=[],
    model.out=[],
    //if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin=1
    model.rpar=[]
    model.ipar=[local_address; pan_id; channel_id; coord_addr];
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(local_address);..
           sci2exp(pan_id);..
           sci2exp(channel_id);..
           sci2exp(coord_addr)]
    gr_i=['xstringb(orig(1),orig(2),..
		[''FLEX 802.15.4'';..
		''DEVICE'';..
		''CONFIG''],..
		sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
