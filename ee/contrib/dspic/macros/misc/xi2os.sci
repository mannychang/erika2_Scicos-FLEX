function [os]=xi2os(xi)
// Calculate the %OS of a 2. order system with damping "xi"

os=100*exp(-xi*%pi/sqrt(1-xi*xi));

endfunction
