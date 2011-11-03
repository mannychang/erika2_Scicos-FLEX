// FLEX_serial_config

function [x,y,typ] = FLEX_serial_config(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    flex_uart_baudrate   = exprs(1);
    standard_draw(arg1);

  case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1);

  case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1);

  case 'getorigin' then
    [x,y]=standard_origin(arg1);

  case 'set' then
    x=arg1;
    model=arg1.model;
	graphics=arg1.graphics;
    exprs=graphics.exprs;
    while %t do
      [ok,..
      flex_uart_baudrate,..
      exprs] = ..
      getvalue('UART Configuration:',..
      ['FLEX UART baudrate:'],..
      list('vec',1), exprs);
      
      if ~ok then break,end
      mywarn = 0;
      
      if(flex_uart_baudrate<9600 | flex_uart_baudrate>115200) then
        warning('Invalid range for FLEX UART baudrate [9600..115200]. Keeping previous values.');
        mywarn = mywarn+ 1;
      end

      if mywarn > 0 then break, end

      in = [];
      out = [];
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[flex_uart_baudrate];
        model.ipar=[2];
        model.dstate=[1];
        x.graphics=graphics;
		x.model=model;
        break
      end
    end

  case 'define' then
    // Default values: 
	flex_uart_baudrate = 115200;
   
    model=scicos_model();
    model.sim=list('flex_serial_config',4);
    model.in = [];
    model.out = [];
    model.evtin=1;
    model.rpar=[flex_uart_baudrate];
    model.ipar=[2];
    model.dstate=[1];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(flex_uart_baudrate)];  
    gr_i=['xstringb(orig(1),orig(2),..
        [''FLEX Serial Config'';..
        ''UART baudrate: ''+string(flex_uart_baudrate)],..
        sz(1),sz(2),''fill'');'];
        
    x=standard_define([5 4],model,exprs,gr_i);
  end
endfunction
