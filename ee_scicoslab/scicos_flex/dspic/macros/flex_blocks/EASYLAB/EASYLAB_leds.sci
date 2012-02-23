function [x,y,typ] = EASYLAB_leds(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    led_threshold=exprs(1)
    gpout_pin=exprs(2)
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
        [ok,led_threshold, gpout_pin,exprs]=..
        getvalue('Select Easylab Led Number',..
        ['Threshold: [0.01..0.99]';..
        'LED [1-8] :'],..
        list('vec',-1,'vec',-1),exprs)
        if ~ok then break,end
        if((led_threshold < 0.01) | (led_threshold > 0.99)) then
            warning('Accepted values for led_threshold are in [0.01,0.99]. Keeping previous values.');
            break;
        end
        if((gpout_pin < 1) | (gpout_pin > 8)) then
            warning('Accepted values for led are in [1-8]. Keeping previous values.');
            break;
        end
        if exists('inport') then in=ones(inport,1), else in=1, end
        out=[]
        [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
        if ok then
            graphics.exprs=exprs;
            model.rpar=[led_threshold];
            model.ipar=[gpout_pin];
            model.dstate=[];
            x.graphics=graphics;x.model=model
            break
        end
    end
  case 'define' then
    gpout_pin=1
    led_threshold=0.5
    model=scicos_model()
    model.sim=list('easylab_leds',4)
    if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.out=[]
    model.evtin=1
    model.rpar=[led_threshold]
    model.ipar=[gpout_pin]
    model.dstate=[];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(led_threshold);sci2exp(gpout_pin)]
    gr_i=['xstringb(orig(1),orig(2),[''EASYLAB'' ; ''LEDS: ''+string(gpout_pin)+ '' Th: ''+string(led_threshold)],sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
