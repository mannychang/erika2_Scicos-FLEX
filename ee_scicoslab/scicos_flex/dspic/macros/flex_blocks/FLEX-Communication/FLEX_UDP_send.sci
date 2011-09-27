// FLEX_UDP_send

function [x,y,typ] = FLEX_UDP_send(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    tx_ch_num         = exprs(1);
    standard_draw(arg1)
    
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
      [ok, tx_ch_num, exprs]=..
      getvalue('UDP Send block',..
      ['TX channel dim (1..15):'],..
      list('vec',1),exprs);
      
      if ~ok then break,end
      
      if(tx_ch_num<1 | tx_ch_num>15) then
        warning('Invalid range for Tx channel dimension (accepted range [1,15]). Keeping previous values.');
        break;
      end
      
      in = ones(tx_ch_num,1);
      out=[];
      
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar= tx_ch_num;
        model.dstate=[1];
        x.graphics=graphics;x.model=model;
        break
      end
    end
    
  case 'define' then
    // Default values: 
    model=scicos_model();
    model.sim=list('flex_udp_send',4);
    tx_ch_num = 2;
    model.in = ones(tx_ch_num,1);
    model.out=[];
    model.evtin=1;
    model.rpar=[];
    model.ipar= tx_ch_num;
    model.dstate=[1];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(tx_ch_num)];   
    gr_i=['xstringb(orig(1),orig(2),..
        [''FLEX UDP'';..
         ''Transmit''],..
        sz(1),sz(2),''fill'');'];
    x=standard_define([4 3],model,exprs,gr_i);
  end
endfunction
