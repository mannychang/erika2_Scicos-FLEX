// ***********************************************************
// * ScicosLab Pack Installer                                *
// * Requires: Scicoslab v4.4.1                              *
// * Built/Tested On: XP, Windows 7                          *
// * Description: C30 compiler path detecting script         *
// * Copyright (c) 2012 Evidence Srl                         *
// * Author: Dario Di Stefano, Evidence                      *
// ***********************************************************

// Set useful variables
SCIDIR = strsubst(SCI,'/','\');
c30path_err = 0;

// °°° Step 1: Search in the common_oil.pref, if the file exists...
[x_x_x,pref_err] = fileinfo(SCIDIR + '\contrib\scicos_ee\RT-Druid\configuration\common_oil.pref');
if pref_err == 0
	c30path_conf = mgetl(SCIDIR + '\contrib\scicos_ee\RT-Druid\configuration\common_oil.pref');
	if c30path_conf==[]
		c30path_err = 1;
		else
		[c30path_row,c30path_col]=grep(c30path_conf,'gcc_compiler');
		if c30path_row == []
			c30path_err = 1;
		else
			c30path_row = c30path_row(1);
			col_index = strindex(c30path_conf(c30path_row),':/');
			c30_asm30_paths = part(c30path_conf(c30path_row),[(col_index-1):length(c30path_conf(c30path_row))]);
			c30_asm30_paths = strsubst(c30_asm30_paths,'/','\');
		end
	end
else

// °°° Step 2: Search in the PATH
	path_var = getenv('PATH','');
	c30_bin_index = strindex(path_var,'mplabc30');
	if c30_bin_index == []
		c30_bin_index = strindex(path_var,'MPLAB C30');
	end
	if c30_bin_index ~= []
		c30_bin_index = c30_bin_index(1);
		semic_index = strindex(path_var,';');
		for icnt = 1:length(semic_index)
			if semic_index(icnt) > c30_bin_index
				icnt = icnt-1;
				break;
			end
		end
		if semic_index == []
			semic_index = 0;
			icnt = 1;
		end
		if icnt == length(semic_index)
			c30_asm30_paths = part(path_var,[semic_index(icnt)+1:length(path_var)+4]);
		else
			c30_asm30_paths = part(path_var,[semic_index(icnt)+1:semic_index(icnt+1)-1-4]);
		end
	else
		c30path_err = 1;
	end
end

// °°° Step 3: Search in the %PROGRAMFILES% path
if c30path_err > 0
	programfiles = getenv("PROGRAMFILES","");
	mplabc30 = '\Microchip\mplabc30';
	res_mchp = isdir(programfiles + mplabc30);
	if res_mchp == %T
		ls_mchp = ls(programfiles + mplabc30);
		if ls_mchp ~= []
			[x_x_x,mchp_err] = fileinfo(programfiles + mplabc30 + '\' + ls_mchp(1) + '\bin\pic30-gcc.exe');
			if mchp_err == 0
				c30_asm30_paths = programfiles + mplabc30 + '\' + ls_mchp(1);
				c30path_err = 0;
			end
		end
	else
		mplabc30 = '\Microchip\MPLAB C30';
		[x_x_x,mchp_err] = fileinfo(programfiles + mplabc30 + '\bin\pic30-gcc.exe');
		if mchp_err == 0
			c30_asm30_paths = programfiles + mplabc30;
			c30path_err = 0;
		end
	end
end

// °°° Step 4: Search failed, set a default path...
if c30path_err > 0
	c30_asm30_paths = 'C:\Programmi\Microchip\MPLAB C30';
end

