// ***********************************************************
// * ScicosLab Pack Installer                                *
// * Requires: Scicoslab v4.4.1                              *
// * Built/Tested On: XP, Windows 7                          *
// * Description: Close link with the DLLs                   *
// * Copyright (c) 2012 Evidence Srl                         *
// * Author: Dario Di Stefano, Evidence                      *
// ***********************************************************

// This script is used to unlink DLLs

if exists('roller_link_num')
	ulink(roller_link_num);
end
if exists('flexsim_link_num')
	ulink(flexsim_link_num);
end
if exists('nativeinteger_link_num')
	ulink(nativeinteger_link_num);
end
if exists('udp_link_num')
	ulink(udp_link_num);
end
if exists('mcp2200_link_num')
	ulink(mcp2200_link_num);
end
if exists('serial_gateway_link_num')
	ulink(serial_gateway_link_num);
end
if exists('rs232_link_num')
	ulink(rs232_link_num);
end
if exists('libsmcube_link_num')
	ulink(libsmcube_link_num);
end
if exists('libsprintf_link_num')
	ulink(libsprintf_link_num);
end
