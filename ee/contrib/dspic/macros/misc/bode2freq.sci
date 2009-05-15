function f=bode2freq(sys,val,fmin,fmax,typ)
// Interpolation for bode values

f=sqrt(fmin*fmax);
repf=repfreq(sys,[fmin,f,fmax]);
[db,phi]=dbphi(repf);

if typ=='db' then
  valf=db;
else
  valf=phi;
end

while(abs(val-valf(2))>1000*%eps)
  delta=val-valf;
  if delta(1)*delta(2) >=0 then
    fmin=f;
  else
    fmax=f;
  end
  f=sqrt(fmin*fmax);
  repf=repfreq(sys,[fmin,f,fmax]);
  [db,phi]=dbphi(repf);
  if typ=='db' then
    valf=db;
  else
    valf=phi;
  end
end

endfunction

