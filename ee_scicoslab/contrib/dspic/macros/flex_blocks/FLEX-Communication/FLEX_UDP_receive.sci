// FLEX_UDP_receive

function [x,y,typ] = FLEX_UDP_receive(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    flex_port_id 	= exprs(1);
    pc_port_id 		= exprs(2);
	flex_ip 		= exprs(3);
	flex_mask 		= exprs(4);
	flex_gate 		= exprs(5);
	flex_prim 		= exprs(6);
	flex_sec 		= exprs(7);
	pc_ip 			= exprs(8);
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
    old_flex_port_id 	= exprs(1);
    old_pc_port_id 		= exprs(2);
	old_flex_ip 		= exprs(3);
	old_flex_mask 		= exprs(4);
	old_flex_gate 		= exprs(5);
	old_flex_prim 		= exprs(6);
	old_flex_sec 		= exprs(7);
	old_pc_ip 			= exprs(8);
    while %t do
      [ok, flex_port_id, pc_port_id, flex_ip, flex_mask, flex_gate, flex_prim, flex_sec, pc_ip, exprs]=..
      getvalue('UDP Receive Parameters:',..
      ['FLEX Port number [1...65535]:';..
	  'PC Port number [1...65535]:';..
	  'FLEX IP [es. 127 0 0 1]:';..
	  'FLEX MASK [es. 255 255 255 0]:';..
	  'FLEX GATEWAY [es. 127 0 0 1]:';..
	  'FLEX PRIMARY DNS [es. 127 0 0 1]:';..
	  'FLEX SECONDARY DNS [es. 127 0 0 1]:';..
	  'PC IP [es. 127 0 0 1]:'],..
      list('vec',1,'vec',1,'vec',4,'vec',4,'vec',4,'vec',4,'vec',4,'vec',4),exprs);
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
        model.ipar=[flex_port_id,
					pc_port_id,
					flex_ip(1),
					flex_ip(2),
					flex_ip(3),
					flex_ip(4),
					flex_mask(1),
					flex_mask(2),
					flex_mask(3),
					flex_mask(4),
					flex_gate(1),
					flex_gate(2),
					flex_gate(3),
					flex_gate(4),
					flex_prim(1),
					flex_prim(2),
					flex_prim(3),
					flex_prim(4),
					flex_sec(1),
					flex_sec(2),
					flex_sec(3),
					flex_sec(4),
					pc_ip(1),
					pc_ip(2),
					pc_ip(3),
					pc_ip(4)];
        model.dstate=[1];
        x.graphics=graphics;x.model=model;
        break
      end
    end
	
  case 'define' then
    // Default values: 
	flex_port_id 	= 9760;																		
    pc_port_id 		= 9761;																		
	flex_ip 		= [192,168,0,2];
	flex_mask 		= [255,255,255,0];
	flex_gate 		= [192,168,10,1];
	flex_prim 		= [192,168,10,1];
	flex_sec 		= [192,168,10,1];
	pc_ip 			= [192,168,0,1];
    model=scicos_model();
    model.sim=list('flex_udp_receive',4);
    //if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.in = [];																				// we set a fixed input dimension = 15
    model.out = ones(15,1);
    model.evtin=1;
    model.rpar=[];
    model.ipar=[flex_port_id,
					pc_port_id,
					flex_ip(1),
					flex_ip(2),
					flex_ip(3),
					flex_ip(4),
					flex_mask(1),
					flex_mask(2),
					flex_mask(3),
					flex_mask(4),
					flex_gate(1),
					flex_gate(2),
					flex_gate(3),
					flex_gate(4),
					flex_prim(1),
					flex_prim(2),
					flex_prim(3),
					flex_prim(4),
					flex_sec(1),
					flex_sec(2),
					flex_sec(3),
					flex_sec(4),
					pc_ip(1),
					pc_ip(2),
					pc_ip(3),
					pc_ip(4)];
    model.dstate=[1];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(flex_port_id);..
           sci2exp(pc_port_id);..
		   sci2exp(flex_ip);..
		   sci2exp(flex_mask);..
		   sci2exp(flex_gate);..
		   sci2exp(flex_prim);..
		   sci2exp(flex_sec);..
		   sci2exp(pc_ip)];
		   
	gr_i=['xstringb(orig(1),orig(2),..
		[''FLEX UDP'';..
		 ''RECEIVE'';..
		 ''FLEX PORT: ''+string(flex_port_id);..
         ''PC PORT: ''+string(pc_port_id);..
		 ''FLEX IP: ''+string(flex_ip);..
		 ''PC IP: ''+string(pc_ip)],..
		sz(1),sz(2),''fill'');'];
		
    x=standard_define([5 4],model,exprs,gr_i);
  end
endfunction
