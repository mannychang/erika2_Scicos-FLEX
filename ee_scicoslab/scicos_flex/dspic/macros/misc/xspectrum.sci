function Txy=xspectrum(x,y)
// System identification with cross spectral analysis

nfft=int(size(x,'*')/2);
wind=window('hn',nfft)';
n=size(x,'*');
nwind=size(wind,'*');
index=1:nwind;
k=fix(n/nwind);

Pxx=zeros(nfft,1);
Pxy2=Pxx;Pxy=Pxx;

for i=1:k
  xw=wind.*detrend(x(index));
  yw=wind.*detrend(y(index));
  index=index+nwind;
  Xx=fft(xw(1:nfft));
  Yy=fft(yw(1:nfft));
  Xx2=abs(Xx).^2;
  Xy=Yy .* conj(Xx);
  Pxx=Pxx+Xx2;
  Pxy=Pxy+Xy;
end

if modulo(nfft,2)==1 then
  selct=(nfft+1)/2;
else
  selct=nfft/2+1;
end

Pxx=Pxx(1:selct);
Pxy=Pxy(1:selct);

Txy = Pxy./Pxx;

endfunction







