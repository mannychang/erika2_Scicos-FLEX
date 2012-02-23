function [x,y,typ] = AMAZING_tuner(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  
  case 'plot' then
    exprs=arg1.graphics.exprs;
    res_x = exprs(1)
    res_y = exprs(2)
    standard_draw(arg1)
	
  case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
	
  case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
	
  case 'getorigin' then
    [x,y]=standard_origin(arg1)
	
  case 'set' then
    x=arg1
    model=arg1.model;graphics=arg1.graphics;
    exprs=graphics.exprs;
    while %t do
	  [ok,res_x,res_y,exprs] = getvalue('Amazing touch panel tuner',..
	   ['X Resolution [100..4096]';..
	   'Y Resolution [100..4096]:'],..
	   list('vec',-1,'vec',-1),exprs)
      if ~ok then 
		break;
	  end 
      if(res_x<100 | res_x>4096) then
        warning('Accepted resolution values in [100..4096]. Keeping previous value.');
        break;
      end

       if(res_y<100 | res_y>4096) then
        warning('Accepted resolution values in [100..4096]. Keeping previous value.');
        break;
      end
       
      in=[],
      out = [],
     [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[res_x;
                    res_y];
        model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
   end
   
  case 'define' then
    res_x = 240 
    res_y = 180
    model = scicos_model()
    model.sim = list('amazing_tuner',4)
    model.in=[],
    model.out=[],
    model.evtin=1
    model.rpar=[]
    model.ipar=[res_x;res_y]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(res_x);sci2exp(res_y)]	
    gr_i=['xstringb(orig(1),orig(2),..
         [''AMAZING'';..
	  ''tuner'' ;..
	  string(res_x) + ''x'' + string(res_y)],..
	  sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
