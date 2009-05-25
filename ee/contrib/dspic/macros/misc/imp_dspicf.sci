function imp_dspicf(folder,templatedir)
fnamein1 = SCI+'/contrib/dspic/compile.bat';
fnamein2 = SCI+'/contrib/dspic/compile.sh';
fnameout1  = folder+'/compile.bat';
fnameout2  = folder+'/compile.sh';
block_name_in = folder+'/'+rdnom+'.c';
block_name_out = folder+'/sci_'+rdnom+'.c';

T=mgetl(fnamein1);
mputl(T,fnameout1);
T=mgetl(fnamein2);
mputl(T,fnameout2);

T=mgetl(block_name_in);
mputl(T,block_name_out);
mdelete(block_name_in);

cd(folder)
cmd = 'start compile ' + templatedir
unix(cmd)
cd '..'
endfunction

