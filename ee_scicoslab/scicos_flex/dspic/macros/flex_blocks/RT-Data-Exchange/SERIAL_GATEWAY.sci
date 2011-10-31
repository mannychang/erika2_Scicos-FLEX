// SERIAL_GATEWAY:

function [x, y, typ] = SERIAL_GATEWAY(job, arg1, arg2)
    x=[];y=[];typ=[];
    select job
    case 'plot' then
        exprs = arg1.graphics.exprs;
        comm_port      = exprs(1);
        sending_port   = exprs(2);
        receiving_port = exprs(3);
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
            [ok, comm_port, sending_port, receiving_port, exprs]=..
            getvalue('Set parameters for Serial Gateway block',..
                ['COM port (1 = COM1, 2 = COM2 ecc.):'; 'Sending Port (for the gateway):'; 'Receiving Port (for the gateway):'],..
                list('vec', 1,'vec', 1, 'vec', 1),..
                exprs);
                 if ~ok then 
                break;
            end
            if (comm_port < 1) then
                warning('COM port must be a positive integer. Keep previous values');
                break;
            end
            in  = [];
            out = [];
            [model,graphics,ok] = check_io(model, graphics, in, out, 1, []);
            if ok then
                graphics.exprs = exprs;
                model.ipar = [comm_port;sending_port;receiving_port];
                model.dstate = [];
                x.graphics = graphics;
                x.model = model;
                break;
            end 
        end
    case 'define' then
        comm_port      = 1;
        sending_port   = 50002;
        receiving_port = 50001;
        model = scicos_model();
        model.sim = list('serial_gateway_block', 4);
        model.in  = [];
        model.out = [];
        model.evtin = 1;
        model.ipar = [comm_port;sending_port;receiving_port]; 
        model.dstate = [];
        model.blocktype = 'd';
        model.dep_ut = [%t %f];
        exprs = [sci2exp(comm_port),sci2exp(sending_port),sci2exp(receiving_port)];
        gr_i = ['xstringb(orig(1),orig(2),..
            [''Serial Gateway''],..
            sz(1),sz(2),''fill'');'];
        x = standard_define([4 3], model, exprs, gr_i);
    end
endfunction
