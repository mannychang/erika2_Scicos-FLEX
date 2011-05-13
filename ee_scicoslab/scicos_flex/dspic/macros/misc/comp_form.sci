function [Contr]=comp_form(A,B,C,D,Ts,K)
// Create the compact form of the Observer ABCD and the
// gain K, 
//
// A,B,C,D: Observer matrices
// Ts: sampling time
// K: state feedback gains

Bu=B(:,1);
By=B(:,2:$);
Du=D(:,1);
Dy=D(:,2:$);

X=inv(1+K*Du);

Ac=A-Bu*X*K*C;
Bc=[Bu*X,By-Bu*X*K*Dy]
Cc=-X*K*C;
Dc=[X,-X*K*Dy]
Contr=syslin('d',Ac,Bc,Cc,Dc)

endfunction
