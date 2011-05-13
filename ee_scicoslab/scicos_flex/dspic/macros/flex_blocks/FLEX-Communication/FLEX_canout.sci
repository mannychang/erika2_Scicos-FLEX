function [x,y,typ] = FLEX_canout(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    can_msg_id = exprs(1);
    n_channels = exprs(2);
    standard_draw(arg1);
  case 'getinputs' then
    [x,y,typ] = standard_inputs(arg1);
  case 'getoutputs' then
    [x,y,typ] = standard_outputs(arg1);
  case 'getorigin' then
    [x,y] = standard_origin(arg1);
  case 'set' then
    x = arg1;
    model = arg1.model;
	graphics = arg1.graphics;
    exprs = graphics.exprs;
    while %t do
      [ok,can_msg_id,n_channels,exprs]=..
      getvalue('Set parameters for block FLEX-CAN OUT',..
      ['CAN Message ID:';..
			 'Number of elements to send [1,.,16]:'],..
      list('vec',-1,'vec',-1),exprs);
      if ~ok then break,end
	  if(can_msg_id<0) then
		warning('Accepted values for id are in [0,+inf]. Keeping previous values.');
		break;
	  end
	  if(n_channels<1 | n_channels>16) then
		warning('Accepted values for channels are in [1,16]. Keeping previous values.');
		break;
	  end
      // if exists('inport') then in=ones(inport,1), else in=1, end
      in = ones(16,1);
	  out=[]
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs = exprs;
        model.rpar = [];
        model.ipar = [can_msg_id, n_channels];
        model.dstate = [1];
        x.graphics = graphics;
		x.model = model;
        break;
      end
    end
  case 'define' then
    can_msg_id = 1;
    n_channels = 10;
    model = scicos_model();
    model.sim = list('flex_can_out',4);
    // if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.in = ones(16,1);
	model.out = [];
    model.evtin = 1;
    model.rpar = [];
    model.ipar = [can_msg_id, n_channels];
    model.dstate = [1];
    model.blocktype = 'd';
    model.dep_ut = [%t %f];
    exprs = [sci2exp(can_msg_id);..
					 sci2exp(n_channels)];
    gr_i =['xstringb(orig(1),orig(2),[''FLEX CAN OUT'' ; ''MSG ID: ''+string(can_msg_id)],sz(1),sz(2),''fill'');'];
    x = standard_define([5 4],model,exprs,gr_i);
  end
endfunction
