// ***********************************************************
// * ScicosLab Pack Installer                                *
// * Requires: Scicoslab v4.4.1                              *
// * Built/Tested On: XP, Windows 7                          *
// * Description: Visual C++ 2008 9.0 detecting script       *
// * Copyright (c) 2012 Evidence Srl                         *
// * Author: Dario Di Stefano, Evidence                      *
// ***********************************************************

// Set useful variables
SCIDIR = strsubst(SCI,'/','\');
vcpath_err = 0;

// °°° Step 1: Search in the configuration.ini, if the file exists...
[x_x_x,ini_err] = fileinfo(SCIDIR + '\contrib\scicos_ee\bin\configuration.ini');
if ini_err == 0
	vcpath_conf = mgetl(SCIDIR + '\contrib\scicos_ee\bin\configuration.ini');
	if vcpath_conf==[]
		vcpath_err = 1;
		else
		[vcpath_row,vcpath_col]=grep(vcpath_conf,'VC\bin');
		if vcpath_row == []
			vcpath_err = 1;
		else
			vcpath_row = vcpath_row(1);
			vc_index = strindex(vcpath_conf(vcpath_row),'VC\bin');
			col_index = strindex(vcpath_conf(vcpath_row),':\');
			visualc_path = part(vcpath_conf(vcpath_row),[(col_index-1):(vc_index+1)]);
		end
	end
else

// °°° Step 2: Search in the PATH
	path_var = getenv('PATH','');
	vc_bin_index = strindex(path_var,'VC\bin');
	if vc_bin_index ~= []
		vc_bin_index = vc_bin_index(1);
		semic_index = strindex(path_var,';');
		for icnt = 1:length(semic_index)
			if semic_index(icnt) > vc_bin_index
				icnt = icnt-1;
				break;
			end
		end
		if semic_index == []
			semic_index = 0;
			icnt = 1;
		end
		if (semic_index(icnt)+1) < (vc_bin_index+1)
			visualc_path = part(path_var,[semic_index(icnt)+1:(vc_bin_index+1)]);
		else
			vcpath_err = 1;
		end
	else
		vcpath_err = 1;
	end
end

// °°° Step 3: Search failed, set a default path...
if vcpath_err > 0
	visualc_path = 'C:\Programmi\Microsoft Visual Studio 9.0\VC';
end
