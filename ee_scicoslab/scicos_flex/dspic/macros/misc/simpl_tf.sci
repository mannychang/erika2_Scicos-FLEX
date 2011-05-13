function g=simpl_tf(h,index)
p=coeff(h.num);
p=p(index);
h.num=poly(p,'s','coeff')
g=ss2tf(tf2ss(h));

endfunction
