function k=bb_dlqr(fi,H,Q,R)
// Calculates the LQR gains for discrete systems
[n1,d1]=size(fi);
big=sysdiag(Q,R);
[w,wp]=fullrf(big,1e-20);
C1=wp(:,1:n1);
D12=wp(:,n1+1:$);
P=syslin('d',fi,H,C1,D12);
[k,X]=lqr(P);
k=-k;

endfunction
