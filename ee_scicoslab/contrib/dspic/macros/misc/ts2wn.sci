function [wn]=ts2wn(ts,xi)
// Calculates the natural frequency of a 2. order system
// with damp "xi" and setting time "ts"

tetaxi=acos(xi);
wn=(-log(0.02*sqrt(1-xi*xi)))/(ts*xi);

endfunction
