function [x,y,typ] = EASYLAB_CPUload_gpout(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    gpout_pin=exprs(1)
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
      [ok,gpout_pin,exprs]=..
      getvalue('Select Easylab Digital Output',..
      ['Dig.Out. pin [1..8] :'],..
      list('vec',-1),exprs)
      if ~ok then break,end
      if((gpout_pin < 1) | (gpout_pin > 8)) then
        warning('Accepted values for output pins are in [1,8]. Keeping previous values.');
        break;
      end
      in = [];
      out = [];
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[gpout_pin];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    gpout_pin=1
    model=scicos_model()
    model.sim=list('easylab_gpout',4)
    model.in = []
    model.out = []
    model.evtin=1
    model.rpar=[]
    model.ipar=[gpout_pin]
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(gpout_pin)]
    gr_i=['xstringb(orig(1),orig(2),[''EASYLAB''; ''CPU LOAD''; ''Dig.Out.: ''+string(gpout_pin)],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
