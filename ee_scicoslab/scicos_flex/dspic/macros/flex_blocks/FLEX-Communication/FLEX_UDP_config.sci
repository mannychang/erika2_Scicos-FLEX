// FLEX_UDP_config

function [x,y,typ] = FLEX_UDP_config(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    flex_port_id        = exprs(1);
    flex_ip_str         = exprs(2);
    flex_mask_str       = exprs(3);
    flex_gate_str       = exprs(4);
    flex_prim_str       = exprs(5);
    flex_sec_str        = exprs(6);
    pc_port_id          = exprs(7);
    pc_ip_str           = exprs(8);
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
      [ok,..
      flex_port_id,..
      flex_ip_str,..
      flex_mask_str,..
      flex_gate_str,..
      flex_prim_str,..
      flex_sec_str,..
      pc_port_id,.. 
      pc_ip_str,.. 
      exprs]=..
      getvalue('UDP Receive Parameters:',..
      ['FLEX Port number:';..
       'FLEX IP';..
       'FLEX MASK:';..
       'FLEX GATEWAY:';..
       'FLEX PRIMARY DNS:';..
       'FLEX SECONDARY DNS:';..
       'PC Port number:';..
       'PC IP:'],..
      list('vec',1,'str',-1,'str',-1,'str',-1,'str',-1,'str',-1,'vec',1,'str',-1),exprs);
      
      if ~ok then break,end
      mywarn = 0;
      
      if(flex_port_id<1 | flex_port_id>65535) then
        warning('Invalid range for FLEX Port number. Keeping previous values.');
        mywarn = mywarn+ 1;
      end  
      if(pc_port_id<1 | pc_port_id>65535) then
        warning('Invalid range for PC Port number. Keeping previous values.');
        mywarn = mywarn+ 1;
      end

      mystr = flex_ip_str;
      ind = strindex(mystr,'.');
      strnum = strsplit(mystr,[ind(1) ind(2) ind(3)]);
      flex_ip = [evstr(strnum(1)), evstr(strnum(2)), evstr(strnum(3)), evstr(strnum(4))];
      if(min(flex_ip)<0 | max(flex_ip)>255) then
        warning('Invalid range for FLEX IP address. Keeping previous values.');
        mywarn = mywarn+ 1;
      end

      mystr = flex_mask_str;
      ind = strindex(mystr,'.');
      strnum = strsplit(mystr,[ind(1) ind(2) ind(3)]);
      flex_mask = [evstr(strnum(1)), evstr(strnum(2)), evstr(strnum(3)), evstr(strnum(4))];
      if(min(flex_mask)<0 | max(flex_mask)>255) then
        warning('Invalid range for FLEX mask. Keeping previous values.');
        mywarn = mywarn+ 1;
      end

      mystr = flex_gate_str;
      ind = strindex(mystr,'.');
      strnum = strsplit(mystr,[ind(1) ind(2) ind(3)]);
      flex_gate = [evstr(strnum(1)), evstr(strnum(2)), evstr(strnum(3)), evstr(strnum(4))];
      if(min(flex_gate)<0 | max(flex_gate)>255) then
        warning('Invalid range for GATEWAY address. Keeping previous values.');
        mywarn = mywarn+ 1;
      end 

      mystr = flex_prim_str;
      ind = strindex(mystr,'.');
      strnum = strsplit(mystr,[ind(1) ind(2) ind(3)]);
      flex_prim = [evstr(strnum(1)), evstr(strnum(2)), evstr(strnum(3)), evstr(strnum(4))];
      if(min(flex_prim)<0 | max(flex_prim)>255) then
        warning('Invalid range for Primary DNS address. Keeping previous values.');
        mywarn = mywarn+ 1;
      end 
      
      mystr = flex_sec_str;
      ind = strindex(mystr,'.');
      strnum = strsplit(mystr,[ind(1) ind(2) ind(3)]);
      flex_sec = [evstr(strnum(1)), evstr(strnum(2)), evstr(strnum(3)), evstr(strnum(4))];
      if(min(flex_sec)<0 | max(flex_sec)>255) then
        warning('Invalid range for Secondary DNS address. Keeping previous values.');
        mywarn = mywarn+ 1;
      end 
      
      mystr = pc_ip_str;
      ind = strindex(mystr,'.');
      strnum = strsplit(mystr,[ind(1) ind(2) ind(3)]);
      pc_ip = [evstr(strnum(1)), evstr(strnum(2)), evstr(strnum(3)), evstr(strnum(4))];
      if(min(pc_ip)<0 | max(pc_ip)>255) then
        warning('Invalid range for PC IP address. Keeping previous values.');
        mywarn = mywarn+ 1;
      end
      
      if mywarn > 0 then break, end

      in = [];
      out = [];
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[flex_port_id,..
                    flex_ip(1),..
                    flex_ip(2),..
                    flex_ip(3),..
                    flex_ip(4),..
                    flex_mask(1),..
                    flex_mask(2),..
                    flex_mask(3),..
                    flex_mask(4),..
                    flex_gate(1),..
                    flex_gate(2),..
                    flex_gate(3),..
                    flex_gate(4),..
                    flex_prim(1),..
                    flex_prim(2),..
                    flex_prim(3),..
                    flex_prim(4),..
                    flex_sec(1),..
                    flex_sec(2),..
                    flex_sec(3),..
                    flex_sec(4),..
                    pc_port_id,..
                    pc_ip(1),..
                    pc_ip(2),..
                    pc_ip(3),..
                    pc_ip(4)];
        model.dstate=[];
        x.graphics=graphics;x.model=model;
        break
      end
    end

  case 'define' then
    // Default values: 
    flex_port_id    = 9760;
    
    flex_ip         = [192,168,0,2];
    flex_ip_str       = '192.168.0.2';
    
    flex_mask       = [255,255,255,0];
    flex_mask_str     = '255.255.255.0';
    
    flex_gate       = [192,168,0,1];
    flex_gate_str     = '192.168.0.1';
    
    flex_prim       = [192,168,0,1];
    flex_prim_str     = '192.168.0.1';
    
    flex_sec        = [192,168,0,1];
    flex_sec_str      = '192.168.0.1';
    
    pc_port_id      = 9761;
    
    pc_ip           = [192,168,0,1];
    pc_ip_str         = '192.168.0.1';
    
    model=scicos_model();
    model.sim=list('flex_udp_config',4);
    model.in = [];
    model.out = [];
    model.evtin=1;
    model.rpar=[];
    model.ipar=[flex_port_id,..
                    flex_ip(1),..
                    flex_ip(2),..
                    flex_ip(3),..
                    flex_ip(4),..
                    flex_mask(1),..
                    flex_mask(2),..
                    flex_mask(3),..
                    flex_mask(4),..
                    flex_gate(1),..
                    flex_gate(2),..
                    flex_gate(3),..
                    flex_gate(4),..
                    flex_prim(1),..
                    flex_prim(2),..
                    flex_prim(3),..
                    flex_prim(4),..
                    flex_sec(1),..
                    flex_sec(2),..
                    flex_sec(3),..
                    flex_sec(4),..
                    pc_port_id,..
                    pc_ip(1),..
                    pc_ip(2),..
                    pc_ip(3),..
                    pc_ip(4)];
    model.dstate=[];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(flex_port_id);..
           flex_ip_str;..
           flex_mask_str;..
           flex_gate_str;..
           flex_prim_str;..
           flex_sec_str;..
           sci2exp(pc_port_id);..
           pc_ip_str];  
    gr_i=['xstringb(orig(1),orig(2),..
        [''FLEX UDP Config'';..
         ''FLEX PORT: ''+string(flex_port_id);..
         ''FLEX IP: ''+string(flex_ip_str);..
         ''PC PORT: ''+string(pc_port_id);..
         ''PC IP: ''+string(pc_ip_str)],..
        sz(1),sz(2),''fill'');'];
        
    x=standard_define([5 4],model,exprs,gr_i);
  end
endfunction
