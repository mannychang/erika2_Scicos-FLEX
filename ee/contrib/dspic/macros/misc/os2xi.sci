function [xi]=os2xi(os)
// Calculates the damping factor xi for a 2. order system
// with %OS os

os=os/100;
xi=-log(os)/sqrt(%pi*%pi+log(os)*log(os));

endfunction
