function [x,y,typ] = FLEX_MTB_touch(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    axis = exprs(1)
    res = exprs(2)
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
	  [ok,axis,res,exprs] = getvalue('Touch panel parameters interface',..
	   ['Axis [''X'' or ''Y'']:';..
	   'Resolution [100..4096]:'],..
	   list('str',1,'vec',-1),exprs)
      if ~ok then break,end
        
      if(axis~='X' & axis~='Y') then 
        warning('Accepted axis values are in [X,Y]. Keeping previous values.');
        break
      end
      if(res<100 | res>4096) then
        warning('Accepted resolution values in [100..4096]. Keeping previous values.');
        break;
      end
       
      in=[],
      if exists('outport') then out=ones(outport,1), else out=1, end
	  
	  [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[asciimat(axis);
                    res];
        model.dstate=[];
        x.graphics=graphics;x.model=model
        break
      end
   end
  case 'define' then
    axis = 'X'
	res = 100
    model = scicos_model()
    model.sim = list('flex_daughter_touch',4)
    model.in=[],
    if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin=1
    model.rpar=[]
    model.ipar=[asciimat(axis);
                res]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[axis,sci2exp(res)]	
    gr_i=['xstringb(orig(1),orig(2),..
         [''MTB touch'' ; ''Axis: ''+ axis;..
		  ''Resolution:'' + string(res)],..
		  sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
