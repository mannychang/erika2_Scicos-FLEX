// Expected integer params 
//my_addr_b1=exprs(1);		// my address byte 1
//my_addr_b2=exprs(2);		// My address byte 2
//my_addr_b3=exprs(3);		// My address byte 3
//my_addr_b4=exprs(4);		// My address byte 4
//my_port=exprs(5);			// My UDP port number

function [x,y,typ] = UDP_RECEIVER(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    my_addr_b1 	= exprs(1);
    my_addr_b2 	= exprs(2);
    my_addr_b3 	= exprs(3);
    my_addr_b4 	= exprs(4);
    my_port 	= exprs(5);
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
      [ok, my_addr_b1, my_addr_b2, my_addr_b3, my_addr_b4, my_port, exprs]=..
      getvalue('UDP Communication Parameters [default values for usb, (src:192.168.0.1,9761) for ethernet] :',..
      ['My address byte 1 [0...255]:';..
    	'My address byte 2 [0...255]:';..
    	'My address byte 3 [0...255]:';..
    	'My address byte 4 [0...255]:';..
    	'My port number [1...65535]:';..
       ],..
      list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1),exprs);
      if ~ok then break,end
      if(my_addr_b1<0 | my_addr_b1>255) then
        warning('Invalid range for My Address byte 1. Keeping previous values.');
        break;
      end
      if(my_addr_b2<0 | my_addr_b2>255) then
        warning('Invalid range for My Address byte 2. Keeping previous values.');
        break;
      end
      if(my_addr_b3<0 | my_addr_b3>255) then
        warning('Invalid range for My Address byte 3. Keeping previous values.');
        break;
      end
      if(my_addr_b4<0 | my_addr_b4>255) then
        warning('Invalid range for My Address byte 4. Keeping previous values.');
        break;
      end
      if(my_port<1 | my_port>65535) then
        warning('Invalid range for My Port number. Keeping previous values.');
        break;
      end
      
      // if exists('inport') then in=ones(inport,1), else in=1, end
      in = [];																	
      out= 15;	// we set a fixed OUTPUT dimension = 15 
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[my_addr_b1, my_addr_b2, my_addr_b3, my_addr_b4, my_port];
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
  	my_port	 	= 50000;		// Default value																
    model=scicos_model();
    model.sim=list('udp_receiver',4);
    //if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.in = [];															
    model.out=15;	// we set a fixed OUTPUT dimension = 15 
    model.evtin=1;
    model.rpar=[];
    model.ipar=[my_addr_b1, my_addr_b2, my_addr_b3, my_addr_b4, my_port];
    model.dstate=[];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(my_addr_b1);..
    		sci2exp(my_addr_b2);..
    		sci2exp(my_addr_b3);..
    		sci2exp(my_addr_b4);..
    		sci2exp(my_port);..
           	];
    gr_i=['xstringb(orig(1),orig(2),..
		[''PC UDP RECEIVER'';..
		 ''MY PORT: ''+string(my_port);..
		 ''MY IP: ''+string(my_addr_b1)+''.''+string(my_addr_b2)+''.''+string(my_addr_b3)+''.''+string(my_addr_b4);..
		 ],..
		sz(1),sz(2),''fill'');'];
    x=standard_define([5 4],model,exprs,gr_i);
  end
endfunction
