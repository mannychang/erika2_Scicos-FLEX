function []=bb_step(sl,tf,index,str)
// Step response for the system "sl" from 0 to final time "tf"
[lhs,rhs]=argn(0);
t=0:0.001:tf;
y=csim('step',t,sl);
plot2d(t,y);xgrid(4);
if rhs==4 then 
   xstring(t(index),y(index),str) 
end

endfunction


  
