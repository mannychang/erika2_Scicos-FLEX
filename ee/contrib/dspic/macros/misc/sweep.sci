function u=sweep(N,f_init,f_end,ts)
// Create a set of points for the chearp function
//
// N: signal size
// f_init, f_end: frequencies (initial and final)
// ts: sampling time

k=0:N-1;
fakt=0.1*N;
fk=f_init+1/(N-1)*k*(f_end-f_init);
wk=k/fakt.*(N-k)/fakt;
index=find(wk>1);
wk(index)=1;
u=wk.*sin(2*%pi*fk.*k*ts);
A0=-mean(u);
u=A0+u;

endfunction
