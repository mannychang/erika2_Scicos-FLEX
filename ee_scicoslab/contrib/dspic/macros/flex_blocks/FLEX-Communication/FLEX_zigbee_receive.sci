function [x,y,typ] = FLEX_zigbee_receive(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  
  case 'plot' then
    exprs=arg1.graphics.exprs;
    zigbee_ch=exprs(1)
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
      [ok,zigbee_ch,exprs]=..
      getvalue('Set Channel number',..
      ['Channel [0..255] :'],..
      list('vec',-1),exprs)
      if ~ok then 
		warning('Invalid parameters!');
		break;
	  end
      in=[],
      if exists('outport') then out=ones(outport,1), else out=1, end
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[zigbee_ch];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
	
  case 'define' then
    zigbee_ch=1
    model=scicos_model()
    model.sim=list('flex_zigbee_receive',4)
    model.in=[],
    if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin=1
    model.rpar=[]
    model.ipar=[zigbee_ch]
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(zigbee_ch)]
    gr_i=['xstringb(orig(1),orig(2),..
		[''FLEX'' ; ''ZIGBEE'';..
		''REC CH ''+string(zigbee_ch)],..
		sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
