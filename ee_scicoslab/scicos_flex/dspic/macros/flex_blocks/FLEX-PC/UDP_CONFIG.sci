// UDP_CONFIG

function [x,y,typ] = UDP_CONFIG(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    my_ip_addr_str     = exprs(1);
    my_port            = exprs(2);
    dst_ip_addr_str    = exprs(3);
    dst_port           = exprs(4);
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
      [ok, my_ip_addr_str, my_port, dst_ip_addr_str, dst_port, exprs]=..
      getvalue('UDP Communication Parameters (keep default values for usb)',..
      ['TX IP ADDRESS:';..
       'My port number:';..
       'REMOTE IP address:';..
       'Remote port:'],.. 
      list('str',-1,'vec',1,'str',-1,'vec',1),exprs);

      if ~ok then break,end
      mywarn = 0;
      
      mystr = my_ip_addr_str;
      ind = strindex(mystr,'.');
      strnum = strsplit(mystr,[ind(1) ind(2) ind(3)]);
      my_ip_addr = [evstr(strnum(1)), evstr(strnum(2)), evstr(strnum(3)), evstr(strnum(4))];
      if(min(my_ip_addr)<0 | max(my_ip_addr)>255) then
        warning('Invalid range for TX IP address. Keeping previous values.');
        mywarn = mywarn+ 1;
      end
      
      if(my_port<1 | my_port>65535) then
        warning('Invalid range for My Port number. Keeping previous values.');
        mywarn = mywarn+ 1;
      end
      
      mystr = dst_ip_addr_str;
      ind = strindex(mystr,'.');
      strnum = strsplit(mystr,[ind(1) ind(2) ind(3)]);
      dst_ip_addr = [evstr(strnum(1)), evstr(strnum(2)), evstr(strnum(3)), evstr(strnum(4))];
      if(min(dst_ip_addr)<0 | max(dst_ip_addr)>255) then
        warning('Invalid range for Remote IP address. Keeping previous values.');
        mywarn = mywarn+ 1;
      end
      
      if(dst_port<1 | dst_port>65535) then
        warning('Invalid range for remote Port number. Keeping previous values.');
        mywarn = mywarn+ 1;
      end

      if mywarn > 0 then break, end
      
      in = [];
      out = [];
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[my_ip_addr(1),.. 
                    my_ip_addr(2),..
                    my_ip_addr(3),..
                    my_ip_addr(4),..
                    my_port,.. 
                    dst_ip_addr(1),..
                    dst_ip_addr(2),..
                    dst_ip_addr(3),..
                    dst_ip_addr(4),..
                    dst_port];
        model.dstate=[];
        x.graphics=graphics;
        x.model=model;
        break
      end
    end
    
  case 'define' then
    my_ip_addr_str  = '127.0.0.1';
    my_ip_addr      = [127,0,0,1];
    my_port         = 50002;
    dst_ip_addr_str = '127.0.0.1';
    dst_ip_addr     = [127,0,0,1];
    dst_port        = 50001;

    model=scicos_model();
    model.sim=list('udp_config',4);
    model.in = [];
    model.out = [];
    model.evtin=1;
    model.rpar=[];
    model.ipar=[my_ip_addr(1),.. 
                my_ip_addr(2),..
                my_ip_addr(3),..
                my_ip_addr(4),..
                my_port,.. 
                dst_ip_addr(1),..
                dst_ip_addr(2),..
                dst_ip_addr(3),..
                dst_ip_addr(4),..
                dst_port];
    model.dstate=[];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[my_ip_addr_str;..
           sci2exp(my_port);..
           dst_ip_addr_str;..
           sci2exp(dst_port)];
    gr_i=['xstringb(orig(1),orig(2),..
         [''PC UDP Config'';..
          ''MY PORT: ''+string(my_port);..
          ''MY IP: ''+string(my_ip_addr_str);..
          ''DST PORT: ''+string(dst_port);..
          ''DST IP: ''+string(dst_ip_addr_str)],..
         sz(1),sz(2),''fill'');'];
    x=standard_define([5 4],model,exprs,gr_i);
  end
endfunction
