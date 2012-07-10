// ***********************************************************
// * ScicosLab Pack Installer                                *
// * Requires: Scicoslab v4.4.1                              *
// * Built/Tested On: XP, Windows 7                          *
// * Description: Close link with the DLLs                   *
// * Copyright (c) 2012 Evidence Srl                         *
// * Author: Dario Di Stefano, Evidence                      *
// ***********************************************************

// This script is used to unlink DLLs
unlink_err = 0;

//% SMCUBE unlinking
bfunc = 'smcube_block';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end

//% Sources unlinking
bfunc = 'rt_sinus';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end

//% GW unlinking
bfunc = 'serial_gateway_block';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end

//% RS232 unlinking
bfunc = 'rs232_config';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end

//% MCP2200 unlinking
bfunc = 'mcp2200_block';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end

//% Rollers GUI unlinking
bfunc = 'EvidenceRollers';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end

//% UDP unlinking
bfunc = 'udp_config';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end

//% Native integer unlinking
bfunc = 'nat_gainblk_i32n';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end

//% iSIm unlinking
bfunc = 'flex_blocks';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end

//% Flex SIm unlinking
bfunc = 'flex_adcin';
[test,ilib] = c_link(bfunc);
if (test)
  ulink(ilib);
  if( c_link(bfunc) )
    disp('Error: Unable to remove '+bfunc+' DLL link!');
    unlink_err = -1;
  else
    disp('Removing DLL link...Done!');
  end
end
