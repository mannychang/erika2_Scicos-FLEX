function [x,y,typ] = FLEX_randm(job,arg1,arg2)
  [lhs,rhs] = argn(0);
  if (rhs == 1)
    [x,y,typ] = RT_randm(job);
  elseif (rhs == 2)
    [x,y,typ] = RT_randm(job, arg1);
  elseif (rhs == 3)
    [x,y,typ] = RT_randm(job, arg1, arg2);
  else
    disp('Error randm: bad rhs: '+string(rhs))
    x=[];
    y=[];
    typ=[];
  end
endfunction
