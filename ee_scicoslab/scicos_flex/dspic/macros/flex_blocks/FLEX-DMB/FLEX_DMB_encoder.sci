function [x,y,typ] = FLEX_DMB_encoder(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  
  case 'plot' then
    exprs = arg1.graphics.exprs;
    enc_type = exprs(1);
	enc_gain = exprs(2);
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
      [ok, enc_type, enc_gain, exprs] = getvalue('Encoder parameters:',..
      [' Encoder type [0(hw encoder),1(sw encoder)] :';..
	   ' Encoder gain :'],..
      list('vec',1,'vec',1),exprs);
      if ~ok then 
		warning('FLEX_DMB_encoder: Invalid parameters!');
		break;
	  end
	  if((enc_type>1) | (enc_type<0)) then
		warning('Encoder type unknown!. Keeping previous values.');
		break;
	  end
      in=[];
      out = 1;
      [model,graphics,ok] = check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs = exprs;
        model.rpar = [enc_gain];
        model.ipar = [enc_type];
        model.dstate = [1];
        x.graphics = graphics;
		x.model = model;
        break
      end
    end
  case 'define' then
    enc_type = 0;
	enc_gain = 1.0;
    model = scicos_model();
    model.sim = list('flex_daughter_encoder',4);
    model.in = [];
    model.out = 1;
    model.evtin = 1;
    model.rpar = [enc_gain];
    model.ipar = [enc_type];
    model.dstate = [1];
    model.blocktype = 'd';
    model.dep_ut = [%t %f];
    exprs = [sci2exp(enc_type); sci2exp(enc_gain)];
    gr_i = ['xstringb(orig(1),orig(2),[''FLEX-DMB'' ; ''Enc.In : ''+ string(enc_type)],sz(1),sz(2),''fill'');'];
    x = standard_define([3 2],model,exprs,gr_i);
  end
endfunction
