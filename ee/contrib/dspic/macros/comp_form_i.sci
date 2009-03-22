function [Contr]=comp_form_i(A,B,C,D,Ts,K,Cy)
// Create the compact form of the Observer ABCD and the
// gain K, using an integrator at the input to eliminate the
// steady state error
//
// A,B,C,D: Observer matrices
// Ts: sampling time
// K: state feedback gains
// Cy: matrix to extract the output for the steady state feedback

[larg,rarg]=argn(0);

if rarg ~= 7 then
	Cy = [1];
end

ss_sys=syslin('d',A,B,C,D);
ss_sys(7)=Ts;
g_sys=ss2tf(ss_sys);

g_int=syslin('d',Ts/(%z-1));
g_int(7)=Ts;

gu=g_sys('num')(:,1)./g_sys('den')(:,1);
gy=g_sys('num')(:,2:$)./g_sys('den')(:,2:$);

nn=size(K,2);

Ke = K(1,nn);
K  = K(1,1:nn-1);

Greg=[-Ke*g_int/(1+K*gu),(Ke*Cy*g_int-K*gy)/(1+K*gu)];
Contr=tf2ss(Greg);

endfunction

