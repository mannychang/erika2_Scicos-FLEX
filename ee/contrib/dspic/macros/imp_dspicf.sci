function imp_dspicf(folder,templatedir)
fnamein1 = SCI+'/contrib/dspic/compile.bat';
fnamein2 = SCI+'/contrib/dspic/compile.sh';
fnameout1  = folder+'/compile.bat';
fnameout2  = folder+'/compile.sh';

T=mgetl(fnamein1);
mputl(T,fnameout1);
T=mgetl(fnamein2);
mputl(T,fnameout2);
cd(folder)
cmd = 'start compile ' + templatedir
unix(cmd)
cd '..'
endfunction

