function imp_dspicf(folder,templatedir)
fnamein1 = SCI+'/contrib/scicos_ee/scicos_flex/dspic/compile.bat';
fnamein2 = SCI+'/contrib/scicos_ee/scicos_flex/dspic/compile.sh';
fnamein3 = SCI+'/contrib/scicos_ee/scicos_flex/dspic/flash.bat';
fnamein4 = SCI+'/contrib/scicos_ee/scicos_flex/dspic/flash.sh';

fnameout1  = folder+'/compile.bat';
fnameout2  = folder+'/compile.sh';
fnameout3  = folder+'/flash.bat';
fnameout4  = folder+'/flash.sh';

T=mgetl(fnamein1);
mputl(T,fnameout1);
T=mgetl(fnamein2);
mputl(T,fnameout2);
T=mgetl(fnamein3);
mputl(T,fnameout3);
T=mgetl(fnamein4);
mputl(T,fnameout4);

cd(folder)
cmd = 'start compile' + ' ' + SCI+'/contrib/scicos_ee/RT-Druid' + ' ' + templatedir
unix(cmd)
cd '..'
endfunction

