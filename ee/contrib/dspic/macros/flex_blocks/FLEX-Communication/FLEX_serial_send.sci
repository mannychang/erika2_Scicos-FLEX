function [x,y,typ] = FLEX_serial_send(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    serial_port=exprs(1)
		baudrate=exprs(2)
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
      [ok,serial_port,baudrate,exprs]=..
      getvalue('Set parameters for FLEX serial send block',..
      ['Serial port [1,2] :';..
			'Baudrate [9600,19200,57600,115200'],..
			list('vec',-1,'vec',-1),exprs)
      if ~ok then break,end
      if exists('inport') then in=ones(inport,1), else in=1, end
      out=[]
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[baudrate];
        model.ipar=[serial_port];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    serial_port=1
		baudrate=115200
    model=scicos_model()
    model.sim=list('flex_serial_send',4)
    if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.out=[]
    model.evtin=1
    model.rpar=[baudrate]
    model.ipar=[serial_port]
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(serial_port);sci2exp(baudrate)]
    gr_i=['xstringb(orig(1),orig(2),..
					[''FLEX Serial'';..
					''Send Port: '' + string(serial_port);..
					''Baudrate: '' + string(baudrate)],..
					sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
