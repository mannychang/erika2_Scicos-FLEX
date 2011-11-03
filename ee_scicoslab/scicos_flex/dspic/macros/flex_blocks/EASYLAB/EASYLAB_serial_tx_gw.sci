function [x,y,typ] = EASYLAB_serial_tx_gw(job,arg1,arg2)
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
        getvalue('Set parameters for EASYLAB serial send block',..
        ['Baudrate [9600,19200,57600,115200]:'],..
        list('vec',1),exprs)
        if ~ok then break,end
        if((baudrate <> 9600) & (baudrate <> 19200) & (baudrate <> 57600) & (baudrate <> 115200)) then
            warning('Accepted values for serial port are in [9600,19200,57600,115200]. Keeping previous values.');
            break;
        end
        //if exists('inport') then in=ones(inport,1), else in=1, end
        in=ones(2,1);    // we set a fixed input dimension = 2
        out=[];
        [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
        if ok then
            graphics.exprs=exprs;
            model.rpar=[baudrate];
            model.ipar=[];
            model.dstate=[1];
            x.graphics=graphics;x.model=model
            break;
        end
    end
    case 'define' then
        baudrate=115200;
        model=scicos_model();
        model.sim=list('easylab_serial_send',4);
        // if exists('inport') then model.in=ones(inport,1), else model.in=1, end
        model.in=ones(2,1);    // we set a fixed input dimension = 2 
        model.out=[];
        model.evtin=1;
        model.rpar=[baudrate];
        model.ipar=[];
        model.dstate=[1];
        model.blocktype='d';
        model.dep_ut=[%t %f];
        exprs=[sci2exp(baudrate)];
        gr_i=['xstringb(orig(1),orig(2),..
            [''Easylab Send GW'';..
            ''Baudrate: '' + string(baudrate)],..
            sz(1),sz(2),''fill'');'];
        x=standard_define([3 2],model,exprs,gr_i);
    end
endfunction
