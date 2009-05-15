function [s]=xw2s(xi,wn)
// Calculates the complex pol paar "s" of a second order system
// with damping "xi" and natural frequency "wn"
s=-xi*wn+i*wn*sqrt(1-xi*xi);
endfunction
