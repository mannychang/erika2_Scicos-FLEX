
// Modified for RT purposes by Roberto Bucher - RTAI Team
// roberto.bucher@supsi.ch

function rename(folder,newname,ext)
  oldname=folder+'/Makefile';
  newname=folder+'/'+newname;
  T=mgetl(oldname);
  T=strsubst(T,'.obj','.o');
  T=strsubst(T,'.o',ext);
  T=strsubst(T,SCI,WSCI);
  mputl(T,newname);
  mdelete(oldname);
endfunction

