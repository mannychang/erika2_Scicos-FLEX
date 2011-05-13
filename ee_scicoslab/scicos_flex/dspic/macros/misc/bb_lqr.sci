function k=bb_lqr(a,b,Q,R)
// Calculates the LQR gains for continous systems
[n1,d1]=size(a);
big=sysdiag(Q,R);
[w,wp]=fullrf(big,1e-20);
C1=wp(:,1:n1);
D12=wp(:,n1+1:$);
P=syslin('c',a,b,C1,D12);
[k,X]=lqr(P);
k=-k;

endfunction

