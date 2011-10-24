// MCP2200: Communication block to configure Serial Communication 
// and intereact with GPIO pins

function [x,y,typ] = MCP2200(job,arg1,arg2)
    x=[];y=[];typ=[];
    select job
    case 'plot' then
        exprs=arg1.graphics.exprs;
        baudrate        = exprs(1);
        pin_in          = exprs(2);
        pin_out         = exprs(3);
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
            [ok, baudrate, pin_in, pin_out, exprs]=..
                getvalue('Set parameters for MCP2200 block',..
                ['Baudrate [9600,19200,57600,115200]:'; 'Input Pins:'; 'Output Pins:'],..
                list('vec', 1,'vec', -1, 'vec', -1),..
                exprs);
      
            if ~ok then break,end
            mywarn = 0;
            if((baudrate <> 9600) & (baudrate <> 19200) & (baudrate <> 57600) & (baudrate <> 115200)) then
                warning('Accepted values for serial port are in [9600,19200,57600,115200]. Keeping previous values.');
                mywarn = 1;
            end
            
            pin_in_aux  = pin_in;
            pin_out_aux = pin_out;
            pin_in = [];
            pin_out = [];
            in_length  = length(pin_in_aux);
            out_length = length(pin_out_aux);
            
            //Array column conversion
            for i = 1:in_length
                pin_in = [pin_in; pin_in_aux(i)];
                //check value
                if (pin_in_aux(i) < 0 | pin_in_aux(i) > 5) then
                    warning('Input Index: ' + string(i) + ' Pin' + string(pin_in_aux(i)) + ' out of range [0-5]! Keeping previous values.');
                    mywarn = mywarn + 1;
                end
                //check for duplicates
                for j = 1:in_length
                    if ((pin_in_aux(i) == pin_in_aux(j)) & (i > j)) then
                        warning('Make no sense have duplicated pin' + string(pin_in_aux(i)) + ' as input! Keeping previous values.');
                        mywarn = mywarn + 1;
                        break;
                    end
                end
            end
        
            for i = 1:out_length
                pin_out = [pin_out; pin_out_aux(i)];
                //check value
                if (pin_out_aux(i) < 0 | pin_out_aux(i) > 5) then
                    warning('Output Index: ' + string(i) + ' Pin' + string(pin_out_aux(i)) + ' out of range [0-5]! Keeping previous values.');
                    mywarn = mywarn + 1; 
                end
                //check for duplicates
                for j = 1:out_length
                    if ((pin_out_aux(i) == pin_out_aux(j)) & (i > j)) then
                        warning('Make no sense have duplicated pin' + string(pin_out_aux(i)) + ' as output! Keeping previous values.');
                        mywarn = mywarn + 1;
                        break;
                    end
                end
            end
            
            //Check for pin IN and out at the same time
            for i = 1:in_length
                for j = 1:out_length
                    if ((pin_in(i) == pin_out(j)) & (i > j)) then
                        warning('Invalid pin assignament. pin' + string(pin_in(i)) + ' cannot be IN and OUT at the same time! Keeping previous values.');
                        mywarn = mywarn + 1;
                        break;
                    end
                end 
            end

            if mywarn > 0 then break, end
      
            in  = ones(out_length, 1);
            out = ones(in_length, 1);
            [model,graphics,ok] = check_io(model,graphics,in,out,1,[]);
            if ok then
                graphics.exprs=exprs;
                model.rpar = baudrate;
                model.ipar = [pin_in;pin_out];
                model.dstate = [];
                x.graphics = graphics;
                x.model = model;
                break;
            end
        end
    case 'define' then
        baudrate = 115200;
        pin_in  = [];
        pin_out = [];
        model=scicos_model();
        model.sim=list('mcp2200_block', 4);
        model.in = [];
        model.out = [];
        model.evtin = 1;
        model.rpar = baudrate;
        model.ipar = [pin_in;pin_out]; 
        model.dstate = [];
        model.blocktype = 'd';
        model.dep_ut = [%t %f];
        exprs = [sci2exp(baudrate),sci2exp(pin_in),sci2exp(pin_out)];
        gr_i =  ['xstringb(orig(1),orig(2),..
            [''MCP2200''],..
            sz(1),sz(2),''fill'');'];
        x=standard_define([4 3], model, exprs, gr_i);
    end
endfunction
