  function [x,y,typ] = RT_randm(job,arg1,arg2)
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
    x = arg1; 
    model=arg1.model;
    graphics=arg1.graphics;
    exprs=graphics.exprs;
    while %t do
      [ok, A, seed, exprs] = getvalue('Set Random block parameters',..
                              ['Amplitude:';
                               'Seed:'],...
                               list('vec',-1,'vec',-1),exprs);
      
      if ~ok then break,end
      
      if exists('outport') then 
          out = ones(outport,1), 
          in=[],
      else
          out=1, in=[],
      end  
      
      [model,graphics,ok] = check_io(model,graphics,in,out,1,[])
      
      if ok then
        graphics.exprs = exprs;
        model.rpar = [A, seed];
        model.ipar = [];
        //** Finalize the data
        x.graphics = graphics;
        x.model    = model;
        break ; //exit from the infinite while loop 
      end

    end //** end of the infinite while loop 
  
  case 'define' then
    //** default values 
    A = 1.0 ;
    seed = 0.0; 
    model = scicos_model()
    model.sim = list('rt_randm',4); 
    if exists('outport') then model.out=ones(outport,1), model.in=[], else model.out=1, model.in=[], end
    model.evtin = 1 ; 
    model.rpar  = [A,seed]; 
    model.ipar  = [];
    model.blocktype='d';
    model.dep_ut=[%t %f];
    exprs=[sci2exp(A),sci2exp(seed)];
    gr_i=['xstringb(orig(1),orig(2),''Random'',sz(1),sz(2),''fill'');'];
    x = standard_define([3 2],model,exprs,gr_i); 

end //** of select job 

endfunction
