// FLEX_UDP_receive

function [x,y,typ] = FLEX_UDP_receive(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    rx_ch_num = exprs(1);
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
      [ok, rx_ch_num, exprs]=..
      getvalue('UDP Receive block',..
      ['RX channel dim (1..15):'],..
      list('vec',1),exprs);
      
      if ~ok then break,end
      
      if(rx_ch_num<1 | rx_ch_num>15) then
        warning('Invalid value for Rx channel dimension (accepted range [1,15]). Keeping previous values.');
        break;
      end
      
      in = [];
      out = ones(rx_ch_num,1);
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar = rx_ch_num;
        model.dstate=[];
        x.graphics=graphics;x.model=model;
        break
      end
    end

  case 'define' then
    // Default values: 
    rx_ch_num = 2;
    model=scicos_model();
    model.sim=list('flex_udp_receive',4);
    model.in = [];
    model.out = ones(rx_ch_num,1);
    model.evtin=1;
    model.rpar=[];
    model.ipar = rx_ch_num;
    model.dstate=[];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(rx_ch_num)];
           
    gr_i=['xstringb(orig(1),orig(2),..
        [''FLEX UDP'';..
         ''Receive''],..
        sz(1),sz(2),''fill'');'];
        
    x=standard_define([4 3],model,exprs,gr_i);
  end
endfunction
