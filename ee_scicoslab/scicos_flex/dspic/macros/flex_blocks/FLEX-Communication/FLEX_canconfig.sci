// FLEX_canconfig

function [x,y,typ] = FLEX_canconfig(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    can_msg_id_1        = exprs(1);
    can_msg_id_2        = exprs(2);
    can_msg_id_3        = exprs(3);
    can_msg_id_4        = exprs(4);
    can_msg_id_5        = exprs(5);
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
    while %t do
      [ok,..
      can_msg_id_1,..
      can_msg_id_2,..
      can_msg_id_3,..
      can_msg_id_4,..
      can_msg_id_5,.. 
      exprs]=..
      getvalue('CAN configuration parameters:',..
      ['MESSAGE 1 ID:';..
       'MESSAGE 2 ID';..
       'MESSAGE 3 ID:';..
       'MESSAGE 4 ID:';..
       'MESSAGE 5 ID:'],..
      list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1),exprs);
      
      if ~ok then break,end
      mywarn = 0;
      
      if(can_msg_id_1<0 | can_msg_id_1>4294967281) then
        warning('Accepted values for id are in [0,4294967281]. Keeping previous values.');
        mywarn = mywarn+ 1;
      end
      if(can_msg_id_2<0 | can_msg_id_2>4294967281) then
        warning('Accepted values for id are in [0,4294967281]. Keeping previous values.');
        mywarn = mywarn+ 1;
      end
      if(can_msg_id_3<0 | can_msg_id_3>4294967281) then
        warning('Accepted values for id are in [0,4294967281]. Keeping previous values.');
        mywarn = mywarn+ 1;
      end
      if(can_msg_id_4<0 | can_msg_id_4>4294967281) then
        warning('Accepted values for id are in [0,4294967281]. Keeping previous values.');
        mywarn = mywarn+ 1;
      end
      if(can_msg_id_5<0 | can_msg_id_5>4294967281) then
        warning('Accepted values for id are in [0,4294967281]. Keeping previous values.');
        mywarn = mywarn+ 1;
      end
      
      if mywarn > 0 then break, end

      in = [];
      out = [];
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[]);
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[can_msg_id_1,..
                    can_msg_id_2,..
                    can_msg_id_3,..
                    can_msg_id_4,..
                    can_msg_id_5];
        model.dstate=[];
        x.graphics=graphics;x.model=model;
        break
      end
    end

  case 'define' then
    // Default values: 
    
    can_msg_id_1 = 1;
    can_msg_id_2 = 2;
    can_msg_id_3 = 3;
    can_msg_id_4 = 4;
    can_msg_id_5 = 5;
    
    model=scicos_model();
    model.sim=list('flex_can_config',4);
    model.in = [];
    model.out = [];
    model.evtin=1;
    model.rpar=[];
    model.ipar=[can_msg_id_1,..
                    can_msg_id_2,..
                    can_msg_id_3,..
                    can_msg_id_4,..
                    can_msg_id_5];
    model.dstate=[];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(can_msg_id_1);..
           sci2exp(can_msg_id_2);..
           sci2exp(can_msg_id_3);..
           sci2exp(can_msg_id_4);..
           sci2exp(can_msg_id_5)];  
    gr_i=['xstringb(orig(1),orig(2),..
         [''FLEX CAN Config'';..
          ''MSG1 ID: ''+string(can_msg_id_1);..
		  ''MSG2 ID: ''+string(can_msg_id_2);..
		  ''MSG3 ID: ''+string(can_msg_id_3);..
		  ''MSG4 ID: ''+string(can_msg_id_4);..
		  ''MSG5 ID: ''+string(can_msg_id_5)],..
          sz(1),sz(2),''fill'');'];
        
    x=standard_define([6 5],model,exprs,gr_i);
  end
endfunction
