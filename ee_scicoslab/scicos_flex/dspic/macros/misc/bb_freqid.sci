function P=bb_freqid(x,y);
// System identification using energy spectra
// u: input signal
// y: output signal

nz=size(x,'*');
xf=fft(x);
yf=fft(y);
xf=xf(2:nz/2);
yf=yf(2:nz/2);

N=size(xf,'*');
Fxx=1/N*xf'.'.*xf;
Fxy=1/N*xf'.'.*yf;

P=Fxy./Fxx;

endfunction

