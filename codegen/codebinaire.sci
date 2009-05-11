
//==========================================================================
//used in do_compile_superblock
function vec=codebinaire(v,szclkIN)

  vec=zeros(1,szclkIN)
  for i=1:szclkIN
    w=v/2;
    vec(i)=v-2*int(w);
    v=int(w);
  end
endfunction
