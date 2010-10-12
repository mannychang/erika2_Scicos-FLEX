function [x,y,typ] = AMAZING_touch(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  
  case 'plot' then
    exprs=arg1.graphics.exprs;
    axis = exprs(1)
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
	  [ok,axis,exprs] = getvalue('Touch panel parameters interface',..
	   ['Axis [''X'' or ''Y'']:'],..
	   list('str',1),exprs)
      if ~ok then 
		warning('AMAZING_touch: Invalid parameters!');
		break;
	  end 
      if(axis~='X' & axis~='Y') then 
        warning('Accepted axis values are in [X,Y]. Keeping previous value.');
        break
      end
       
      in=[],
      if exists('outport') then out=ones(outport,1), else out=1, end
	  
	  [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[asciimat(axis)];
        model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
   end
   
  case 'define' then
    axis = 'X'
    model = scicos_model()
    model.sim = list('amazing_touch',4)
    model.in=[],
    if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin=1
    model.rpar=[]
    model.ipar=[asciimat(axis)]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[axis]	
    gr_i=['xstringb(orig(1),orig(2),..
         [''AMAZING'';..
	  ''touch'' + '' '' + axis],..
	  sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
