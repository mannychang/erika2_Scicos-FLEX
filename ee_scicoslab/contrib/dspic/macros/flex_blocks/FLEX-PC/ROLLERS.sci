function [x,y,typ] = ROLLERS(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    //exprs=arg1.graphics.exprs;
    //axis = exprs(1)
    standard_draw(arg1)
  case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
  case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
  case 'getorigin' then
    [x,y]=standard_origin(arg1)
  case 'set' then
    x = arg1;
    model = arg1.model;
    graphics = arg1.graphics;
    exprs = graphics.exprs;
    while %t do
	   [ok,axis1,axis2,axis3,axis4,axis5,axis6,exprs] = getvalue('Roller GUI (please, insert the configuration parameters of the rollers)',..
	   ['KPx[max,step,value]:';'TIx[max,step,value]:';'TDx[max,step,value]:';'KPy[max,step,value]:';'TIy[max,step,value]:';'TDy[max,step,value]:'],..
	   list('vec',3,'vec',3,'vec',3,'vec',3,'vec',3,'vec',3),exprs)
      if ~ok then 
		warning('ROLLERS: Invalid parameters!');
		break;
	  end
	  
	  if(axis1(1) < 1.0) then
		warning('Accepted values for axis1(1) are in [1.0,+inf]. Keeping previous value.');
		break;
	  end
	  if((axis1(2) < 0.0001)|(axis1(2) > axis1(1))) then
		warning('Accepted values for axis1(2) are in [0.0001,axis1(1)]. Keeping previous value.');
		break;
	  end
	  if((axis1(3) < 0.0)|(axis1(3) > axis1(1))) then
		warning('Accepted values for axis1(3) are in [0.0,axis1(1)]. Keeping previous value.');
		break;
	  end
	  
	  if(axis2(1) < 1.0) then
		warning('Accepted values for axis2(1) are in [1.0,+inf]. Keeping previous value.');
		break;
	  end
	  if((axis2(2) < 0.0001)|(axis2(2) > axis2(1))) then
		warning('Accepted values for axis2(2) are in [0.0001,axis2(1)]. Keeping previous value.');
		break;
	  end
	  if((axis2(3) < 0.0)|(axis2(3) > axis2(1))) then
		warning('Accepted values for axis2(3) are in [0.0,axis2(1)]. Keeping previous value.');
		break;
	  end
	  
	  if(axis3(1) < 1.0) then
		warning('Accepted values for axis3(1) are in [1.0,+inf]. Keeping previous value.');
		break;
	  end
	  if((axis3(2) < 0.0001)|(axis3(2) > axis3(1))) then
		warning('Accepted values for axis3(2) are in [0.0001,axis3(1)]. Keeping previous value.');
		break;
	  end
	  if((axis3(3) < 0.0)|(axis3(3) > axis3(1))) then
		warning('Accepted values for axis3(3) are in [0.0,axis3(1)]. Keeping previous value.');
		break;
	  end
	  
	  if(axis4(1) < 1.0) then
		warning('Accepted values for axis4(1) are in [1.0,+inf]. Keeping previous value.');
		break;
	  end
	  if((axis4(2) < 0.0001)|(axis4(2) > axis4(1))) then
		warning('Accepted values for axis4(2) are in [0.0001,axis4(1)]. Keeping previous value.');
		break;
	  end
	  if((axis4(3) < 0.0)|(axis4(3) > axis4(1))) then
		warning('Accepted values for axis4(3) are in [0.0,axis4(1)]. Keeping previous value.');
		break;
	  end
	  
	  if(axis5(1) < 1.0) then
		warning('Accepted values for axis5(1) are in [1.0,+inf]. Keeping previous value.');
		break;
	  end
	  if((axis5(2) < 0.0001)|(axis5(2) > axis5(1))) then
		warning('Accepted values for axis5(2) are in [0.0001,axis5(1)]. Keeping previous value.');
		break;
	  end
	  if((axis5(3) < 0.0)|(axis5(3) > axis5(1))) then
		warning('Accepted values for axis5(3) are in [0.0,axis5(1)]. Keeping previous value.');
		break;
	  end
	  
	  if(axis6(1) < 1.0) then
		warning('Accepted values for axis6(1) are in [1.0,+inf]. Keeping previous value.');
		break;
	  end
	  if((axis6(2) < 0.0001)|(axis6(2) > axis6(1))) then
		warning('Accepted values for axis6(2) are in [0.0001,axis6(1)]. Keeping previous value.');
		break;
	  end
	  if((axis6(3) < 0.0)|(axis6(3) > axis6(1))) then
		warning('Accepted values for axis6(3) are in [0.0,axis6(1)]. Keeping previous value.');
		break;
	  end
	  
      in = [];
      out = 7;
	  [model,graphics,ok] = check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs = exprs;
        model.rpar = [axis1;axis2;axis3;axis4;axis5;axis6];
        model.ipar = [];
        model.dstate = [];
        x.graphics = graphics;
        x.model = model;
        break
      end
    end
  case 'define' then
    axis1 = [10;0.001;1];
    axis2 = [10;0.001;1];
    axis3 = [10;0.001;1];
    axis4 = [10;0.001;1];
    axis5 = [10;0.001;1];
    axis6 = [10;0.001;1];
    model = scicos_model();
    model.sim = list('EvidenceRollers',4);
    model.in = [];
    model.out = 7;  //if exists('outport') then model.out=ones(outport,1), else model.out=1, end
    model.evtin = 1;
    model.rpar = [axis1;axis2;axis3;axis4;axis5;axis6];
    model.ipar = [];
    model.dstate = [];
    model.blocktype = 'd';
    model.dep_ut = [%t %f];
    exprs = ['[10;0.001;1]';'[10;0.001;1]';'[10;0.001;1]';'[10;0.001;1]';'[10;0.001;1]';'[10;0.001;1]'];	
    gr_i = ['xstringb(orig(1),orig(2),..
         [''ROLLER'';''GUI''],..
	  sz(1),sz(2),''fill'');'];
    x = standard_define([3 2],model,exprs,gr_i)
  end
endfunction

