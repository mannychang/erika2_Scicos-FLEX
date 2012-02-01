// ***********************************************************
// * ScicosLab Pack Installer                                *
// * Requires: Scicoslab v4.4.1                              *
// * Built/Tested On: XP, Windows 7                          *
// * Description: Installation script of the Code generator  *
// *              for FLEX and EASYLAB boards                *
// * Copyright (c) 2011 Evidence Srl                         *
// * Author: Dario Di Stefano, Evidence                      *
// ***********************************************************

// Set global variables
EE_DEBUG = 1;
MYDIR = get_absolute_file_path('installer.sce');
OLDDIR = pwd();
SCIDIR = strsubst(SCI,'/','\');

if exists('INSTALLER_BATCH_MODE')==0 & exists('ans_inst_dialog')==0
	ans_inst_dialog = buttondialog("ScicosLab pack Setup utility.\nChoose if install or uninstall the toolbox. ","Install|Uninstall|Exit","question"); 
	ans_inst_dialog_up = 1;
else
	if exists('ans_inst_dialog')==0
		ans_inst_dialog='1'; // Install by default...
	end
end

if ans_inst_dialog=='3' | ans_inst_dialog=='0'
  cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
end

// *********************************************************************************
// ************************* SCICOS_PACK UNINSTALLER *******************************
if ans_inst_dialog=='2'
  // Uninstaller section: exec('unistaller.sce');
  // TEST if user is Admin
  cd(SCIDIR + '\contrib');
winId_wait =waitbar("                                                        " + "\n" + ..
					"                  EE Scicos pack Setup.                 " + "\n" + ..
					"        Uninstallation may take several seconds...      " + "\n" + ..
					"                                                        " + "\n" + ..
					"                                                        " + "\n" + ..
					"                                                        " );
  cmd = 'dir > test.x';
  unix(cmd);
  [x,err] = fileinfo('test.x');
  if err ~= 0
waitbar("                                                        " + "\n" + ..
		"                 # Uninstallation Error #               " + "\n" + ..
		"   Access denied! Is not possible to delete the files.  " + "\n" + ..
		"   Please, run ScicosLab with administrator privileges. " + "\n" + ..
		"                  Uninstallation aborted!               " + "\n" + ..
		"                                                        ", winId_wait);
    cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
  else
    cmd = 'del /Q test.x';
    unix(cmd);
  end

  // Change working directory
  cd(MYDIR);
  // Load libraries
  getf scicos_ee\utils\utils.sci 
  // Check .scilab file presence
  res = grep(SCIHOME,'4.4.1');
  if res==[]
    cd(MYDIR+'scicos_ee\user\Scilab\4.4b7'); // 4.4b7
  else
    cd(MYDIR+'scicos_ee\user\ScicosLab\4.4.1'); // 4.4.1
  end
  [x,ierr]=fileinfo(SCIHOME+'\.scilab');
  if ierr==0
    txt=mgetl(SCIHOME+'\.scilab');
    res_begin = grep(txt,'### Scicos EE ###');
    res_end = grep(txt,'### Scicos EE - end ###');
    if res_begin==[]
      EE_debug_printf('  #warning: .scilab is already cleaned!', 0);
    elseif res_end==[]
      EE_debug_printf('  #error: .scilab file is corrupted!', 0);
waitbar("                                                        " + "\n" + ..
		"                 # Uninstallation Error #               " + "\n" + ..
		"                .scilab file is corrupted!              " + "\n" + ..
		"    (Please, delete or modify the file before to retry) " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
    cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
    else
      // To change '.scilab' file user should not be Admin
      res = size(txt);
      new_txt = [txt(1:res_begin-1);txt(res_end+1:res(1))];
      mputl(new_txt, SCIHOME+'\.scilab');
      EE_debug_printf('  #warning: .scilab was successfully restored!', 0);
    end
  else
    EE_debug_printf('  #warning: .scilab file not found!.', 0);
  end
  // 50%
  waitbar(0.5, winId_wait);
  // Delete scicos_ee folder from contrib
waitbar("                                                        " + "\n" + ..
		"                      Deleting files...                 " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
  EE_debug_printf('  Deleting files...', 0); 
  
  // Unlink DLLs
  exec(MYDIR + 'scicos_ee\utils\unlink_dll.sce');
  
  cd(SCIDIR + '\contrib');
  // Removing ScicosLabPack_install.log
  [loginfo,ierr]=fileinfo('ScicosLabPack_install.log');
  if ierr==0
    cmd = 'del /Q ScicosLabPack_install.log';
    unix(cmd);
  end
  // Removing scicos_ee
  res = isdir(SCIDIR+'\contrib\scicos_ee');
  if res==%T
    cmd = 'rmdir /s /q scicos_ee';
    unix(cmd);
  end
  // 100%
  waitbar(1, winId_wait);
waitbar("                                                        " + "\n" + ..
		"          Uninstallation completed successfully!        " + "\n" + ..
		"   Restart ScicosLab for the changes to take effect...  " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
  EE_debug_printf('  Uninstallation completed successfully!', 0); 
  EE_debug_printf('  Please, restart ScicosLab for the changes to take effect...', 0);
  cd(home); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
end
// *********************************************************************************
// *************************** SCICOS_PACK INSTALLER *******************************
if ans_inst_dialog=='1'
  // Initial message
winId_wait =waitbar("                                                        " + "\n" + ..
					"                 EE Scicos pack Setup.                  " + "\n" + ..
					"          Installation may take several seconds...      " + "\n" + ..
					"                                                        " + "\n" + ..
					"                                                        " + "\n" + ..
					"                                                        ");
  // Change working directory
  cd(MYDIR);
  // Load libraries
  getf scicos_ee\utils\utils.sci 
else
  cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return; // added only for completeness...
end

// Installation test
cd(SCIDIR + '\contrib');
cmd = 'dir > test.x';
unix(cmd);
[x,err] = fileinfo('test.x');
if err ~= 0
waitbar("                                                        " + "\n" + ..
		"                   # Installation Error #               " + "\n" + ..
		"                       Access denied!                   " + "\n" + ..
		"   Please, run ScicosLab with administrator privileges. " + "\n" + ..
		"                      Installation aborted!             " + "\n" + ..
		"                                                        ", winId_wait);
    cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
else
  cmd = 'del /Q test.x';
  unix(cmd);
end
cd(MYDIR);
 // Installation log
[fd_log, err] = mopen(SCIDIR+'\contrib\ScicosLabPack_install.log', 'w');
if err ~= 0
waitbar("                                                        " + "\n" + ..
		"                 # Installation Error #                 " + "\n" + ..
		" Access denied! Is not possible to create the log file. " + "\n" + ..
		"  Please, run ScicosLab with administrator privileges.  " + "\n" + ..
		"                 Installation aborted!                  " + "\n" + ..
		"                                                        ", winId_wait);
  cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
else
  EE_debug_printf('### EE Scicoslab pack installer ###', fd_log);
end
  
// 10%
waitbar(0.1, winId_wait);

if exists('NO_APPS_CHECK')==0

	// --- Check Cygwin presence ---
	waitbar("                                                        " + "\n" + ..
			"                 Check Cygwin presence...               " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        ", winId_wait);
	[x,ierr]=fileinfo('C:\cygwin\bin\bash.exe');
	if ierr==0
	waitbar("                                                        " + "\n" + ..
			"              Check Cygwin presence...found!            " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        ", winId_wait);
	else
	waitbar("                                                        " + "\n" + ..
			"                  # Installation Warning #              " + "\n" + ..
			"                    Cygwin not found!                   " + "\n" + ..
			"              Cygwin should be installed in C:\         " + "\n" + ..
			"           if you plan to generate code for dsPIC.      " + "\n" + ..
			"                                                        ", winId_wait);
	  EE_debug_printf('  #Warning Cygwin not found!', fd_log);
	end

	// --- Create apps.list (installed applications list) ---
	cd(MYDIR+'scicos_ee\utils');
	cmd = 'installed_win_apps_list.bat > apps.list';
	unix(cmd);
	cd(MYDIR+'scicos_ee\utils');
	txt=mgetl('apps.list');

	// 20 %
	waitbar(0.2, winId_wait);

	// Check Visual C++ presence
	waitbar("                                                        " + "\n" + ..
			"           Check Visual C++ 2008 presence...            " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        ", winId_wait);
	res = grep(txt,'Visual C++ 2008');
	if res==[]
		waitbar("                                                        " + "\n" + ..
				"                # Installation Error #                  " + "\n" + ..
				"                 Visual C++ not found!                  " + "\n" + ..
				"    Installation of Visual C++ 2008 Express Edition     " + "\n" + ..
				"        is required by the ScicosLab EE pack!           " + "\n" + ..
				"                                                        ", winId_wait);
			EE_debug_printf('  #error: Visual C++ 2008 not found!', fd_log);
			EE_debug_printf('  #error: Installation of Visual C++ 2008 Express Edition is required by the ScicosLab EE pack!', fd_log);
			EE_debug_printf('  Installation aborted!', fd_log);
			EE_debug_printf('### END ###', fd_log);
			mclose(fd_log);
			cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
	end

	// Check Java presence
	waitbar("                                                        " + "\n" + ..
			"               Check Java presence...                   " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        ", winId_wait);
	res = grep(txt,'Java(TM)');
	if res==[]
	  EE_debug_printf('  #warning: Java not found!', fd_log);
	  EE_debug_printf('  #warning: Installation of Java is required by the EE-ScicosLab code generator!', fd_log);
	waitbar("                                                        " + "\n" + ..
			"              # Installation Warning #                  " + "\n" + ..
			"                  Java not found!                       " + "\n" + ..
			"  Java is required by the EE-ScicosLab code generator!  " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        ", winId_wait);
	end
end

	waitbar("                                                        " + "\n" + ..
		"           Check Visual C++ 2008 settings...            " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);

VC_path_valid = %F;
if exists('visualc_path')==0
	[x_x_x,vcpp_detect_err] = fileinfo(MYDIR + 'scicos_ee\utils\vcpp_detect.sce');
	if vcpp_detect_err == 0
		exec(MYDIR + 'scicos_ee\utils\vcpp_detect.sce');
	else
		visualc_path = 'C:\Programmi\Microsoft Visual Studio 9.0\VC';
	end
end

while VC_path_valid==%F
	if exists('INSTALLER_BATCH_MODE')==0 // in batch mode do not display the dialog window
		visualc_path = x_dialog(['Enter Visual C++ directory path'],[visualc_path]);
		visualc_path_up = 1;
	else
		if exists('INSTALLER_BATCH_MODE')==1
			EE_debug_printf(' #warning: Installation in batch mode is using the default Visual C++ path!', fd_log);
		end
	end

	if visualc_path == []
			waitbar("                                                        " + "\n" + ..
					"                  # Installation Error #                " + "\n" + ..
					"        Please, install a valid Visual C++ compiler     " + "\n" + ..
					"         otherwise code generator will not work!        " + "\n" + ..
					"                                                        " + "\n" + ..
					"                  Installation aborted!                 ", winId_wait);
			EE_debug_printf('  Installation aborted!', fd_log);
			EE_debug_printf('### END ###', fd_log);
			mclose(fd_log);
			cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
	end
	visualc_path = visualc_path(1); // to avoid problems in case of multi-string array.
	visualc_path = strsubst(visualc_path,'/','\');
	ind_slash = strindex(visualc_path,'\');
	len_path = length(visualc_path);
	if ind_slash(length(ind_slash)) ==  len_path
		strs_temp = strsplit(visualc_path,[ind_slash(length(ind_slash))-1, ind_slash(length(ind_slash))-1]);
		visualc_path = strs_temp(1);
	end
	[x,ierr]=fileinfo(visualc_path + '\bin\cl.exe');  
	if ierr<0 // path is wrong!!!
		VC_path_valid = %F;
		if exists('INSTALLER_BATCH_MODE')==0 & exists('vcansw_retry')==0
			vcansw_retry = buttondialog("Inserted path is not valid! (Yes: to retry, No: to abort)","yes|no","question"); 
			vcansw_retry_up = 1;
		else
			if exists('vcansw_retry')==0
				vcansw_retry = '2'; // error in case of batch_mode - installer...
			end
		end
		if vcansw_retry=='2'
			waitbar("                                                        " + "\n" + ..
					"                  # Installation Error #                " + "\n" + ..
					"        Please, install a valid Visual C++ compiler     " + "\n" + ..
					"         otherwise code generator will not work!        " + "\n" + ..
					"               path + /bin/cl.exe NOT FOUND!            " + "\n" + ..
					"                  Installation aborted!                 ", winId_wait);
			EE_debug_printf('  Installation aborted!', fd_log);
			EE_debug_printf('### END ###', fd_log);
			mclose(fd_log);
			cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
		end
		if vcansw_retry == '0'
			VC_path_valid = %T; // ignore wrong paths...
		end
	else
		VC_path_valid = %T; // path is OK...
	end
	if exists('vcansw_retry_up')==1
		clear vcansw_retry vcansw_retry_up
	end
end

[x,err] = fileinfo(MYDIR + "scicos_ee\bin\SMCube.exe");
if err == 0
	// CONFIGURATION.INI file is created 
	[fd,err] = mopen(MYDIR+'scicos_ee\utils\configuration.ini', 'w');
	
	if err ~= 0
		waitbar("                                                        " + "\n" + ..
				"                 # Installation Error #                 " + "\n" + ..
				" Access denied! Is not possible to create a pref file!  " + "\n" + ..
				"   Please, run ScicosLab with administrator privileges. " + "\n" + ..
				"                 Installation aborted!                  " + "\n" + ..
				"                                                        ", winId_wait);
		EE_debug_printf('  #error: Access denied! Is not possible to create a configuration file!', fd_log);
		EE_debug_printf('  #error: Please, run ScicosLab with administrator privileges to install the toolbox', fd_log);
		EE_debug_printf('  Installation aborted!', fd_log);
		EE_debug_printf('### END ###', fd_log);
		mclose(fd_log);
		cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
	end

	mfprintf(fd,"##Compiler path, including the executable file name\n");
	mfprintf(fd,"Compiler=");
	mfprintf(fd, strsubst(visualc_path,'\','\\'));
	mfprintf(fd,"\\bin\\cl.exe\n");
	mclose(fd);
	waitbar("                                                        " + "\n" + ..
			"  Creating preferences file configuration.ini ... Done! " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        ", winId_wait);
end

	waitbar("                                                        " + "\n" + ..
			"                C30 compiler settings:                  " + "\n" + ..
			" - Recommended version: v3.30 or later                  " + "\n" + ..
			" - Bad version: v3.25, sprintf_block cannot be compiled!" + "\n" + ..
			" For further info visit: http://www.microchip.com/      " + "\n" + ..
			"                                                        ", winId_wait);

	if exists('INSTALLER_BATCH_MODE')==0 & exists('answ_c30')==0
		answ_c30 = buttondialog("Code Generation for dsPIC DSC microcontroller requires a valid path for the Microchip C30 Compiler.\n" + ..
                                "Please answer yes if you plan to generate code for the target.\n" + ..
                                "If you are only installing the tool for simulation purposes, and not for code generation, you can safely answer No.","yes|no","question"); 
		answ_c30_up = 1;
		if answ_c30=='2' | answ_c30=='0'
			waitbar("                                                        " + "\n" + ..
					"              # Installation Warning #                  " + "\n" + ..
					"             C30 compiler not specified                 " + "\n" + ..
					"            Code generator will not work!               " + "\n" + ..
					"                                                        " + "\n" + ..
					"                                                        ", winId_wait);
			EE_debug_printf('  #warning: If C30 compiler is not specified, code generator is disabled!', fd_log);
		end
	else
		if exists('answ_c30')==0
			answ_c30 = '1';
		end
	end

// C30 path
CC_path_valid=%F; // path is non valid at the moment...
if exists('c30_asm30_paths')==0
	[x_x_x,c30_detect_err] = fileinfo(MYDIR + 'scicos_ee\utils\c30_detect.sce');
	if c30_detect_err == 0
		exec(MYDIR + 'scicos_ee\utils\c30_detect.sce');
	else
		c30_asm30_paths = 'C:\Programmi\Microchip\MPLAB C30';
	end
end
while CC_path_valid==%F
	  if exists('INSTALLER_BATCH_MODE')==0 & answ_c30=='1' // in batch mode do not display the dialog window
		c30_asm30_paths = x_dialog(['Enter C30 installation directory path';..
		                            '- Recommended version: v3.30 or later';..
                                    '- Bad version: v3.25, sprintf_block cannot be compiled!';..
                                    'For further info visit: http://www.microchip.com/'],[c30_asm30_paths]);
		c30_asm30_paths_up = 1;
	  else
			if exists('INSTALLER_BATCH_MODE')==1
				EE_debug_printf(' #warning: Installation in batch mode is using the default C30 path!', fd_log);
			end
	  end
	  if c30_asm30_paths==[]
			[x_x_x,c30_detect_err] = fileinfo(MYDIR + 'scicos_ee\utils\c30_detect.sce');
			if c30_detect_err == 0
				exec(MYDIR + 'scicos_ee\utils\c30_detect.sce');
			else
				c30_asm30_paths = 'C:\Programmi\Microchip\MPLAB C30';
			end
			answ_c30 = buttondialog("Code Generation for dsPIC DSC microcontroller requires a valid path for the Microchip C30 Compiler.\n" + ..
                                    "Please answer yes if you plan to generate code for the target.\n" + ..
                                    "If you are only installing the tool for simulation purposes, and not for code generation, you can safely answer No.","yes|no","question"); 
			answ_c30_up = 1;
			if answ_c30=='2' | answ_c30=='0'
				waitbar("                                                        " + "\n" + ..
						"              # Installation Warning #                  " + "\n" + ..
						"             C30 compiler not specified                 " + "\n" + ..
						"            Code generator will not work!               " + "\n" + ..
						"                                                        " + "\n" + ..
						"                                                        ", winId_wait);
				EE_debug_printf('  #warning: If C30 compiler is not specified, code generator is disabled!', fd_log);
			else
				continue;
			end
	  end
	  c30_asm30_paths = c30_asm30_paths(1); // to avoid problems in case of multi-string array.
	  c30_asm30_paths = strsubst(c30_asm30_paths,'\','/');
	  ind_slash = strindex(c30_asm30_paths,'/');
	  len_path = length(c30_asm30_paths);
	  if ind_slash(length(ind_slash)) ==  len_path
		strs_temp = strsplit(c30_asm30_paths,[ind_slash(length(ind_slash))-1, ind_slash(length(ind_slash))-1]);
		c30_asm30_paths = strs_temp(1);
	  end
	  [x,ierr]=fileinfo(c30_asm30_paths + '/bin/pic30-gcc.exe');  
	  if ierr<0 & answ_c30=='1' // path is wrong!!!
			CC_path_valid = %F;
			if exists('INSTALLER_BATCH_MODE')==0 & exists('answ_retry')==0
				answ_retry = buttondialog("Inserted path is not valid! (Yes: to retry, No: to abort)","yes|no","question"); 
				answ_retry_up = 1;
			else
				if exists('answ_retry')==0
					answ_retry = '2'; // error in case of batch_mode - installer...
				end
			end
			if answ_retry=='2'
			waitbar("                                                        " + "\n" + ..
					"               # Installation Error #                   " + "\n" + ..
					"          Please, install a valid compiler              " + "\n" + ..
					"       otherwise code generator will not work!          " + "\n" + ..
					"        path + /bin/pic30-gcc.exe NOT FOUND!            " + "\n" + ..
					"               Installation aborted!                    ", winId_wait);
				EE_debug_printf('  Installation aborted!', fd_log);
				EE_debug_printf('### END ###', fd_log);
				mclose(fd_log);
				cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
			end
			if answ_retry=='0'
				CC_path_valid = %T; // ignore wrong paths...
			end
	  else
			CC_path_valid = %T; // path is OK...
	  end
	  if exists('answ_retry_up')==1
		clear answ_retry answ_retry_up
	  end
end

// COMMON_OIL.PREF file is created 
[fd,err] = mopen(MYDIR+'scicos_ee\utils\common_oil.pref', 'w');
if err ~= 0
	waitbar("                                                        " + "\n" + ..
			"                # Installation Error #                  " + "\n" + ..
			"  Access denied! Is not possible to create a pref file! " + "\n" + ..
			"  Please, run ScicosLab with administrator privileges.  " + "\n" + ..
			"                Installation aborted!                   " + "\n" + ..
			"                                                        ", winId_wait);
	EE_debug_printf('  #error: Access denied! Is not possible to create a preferences file!', fd_log);
	EE_debug_printf('  #error: Please, run ScicosLab with administrator privileges to install the toolbox', fd_log);
	EE_debug_printf('  Installation aborted!', fd_log);
	EE_debug_printf('### END ###', fd_log);
	mclose(fd_log);
	cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
end

mfprintf(fd,"# Path to the ASM30 Assembler\n");
mfprintf(fd,"# NOT the assembler distributed with the C30 Compiler!\n");
mfprintf(fd,"preference_pic30__path_for_asm_compiler = ");
//mfprintf(fd, strsubst(c30_asm30_paths,'\','\\\\') );
mfprintf(fd, c30_asm30_paths );
mfprintf(fd,"\n");
mfprintf(fd,"\n");
mfprintf(fd,"# Path to the C30 Assembler\n");
mfprintf(fd,"preference_pic30__path_for_gcc_compiler = ");
//mfprintf(fd, strsubst(c30_asm30_paths,'\','\\\\') );
mfprintf(fd, c30_asm30_paths );
mfprintf(fd,"\n");
mfprintf(fd,"\n");
mfprintf(fd,"# Erika Enterprise path\n");
mfprintf(fd,"erika_enterprise_location=${eclipse_home}/../ee\n");
mfprintf(fd,"\n");
mfprintf(fd,"\n");
mclose(fd);
waitbar("                                                        " + "\n" + ..
		"  Creating preferences file common_oil.pref ... Done!   " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);

// 30%
waitbar(0.3, winId_wait);

// Check the existence of scicos_ee folder in contrib
res = isdir(SCIDIR+'\contrib\scicos_ee');
if res==%T
waitbar("                                                        " + "\n" + ..
		"                # Installation Warning #                " + "\n" + ..
		"      An old version of the ScicosLab pack was found!   " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
  EE_debug_printf('  An old version of the ScicosLab pack was found!', fd_log);
  
  if exists('INSTALLER_BATCH_MODE')==0 & exists('ans_old_inst_rm')==0
	ans_old_inst_rm = buttondialog("The old version of the ScicosLab pack will be removed.\nDo you want to continue? (Yes: to continue, No: to abort)","yes|no","question");
	ans_old_inst_rm_up = 1;
  else
	if exists('ans_old_inst_rm')==0
		ans_old_inst_rm = '1'; // remove old pack by default.
	end
  end
  
  if ans_old_inst_rm=='1'
    // UNINSTALLER: Delete scicos_ee folder from contrib
waitbar("                                                        " + "\n" + ..
		"                     Deleting files...                  " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
    EE_debug_printf('  Deleting files...', fd_log); 

    // Unlink DLLs
    exec(MYDIR + 'scicos_ee\utils\unlink_dll.sce');

    // Removing scicos_ee
    cd(SCIDIR+'\contrib');
    cmd = 'rmdir /s /q scicos_ee';
    unix(cmd);

  else
    waitbar("                                                        " + "\n" + ..
			"            The installation was interrupted!           " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        " + "\n" + ..
			"                                                        ", winId_wait);
    EE_debug_printf('  The installation was interrupted!', fd_log);
    EE_debug_printf('### END ###', fd_log);
    mclose(fd_log);
    cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
  end
end

// Ask if the user wants to proceed...
if exists('INSTALLER_BATCH_MODE')==0 & exists('ans_inst_go')==0
	ans_inst_go = buttondialog("The ScicosLab pack will be installed.\nDo you want to continue? (Yes: to continue, No: to abort)","yes|no","question");
	ans_inst_go_up = 1;
else
	if exists('ans_inst_go')==0
		ans_inst_go = '1'; // continue with the installation by dafault...
	end
end

if ans_inst_go=='1'
waitbar("                                                        " + "\n" + ..
		"        The ScicosLab pack for FLEX and Easylab         " + "\n" + ..
		"                 will be installed...                   " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
  cd(SCIDIR+'\contrib');
  EE_debug_printf('  EE Scicos pack will be installed...', fd_log);
  cmd = 'mkdir scicos_ee';
  unix(cmd);
  cmd = 'start xcopy '+ascii(34)+MYDIR+'scicos_ee'+ascii(34)+' '+ascii(34)+SCIDIR+'\contrib\scicos_ee'+ascii(34)+' /s /e /y /i';
  unix(cmd);
  
  else
waitbar("                                                        " + "\n" + ..
		"            The installation was interrupted!           " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
  EE_debug_printf('  The installation was interrupted!', fd_log);
  EE_debug_printf('### END ###', fd_log);
  mclose(fd_log);
  cd(home); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
  end

// 40%
waitbar(0.4, winId_wait);

// Check copy is finished
EE_debug_printf('  ...copying Scicos EE files...', fd_log); 
EE_debug_printf('  Please, do not close the MSDOS window.', fd_log); 
EE_debug_printf('  It will be closed automatically at the end of copy.', fd_log); 
waitbar("                                                        " + "\n" + ..
		"                      Copying files...                  " + "\n" + ..
		"             (Copy may take several seconds!)           " + "\n" + ..
		"          Please, do not close the MSDOS window.        " + "\n" + ..
		"   It will be closed automatically at the end of copy.  " + "\n" + ..
		"                                                        ", winId_wait);
realtimeinit(1); 
realtime(0);
// winId_prog=progressionbar('EE Scicos pack. Copying files...');
res_examples = %F;
res_RTD_conf = %F;
res_RTD_p2 = %F;
res_RTD_plugins = %F;
res_flex_dspic = %F;
res_flex_gw = %F;
res_flex_msvc = %F;
res_flex_rttemp = %F;
res_user = %F;
res_scicosee = isdir(SCIDIR+'\contrib\scicos_ee');
res_utils = isdir(SCIDIR+'\contrib\scicos_ee\utils');
i = 0;

if res_scicosee==%F
waitbar("                                                        " + "\n" + ..
		"                 # Installation Error #                 " + "\n" + ..
		" Access denied! Is not possible to install the toolbox! " + "\n" + ..
		"   Please, run ScicosLab with administrator privileges. " + "\n" + ..
		"                 Installation aborted!                  " + "\n" + ..
		"                                                        ", winId_wait);
  EE_debug_printf('  #error: Access denied! Is not possible to create the directories!', fd_log);
  EE_debug_printf('  #error: Please, run ScicosLab with administrator privileges to install the toolbox', fd_log);
  EE_debug_printf('  Installation aborted!', fd_log);
  // winclose(winId_prog);
  EE_debug_printf('### END ###', fd_log);
  mclose(fd_log);
  cd(home); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
end

while res_utils==%F,
  res = 0.4;
  res_examples = isdir(SCIDIR+'\contrib\scicos_ee\examples');
  res = res + res_examples*0.02;
  res_RTD_conf = isdir(SCIDIR+'\contrib\scicos_ee\RT-Druid\configuration');
  res = res + res_RTD_conf*0.02;
  res_RTD_p2 = isdir(SCIDIR+'\contrib\scicos_ee\RT-Druid\p2');
  res = res + res_RTD_p2*0.04;
  res_RTD_plugins = isdir(SCIDIR+'\contrib\scicos_ee\RT-Druid\plugins');
  res = res + res_RTD_plugins*0.02;
  res_flex_dspic = isdir(SCIDIR+'\contrib\scicos_ee\scicos_flex\dspic');
  res = res + res_flex_dspic*0.2;
  res_flex_gw = isdir(SCIDIR+'\contrib\scicos_ee\scicos_flex\flex_usb2udp_gateway');
  res = res + res_flex_gw*0.05;
  res_flex_rttemp = isdir(SCIDIR+'\contrib\scicos_ee\scicos_flex\RT_templates');
  res = res + res_flex_rttemp*0.02;
  res_user = isdir(SCIDIR+'\contrib\scicos_ee\user');
  res = res + res_user*0.04;
  res_utils = isdir(SCIDIR+'\contrib\scicos_ee\utils');
  res = res + res_utils*0.02;
  
  waitbar(res, winId_wait); // to increment the bar...
  
  i = i+1;
  realtime(i);
  // progressionbar(winId_prog);
  
end
// winclose(winId_prog);

// 90%
waitbar(0.9, winId_wait);

// Check .scilab file presence
EE_debug_printf('  ', fd_log); 
res = grep(SCIHOME,'4.4.1');
if res==[]
  cd(MYDIR+'scicos_ee\user\Scilab\4.4b7'); // 4.4b7
else
  cd(MYDIR+'scicos_ee\user\ScicosLab\4.4.1'); // 4.4.1
end

[x,ierr]=fileinfo(SCIHOME+'\.scilab');
if ierr==0
	waitbar("                                                        " + "\n" + ..
			"               # Installation Warning #                 " + "\n" + ..
			"    The installation should modify the .scilab script   " + "\n" + ..
			"         Please, if the .scilab file is open,           " + "\n" + ..
			"             close it before proceeding...              " + "\n" + ..
			"                                                        ", winId_wait);
	EE_debug_printf('  #warning: Please, if the .scilab file is open, close it before proceeding...', fd_log);
	if exists('INSTALLER_BATCH_MODE')==0 & exists('answ_dot_scilab')==0
		answ_dot_scilab = buttondialog("The installation should modify the .scilab script (Yes: to continue, No: to abort)","yes|no","question");
		answ_dot_scilab_up = 1;
	else
		if exists('answ_dot_scilab')==0
			answ_dot_scilab = '1'; // allow changes in .scilab by default...
		end
	end
	// EE_debug_printf('  #warning: .scilab file found!', fd_log);
	txt=mgetl(SCIHOME+'\.scilab');
else
	// EE_debug_printf('  #warning: .scilab file not found!. The file will be created.', fd_log);
	txt=[];
	if exists('answ_dot_scilab')==0
		answ_dot_scilab = '1'; // allow changes in .scilab by default...
	end
end

if answ_dot_scilab=='1'
	res = grep(txt,'### Scicos EE ###');
	if res ~= [] // the string was found...
		// UNINSTALLER: .scilab will be cleaned
		// EE_debug_printf('  #warning: .scilab should be cleaned!', 0);
		res_begin = grep(txt,'### Scicos EE ###');
		res_end = grep(txt,'### Scicos EE - end ###');
		if res_end==[]
			waitbar("                                                        " + "\n" + ..
					"                # Installation Error #                  " + "\n" + ..
					"              .scilab file is corrupted!                " + "\n" + ..
					"       Please, delete the .scilab file and retry...     " + "\n" + ..
					"                Installation aborted!                   " + "\n" + ..
					"                                                        ", winId_wait);
			EE_debug_printf('  #error: .scilab file is corrupted!', fd_log);
			EE_debug_printf('  Installation aborted!', fd_log);
			EE_debug_printf('### END ###', fd_log);
			mclose(fd_log);
			cd(home); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
		else
			// EE_debug_printf('  #warning: .scilab was successfully restored!', 0);
			res = size(txt);
			new_txt = [txt(1:res_begin-1);txt(res_end+1:res(1))];
			mputl(new_txt, SCIHOME+'\.scilab');
		end
	end

	[fd,err]=mopen(SCIHOME+'\.scilab', 'a');
	txt=mgetl('.scilab');
	res = size(txt);
	mfprintf(fd,"\n");
	mfprintf(fd,"\n");
	mfprintf(fd,"// ### Scicos EE ###\n");
	mfprintf(fd,"\n");
	for i=1:res(1),
	  mfprintf(fd,"%s\n",txt(i));
	end
	mfprintf(fd,"\n");
	mfprintf(fd,"// ### Scicos EE - end ###\n");
	mfprintf(fd,"\n");
	mclose(fd);
end

// 95%
waitbar(0.95, winId_wait);

// Build Scicos EE pack
waitbar("                                                        " + "\n" + ..
		"                  Building ScicosLab EE pack...         " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);

clc
EE_debug_printf('  Building ScicosLab EE pack...', fd_log);
cd(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic");
exec builder.sce

// Create and move Scicos EE pack palettes
waitbar("                                                        " + "\n" + ..
		"           Creating ScicosLab Pack palettes...          " + "\n" + ..
		"     (Depending on your installation, now ScicosLab     " + "\n" + ..
		"       can generate many warnings, do not worry...)     " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
EE_debug_printf('  Creating ScicosLab EE palettes...', fd_log);
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\AMAZING");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-Communication");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-DMB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-MTB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\RT-Data-Exchange");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\EASYLAB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\MISC");
// SMCube palette
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\SMCube");

// Copy the palettes in the directory
cd(MYDIR+'scicos_ee\utils');
cmd = 'start cp_cosf.bat ' + ascii(34)+SCIDIR+ascii(34);
unix(cmd);
cmd = 'start xcopy '+ascii(34)+SCIDIR+'\contrib\scicos_ee\utils\common_oil.pref'+ascii(34)+' '+ascii(34)+SCIDIR+'\contrib\scicos_ee\RT-Druid\configuration'+ascii(34)+' /s /e /y /i';
unix(cmd);
[x,err] = fileinfo(SCIDIR + "\contrib\scicos_ee\utils\configuration.ini");
if err == 0
	cmd = 'start xcopy '+ascii(34)+SCIDIR+'\contrib\scicos_ee\utils\configuration.ini'+ascii(34)+' '+ascii(34)+SCIDIR+'\contrib\scicos_ee\bin'+ascii(34)+' /s /e /y /i';
	unix(cmd);
end

// Clear some unnecessary files..
if exists('NO_APPS_CHECK')==0
	cmd = 'del apps.list';
	unix(cmd);
end
[x,err] = fileinfo('common_oil.pref');
if err == 0
	cmd = 'del common_oil.pref';
	unix(cmd);
end

// 99%
waitbar(0.99, winId_wait);

if exists('INSTALLER_BATCH_MODE')==0
waitbar("                                                        " + "\n" + ..
		"             Creating ScicosLab Pack Help...            " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
	clc
	EE_debug_printf('  Building ScicosLab EE pack Help...', fd_log);
	cd(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\man");
	exec builder.sce
end

// End
// 100%
waitbar(1, winId_wait);
waitbar("                                                        " + "\n" + ..
		"           Installation completed successfully!         " + "\n" + ..
		"        Please, restart ScicosLab for the changes       " + "\n" + ..
		"                   to take effect...                    " + "\n" + ..
		"                                                        " + "\n" + ..
		"                                                        ", winId_wait);
EE_debug_printf('  Installation completed successfully!', fd_log); 
EE_debug_printf('  Please, restart ScicosLab for the changes to take effect...', fd_log);
EE_debug_printf('### END ###', fd_log);
mclose(fd_log);
cd(OLDDIR); exec(MYDIR+'scicos_ee\utils\clear_inst_vars.sce'); return;
