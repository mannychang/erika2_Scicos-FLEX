function [pm]=xi2pm(xi)
// approximate the phase marin of a 2.order
// system with damping "xi"

pm=atan(2*xi/sqrt(-2*xi^2+sqrt(1+4*xi^4)));
pm=pm/%pi*180;

endfunction
