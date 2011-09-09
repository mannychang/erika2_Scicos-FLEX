function [x,y,typ] = EASYLAB_pwm(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    pwm_freq=exprs(1)
    pwm_pin=exprs(2) 
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
    error_par = 0;
    while %t do
        [ok,pwm_freq,pwm_pin,exprs]=..
        getvalue('Select Easylab PWM Output',..
        ['PWM Freq [1000..100000] :';..
        'PWM Out [1..4] :'],..
        list('vec',-1,'vec',-1),exprs)
        if ~ok then break,end
        if((pwm_freq < 1000) | (pwm_freq > 100000)) then
            warning('Accepted values for pwm frequency are in [1000, 100000]. Keeping previous values.');
            error_par = 1;
        end
        if((pwm_pin < 1) | (pwm_pin > 4)) then
            warning('Accepted values for pwm channel are in [1,4]. Keeping previous values.');
            error_par = 1;
        end
        if(error_par)
            break;
        end
        if exists('inport') then in=ones(inport,1), else in=1, end
        out=[]
        [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
        if ok then
            graphics.exprs=exprs;
            model.rpar=[pwm_freq];
            model.ipar=[pwm_pin];
            model.dstate=[1];
            x.graphics=graphics;x.model=model
            break
        end
    end
  case 'define' then
    pwm_pin=1
    pwm_freq=20000
    model=scicos_model()
    model.sim=list('easylab_pwm',4)
    if exists('inport') then model.in=ones(inport,1), else model.in=1, end
    model.out=[]
    model.evtin=1
    model.rpar=[pwm_freq]
    model.ipar=[pwm_pin]
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(pwm_freq);sci2exp(pwm_pin)]
    gr_i=['xstringb(orig(1),orig(2),..
            [''EASYLAB PWM'';..
            ''PWM Pin: '' + string(pwm_pin);..
            ''PWM Freq:'' + string(pwm_freq)],..
            sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
