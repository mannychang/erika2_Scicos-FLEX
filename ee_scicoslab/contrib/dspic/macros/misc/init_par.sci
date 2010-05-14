function [xi,wn,s]=init_par(os,ts)
// Calculates the damping factor xi, the natural frequency wn
// and the pol paar s for a 2. order system with %OS os and
// setting time ts

xi=os2xi(os);
wn=ts2wn(ts,xi);
th=acos(xi);
s=-xi*wn+%i*wn*sqrt(1-xi*xi);

endfunction
