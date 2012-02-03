cd TESTDIR

load SCI/macros/scicos/lib

exec(loadpallibs,-1)

exec('mypalette.cosf',-1)

//names=[];
//for bl=scs_m.objs(1).model.rpar.objs,..
//	names=[names,bl.gui];
//end

if scs_m.objs(1).gui == 'PAL_f' then
	txt = [];
	for bl=scs_m.objs(1).model.rpar.objs,..
	txt=[txt; bl.gui];end
else
	txt = [];
	for bl=scs_m.objs,..
	txt=[txt; bl.gui];end
end

mputl(txt,'blocklist.txt');

quit
