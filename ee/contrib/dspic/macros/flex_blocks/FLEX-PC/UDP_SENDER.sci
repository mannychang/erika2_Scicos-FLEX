// Expected integer params 
//my_addr_b1=exprs(1);		// my address byte 1
//my_addr_b2=exprs(2);		// My address byte 2
//my_addr_b3=exprs(3);		// My address byte 3
//my_addr_b4=exprs(4);		// My address byte 4
//my_port=exprs(5);			// My UDP port number
//dest_addr_b1=exprs(6);	// Destination address byte 1
//dest_addr_b2=exprs(7);	// Destination address byte 2
//dest_addr_b3=exprs(8);	// Destination address byte 3
//dest_addr_b4=exprs(9);	// Destination address byte 4
//dest_port=exprs(10);		// Destination UDP port number

function [x,y,typ] = UDP_SENDER(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    my_addr_b1 	= exprs(1);
    my_addr_b2 	= exprs(2);
    my_addr_b3 	= exprs(3);
    my_addr_b4 	= exprs(4);
    my_port 	= exprs(5);
    dst_addr_b1	= exprs(6);
    dst_addr_b2	= exprs(7);
    dst_addr_b3	= exprs(8);
    dst_addr_b4	= exprs(9);
    dst_port	= exprs(10);
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
      [ok, my_addr_b1, my_addr_b2, my_addr_b3, my_addr_b4, my_port, dst_addr_b1, dst_addr_b2, dst_addr_b3, dst_addr_b4, dst_port, exprs]=..
      getvalue('UDP Communication Parameters [default values for usb, (src:192.168.0.1,9762) and (dest:192.168.0.2,9760) for ethernet] :',..
      ['My address byte 1 [0...255]:';..
    	'My address byte 2 [0...255]:';..
    	'My address byte 3 [0...255]:';..
    	'My address byte 4 [0...255]:';..
    	'My port number [1...65535]:';..
       	'Destination Address byte 1 [0...255]:';..
       	'Destination Address byte 2 [0...255]:';..
       	'Destination Address byte 3 [0...255]:';..
       	'Destination Address byte 4 [0...255]:';..
       	'Destination port [1...65535]:'],..
      list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1),exprs);
      if ~ok then break,end
      if(my_addr_b1<0 | my_addr_b1>255) then
        warning('Invalid range for My Address byte 1. Keeping previous values.');
        my_addr_b1 = exprs(1);
        break;
      end
      if(my_addr_b2<0 | my_addr_b2>255) then
        warning('Invalid range for My Address byte 2. Keeping previous values.');
        my_addr_b2 = exprs(2);
        break;
      end
      if(my_addr_b3<0 | my_addr_b3>255) then
        warning('Invalid range for My Address byte 3. Keeping previous values.');
        my_addr_b3 = exprs(3);
        break;
      end
      if(my_addr_b4<0 | my_addr_b4>255) then
        warning('Invalid range for My Address byte 4. Keeping previous values.');
        my_addr_b4 = exprs(4);
        break;
      end
      if(my_port<1 | my_port>65535) then
        warning('Invalid range for My Port number. Keeping previous values.');
        my_port = exprs(5);
        break;
      end
      if(dst_addr_b1<0 | dst_addr_b1>255) then
        warning('Invalid range for Destination Address byte 1. Keeping previous values.');
        dst_addr_b1 = exprs(6);
        break;
      end
      if(dst_addr_b2<0 | dst_addr_b2>255) then
        warning('Invalid range for Destination Address byte 2. Keeping previous values.');
        dst_addr_b2 = exprs(7);
        break;
      end
      if(dst_addr_b3<0 | dst_addr_b3>255) then
        warning('Invalid range for Destination Address byte 3. Keeping previous values.');
        dst_addr_b3 = exprs(8);
        break;
      end
      if(dst_addr_b4<0 | dst_addr_b4>255) then
        warning('Invalid range for Destination Address byte 4. Keeping previous values.');
        dst_addr_b4 = exprs(9);
        break;
      end
      if(dst_port<1 | dst_port>65535) then
        warning('Invalid range for My Port number. Keeping previous values.');
        my_port = exprs(10);
        break;
      end

      // if exists('inport') then in=ones(inport,1), else in=1, end
      in = 15;//ones(15,1);																		// we set a fixed input dimension = 15 
      out=[];
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[my_addr_b1, my_addr_b2, my_addr_b3, my_addr_b4, my_port, dst_addr_b1, dst_addr_b2, dst_addr_b3, dst_addr_b4, dst_port];
        model.dstate=[];
        x.graphics=graphics;
		x.model=model;
        break
      end
    end
  case 'define' then
  	my_addr_b1 	= 127;			// Default value
  	my_addr_b2 	= 0;			// Default value
  	my_addr_b3 	= 0;			// Default value
  	my_addr_b4 	= 1;			// Default value
  	my_port	 	= 50002;		// Default value
    dst_addr_b1 = 127;			// Default value																		
    dst_addr_b2 = 0;			// Default value																		
    dst_addr_b3 = 0;			// Default value																			
    dst_addr_b4 = 1;			// Default value																			
    dst_port	= 50001;		// Default value 																	
    model=scicos_model();
    model.sim=list('udp_sender',4);
    //if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.in = 15;//ones(15,1);																	// we set a fixed input dimension = 15
    model.out=[];
    model.evtin=1;
    model.rpar=[];
    model.ipar=[my_addr_b1, my_addr_b2, my_addr_b3, my_addr_b4, my_port, dst_addr_b1, dst_addr_b2, dst_addr_b3, dst_addr_b4, dst_port];
    model.dstate=[];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(my_addr_b1);..
    		sci2exp(my_addr_b2);..
    		sci2exp(my_addr_b3);..
    		sci2exp(my_addr_b4);..
    		sci2exp(my_port);..
           	sci2exp(dst_addr_b1);..
           	sci2exp(dst_addr_b2);..
           	sci2exp(dst_addr_b3);..
           	sci2exp(dst_addr_b4);..
           	sci2exp(dst_port)];
    gr_i=['xstringb(orig(1),orig(2),..
		[''PC UDP SENDER'';..
		 ''MY PORT: ''+string(my_port);..
		 ''MY IP: ''+string(my_addr_b1)+''.''+string(my_addr_b2)+''.''+string(my_addr_b3)+''.''+string(my_addr_b4);..
		 ''DST PORT: ''+string(dst_port);..
         ''DST IP: ''+string(dst_addr_b1)+''.''+string(dst_addr_b2)+''.''+string(dst_addr_b3)+''.''+string(dst_addr_b4);..
		 ],..
		sz(1),sz(2),''fill'');'];
    x=standard_define([5 4],model,exprs,gr_i);
  end
endfunction
