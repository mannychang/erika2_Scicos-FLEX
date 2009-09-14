function [x,y,typ] = AMAZING_tuning_reset(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    standard_draw(arg1)
  case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
  case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
  case 'getorigin' then
    [x,y]=standard_origin(arg1)
  case 'set' then
	  x=arg1;
    model=arg1.model;graphics=arg1.graphics;
		
		exprs=graphics.exprs;
    while %t do
	  [ok,exprs] = getvalue('Amazing tuning reset',..
	   [],[],exprs)
     if ~ok then break,end
				
    out=[],
    if exists('inport') then out=ones(inport,1), else in=1, end
	  
	  [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[];
        model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
   end
  case 'define' then
    model = scicos_model()
    model.sim = list('amazing_reset',4)
    if exists('inport') then model.in=ones(inport,1), else in=1, end
    model.out=[],
    model.evtin=1
    model.rpar=[]
    model.ipar=[]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[]	
    gr_i=['xstringb(orig(1),orig(2),..
         [''AMAZING'' ;..
	  ''tuning reset''],..
	  sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
