function yp=bb_interp(x,y,xp);
dk=splin(x,y);
yp=interp(xp,x,y,dk);

endfunction

