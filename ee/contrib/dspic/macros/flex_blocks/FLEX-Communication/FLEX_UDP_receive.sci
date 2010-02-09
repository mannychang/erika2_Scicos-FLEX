// Expected integer params 
//port_id=exprs(1);				// UDP port number
//dest_addr_b1=exprs(2);		// Destination address byte 1
//dest_addr_b2=exprs(3);		// Destination address byte 2
//dest_addr_b3=exprs(4);		// Destination address byte 3
//dest_addr_b4=exprs(5);		// Destination address byte 4
//num_buf=exprs(6);				// Number of buffers for UDP packets transmission

function [x,y,typ] = FLEX_UDP_receive(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    flex_port_id=exprs(1);
    pc_port_id=exprs(2);
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
    old_flex_port_id = exprs(1);
    old_pc_port_id = exprs(2);
    while %t do
      [ok, flex_port_id, pc_port_id, exprs]=..
      getvalue('UDP Receive Params',..
      ['FLEX Port number [1...65535]:';..
	  'PC Port number [1...65535]:'],..
      list('vec',1,'vec',1),exprs);
      if ~ok then break,end
      if(flex_port_id<1 | flex_port_id>65535) then
        warning('Invalid range for FLEX Port number. Keeping previous values.');
        flex_port_id = old_flex_port_id;
        break;
      end
      if(pc_port_id<1 | pc_port_id>65535) then
        warning('Invalid range for PC Port number. Keeping previous values.');
        pc_port_id = old_pc_port_id;
        break;
      end
      // if exists('inport') then in=ones(inport,1), else in=1, end
      in = [];																					// we set a fixed input dimension = 15 
      out = ones(15,1);
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[flex_port_id,pc_port_id];
        model.dstate=[1];
        x.graphics=graphics;x.model=model;
        break
      end
    end
  case 'define' then
    flex_port_id 	= 9760;																		// Default value 
    pc_port_id 		= 9761;																		// Default value 
    model=scicos_model();
    model.sim=list('flex_udp_receive',4);
    //if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.in = [];																				// we set a fixed input dimension = 15
    model.out = ones(15,1);
    model.evtin=1;
    model.rpar=[];
    model.ipar=[flex_port_id,pc_port_id];
    model.dstate=[1];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(flex_port_id);..
           sci2exp(pc_port_id)];
    gr_i=['xstringb(orig(1),orig(2),..
		[''FLEX UDP'';..
		 ''RECEIVE'';..
		 ''FLEX PORT: ''+string(flex_port_id);..
         ''PC PORT: ''+string(pc_port_id);..
		 ],..
		sz(1),sz(2),''fill'');'];
    x=standard_define([4 3],model,exprs,gr_i);
  end
endfunction
