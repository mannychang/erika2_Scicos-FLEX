// ***********************************************************
// * ScicosLab Pack Installer                                *
// * Requires: Scicoslab v4.4.1                              *
// * Built/Tested On: XP, Windows 7                          *
// * Description: Convert .COS to .COSF                      *
// * Copyright (c) 2011 Evidence Srl                         *
// * Author: Dario Di Stefano, Evidence                      *
// ***********************************************************

// Load scicos library
lib(SCI + '/macros/scicos/');
// Load the cos2cosf function
x = cos2cosf;

// example
	//load('prova_cos2cosf.cos'); // load the diagram
	//file_name = 'pippo2.cosf'; // Open a file
	//[u,err] = file('open',file_name,'unknown','formatted'); // open the detination file
	//if err<>0 then
	//	disp("File or directory write access denied");
	//	return
	//end
	//cos2cosf(u,scs_m); // Call the conversion function
	//file('close',u);   // close the file
	//return

// Set the reference path
// SCIDIR = strsubst(SCI,'/','\');
MYDIR = 'C:\Dario\EE_svn\repos\scilab_codegen\trunk\testcase';
MYDIR = x_dialog(['Enter directory path for COS files search:'],[MYDIR]);
if MYDIR == []
    return;
end
cd(MYDIR);

// Create a list for the cos files
if ~MSDOS then // Unix Linux
    disp('linux')
    unix('find . -type f -name *.cos > x.x');
else
    disp('win32')
    unix('dir /S /B *.cos > x.x');
end

cos_list = mgetl('x.x');
mdelete x.x;
cos_list = x_dialog(['Select the files to be converted:'],[cos_list]);
if cos_list == []
    return;
end

// Create the .cosf files
[numr,numl] = size(cos_list);
clear scs_m
for j = 1:1:numr,
    [x,ierr] = fileinfo( cos_list(j) );
    if(ierr <> 0)
        disp('Warning: File ' + cos_list(j) + ' does not exist!');
        continue;
    end
    if( grep(cos_list(j),'cosf') ~= [] )
        continue;
    end
    disp('Converting file ' + cos_list(j));
    load(cos_list(j));
    [fd,err] = file('open',cos_list(j)+'f','unknown','formatted'); // open the detination file
    if err ~= 0
        disp('COS2COSF CONVERTER ERROR IN FILE: ' + cos_list(j));
        break;
    end
    cos2cosf(fd, scs_m); // Call the conversion function
    file('close', fd);   // close the file
    clear scs_m
end












