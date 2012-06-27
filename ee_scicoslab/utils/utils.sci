//###################################
// ScicosLab pack functions
//###################################

function EE_debug_printf(msg, flag)
//% This function prints a message only if flag > 0.

[lhs , rhs] = argn(0);
if (rhs == 1)
  flag = 0;
end
if (flag > 0)
    mfprintf(flag, msg);
    mfprintf(flag, "\n");
elseif (flag==0)
    disp(msg)
end






