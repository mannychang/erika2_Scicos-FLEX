//###################################
// ScicosLab pack functions
//###################################

function res = EE_debug_printf(msg, on_off)
// This function prints a message only if on_off > 0.

[lhs , rhs] = argn(0);
if rhs == 1
  on_off = 0;
end
disp(msg);
if on_off > 0
    mfprintf(on_off, msg);
	mfprintf(on_off, "\n");
end
res = on_off;






