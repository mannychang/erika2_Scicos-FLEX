// RS232_CONFIG

function [x,y,typ] = RS232_CONFIG(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    pc_com_port     = exprs(1);
    pc_baudrate     = exprs(2);
    standard_draw(arg1)
    
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
      [ok, pc_com_port, pc_baudrate, exprs]=..
      getvalue('RS232 Communication Parameters',..
      ['PC COM port:';..
       'PC COM baudrate:'],.. 
      list('vec',1,'vec',1), exprs);

      if ~ok then break,end
      mywarn = 0;

      if( pc_com_port<0 |  pc_com_port>99) then
        warning('Invalid range for COM Port number. Keeping previous values.');
        mywarn = mywarn+ 1;
      end

      if(pc_baudrate<9600 | pc_baudrate>115200) then
        warning('Invalid range for COM port baudrate [9600..115200]. Keeping previous values.');
        mywarn = mywarn+ 1;
      end

      if mywarn > 0 then break, end
      
      in = [];
      out = [];
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[pc_baudrate];
        model.ipar=[pc_com_port];
        model.dstate=[];
        x.graphics=graphics;
        x.model=model;
        break
      end
    end
    
  case 'define' then
    pc_com_port = 1;
    pc_baudrate = 115200;

    model=scicos_model();
    model.sim=list('rs232_config',4);
    model.in = [];
    model.out = [];
    model.evtin=1;
    model.rpar=[pc_baudrate];
    model.ipar=[pc_com_port];
    model.dstate=[];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(pc_com_port);..
           sci2exp(pc_baudrate)];
    gr_i=['xstringb(orig(1),orig(2),..
         [''PC RS232 Config'';..
          ''Port: COM''+string(pc_com_port);..
          ''Baudrate: ''+string(pc_baudrate)],..
         sz(1),sz(2),''fill'');'];
    x=standard_define([5 4],model,exprs,gr_i);
  end
endfunction
