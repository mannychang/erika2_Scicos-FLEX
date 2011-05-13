function [A_f,B_f,C_f,D_f]=fullobs(A,B,C,D,poles)
// Find the full order observer for the system A,B,C,D, with observer poles "poles"

L = ppol(A',C',poles)'

A_f = A-L*C;
B_f = [B-L*D,L];

[m1,n1]=size(A_f);
[m2,n2]=size(B_f);

C_f = eye(m1,n1);
D_f = zeros(m1,n2);

endfunction
