function [num,den]=tfdata(G)
// Gen numerator and denominator from a transfer function
num=G('num');
den=G('den');

endfunction
