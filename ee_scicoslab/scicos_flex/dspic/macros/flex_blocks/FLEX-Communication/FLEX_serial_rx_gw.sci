function [x,y,typ] = FLEX_serial_rx_gw(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    baudrate=exprs(1);
    standard_draw(arg1);
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
        [ok,baudrate,exprs]=..
        getvalue('Set parameters for FLEX serial receive block',..
        ['Baudrate [9600,19200,57600,115200]:'],..
        list('vec',-1),exprs)
        if ~ok then break,end
        if((baudrate <> 9600) & (baudrate <> 19200) & (baudrate <> 57600) & (baudrate <> 115200)) then
            warning('Accepted values for serial port are in [9600,19200,57600,115200]. Keeping previous values.');
            break;
        end
        in=[];
        //if exists('outport') then out=ones(outport,1), else out=1, end
        out=ones(2,1);    // we set a fixed output dimension = 2
        [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
        if ok then
            graphics.exprs=exprs;
            model.rpar=[baudrate];
            model.ipar=[];
            model.dstate=[];
            x.graphics=graphics;x.model=model;
            break;
        end
    end
    case 'define' then

        baudrate=115200;
        model=scicos_model();
        model.sim=list('easylab_serial_receive',4);
        model.in=[];
        //if exists('outport') then model.out=ones(outport,1), else model.out=1, end
        model.out=ones(2,1);    // we set a fixed output dimension = 2
        model.evtin=1;
        model.rpar=[baudrate];
        model.ipar=[];
        model.dstate=[];
        model.blocktype='d';
        model.dep_ut=[%t %f];
        exprs=[sci2exp(baudrate)];
        gr_i=['xstringb(orig(1),orig(2),..
            [''FLEX Receive GW'';..
            ''Baudrate: '' + string(baudrate)],..
            sz(1),sz(2),''fill'');'];
        x=standard_define([3 2],model,exprs,gr_i);
    end
endfunction
