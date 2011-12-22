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


if exists('answ_install_dialog')==0
	answ_install_dialog = buttondialog("ScicosLab pack Setup utility.\nChoose if install or uninstall the toolbox. ","Install|Uninstall|Exit","question"); 
end
if answ_install_dialog=='1'
  // Initial message
winId_wait = waitbar('                                  \n..
                  EE Scicos pack Setup.                 \n..
          Installation may take several seconds...      \n..
                                                        \n..
                                                        \n..
                                                        ');
  // Change working directory
  cd(MYDIR);
  // Load libraries
  getf scicos_ee\utils\utils.sci 
end
if answ_install_dialog=='2'
  // Uninstaller section: exec('unistaller.sce');
  // TEST if user is Admin
  cd(SCIDIR + '\contrib');
winId_wait = waitbar('                                  \n..
                    EE Scicos pack Setup.               \n..
          Uninstallation may take several seconds...    \n..
                                                        \n..
                                                        \n..
                                                        ');
  cmd = 'dir > test.x';
  unix(cmd);
  [x,err] = fileinfo('test.x');
  if err ~= 0
waitbar('                                               \n..
                 # Uninstallation Error #               \n..
   Access denied! Is not possible to delete the files.  \n..
   Please, run ScicosLab with administrator privileges. \n..
                    Uninstallation aborted!             \n..
                                                        ', winId_wait);
    cd(OLDDIR); return;
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
waitbar('                                               \n..
                 # Uninstallation Error #               \n..
                .scilab file is corrupted!              \n..
            (It is recommended to delete the file)      \n..
                                                        \n..
                                                        ', winId_wait);
    cd(OLDDIR); return;
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
waitbar('                                               \n..
                      Deleting files...                 \n..
                                                        \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
  EE_debug_printf('  Deleting files...', 0); 
  // ulink of DLLs
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
  if exists('lib_SMCflex_link_num')
    ulink(lib_SMCflex_link_num);
  end
  if exists('libsmcube_link_num')
    ulink(libsmcube_link_num);
  end

  // Removing ScicosLabPack_install.log
  [loginfo,ierr]=fileinfo('ScicosLabPack_install.log');
  if ierr==0
    cmd = 'del /Q ScicosLabPack_install.log';
    unix(cmd);
  end
  // Removing scicos_ee
  res = isdir(SCIDIR+'\contrib\scicos_ee');
  cd(SCIDIR + '\contrib');
  if res==%T
    cmd = 'rmdir /s /q scicos_ee';
    unix(cmd);
  end
  // 100%
  waitbar(1, winId_wait);
waitbar('                                               \n..
          Uninstallation completed successfully!        \n..
   Restart ScicosLab for the changes to take effect...  \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
  EE_debug_printf('  Uninstallation completed successfully!', 0); 
  EE_debug_printf('  Please, restart ScicosLab for the changes to take effect...', 0);
  cd(home); return;
end
if answ_install_dialog=='3' | answ_install_dialog=='0'
  cd(OLDDIR); return;
end

// Installation test
cd(SCIDIR + '\contrib');
cmd = 'dir > test.x';
unix(cmd);
[x,err] = fileinfo('test.x');
if err ~= 0
waitbar('                                               \n..
                   # Installation Error #               \n..
                       Access denied!                   \n..
   Please, run ScicosLab with administrator privileges. \n..
                      Installation aborted!             \n..
                                                        ', winId_wait);
    cd(OLDDIR); return;
else
  cmd = 'del /Q test.x';
  unix(cmd);
end
cd(MYDIR);
 // Installation log
[fd_log, err] = mopen(SCIDIR+'\contrib\ScicosLabPack_install.log', 'w');
if err ~= 0
waitbar('                                               \n..
                 # Installation Error #                 \n..
 Access denied! Is not possible to create the log file. \n..
  Please, run ScicosLab with administrator privileges.  \n..
                 Installation aborted!                  \n..
                                                        ', winId_wait);
  cd(OLDDIR); return;
else
  EE_debug_printf('### EE Scicoslab pack installer ###', fd_log);
end
  
// Check Cygwin presence
waitbar('                                               \n..
                 Check Cygwin presence...               \n..
                                                        \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
[x,ierr]=fileinfo('C:\cygwin\bin\bash.exe');
if ierr==0
waitbar('                                               \n..
              Check Cygwin presence...found!            \n..
                                                        \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
else
waitbar('                                               \n..
                  # Installation Error #                \n..
                    Cygwin not found!                   \n..
             Cygwin should be installed in C:\          \n..
                  Installation aborted!                 \n..
                                                        ', winId_wait);
  EE_debug_printf('  #Error Cygwin not found!', fd_log);
  EE_debug_printf('  Installation aborted!', fd_log);
  EE_debug_printf('### END ###', fd_log);
  mclose(fd_log);
  cd(OLDDIR); return;
end

// 10%
waitbar(0.1, winId_wait);

if exists('NO_APPS_CHECK')==0
	// Create apps.list (installed applications list) 
	cd(MYDIR+'scicos_ee\utils');
	cmd = 'installed_win_apps_list.bat > apps.list';
	unix(cmd);
	cd(MYDIR+'scicos_ee\utils');
	txt=mgetl('apps.list');

	// 20 %
	waitbar(0.2, winId_wait);

	// Check Visual C++ presence
	waitbar('                                               \n..
				 Check Visual C++ 2008 presence...          \n..
															\n..
															\n..
															\n..
															', winId_wait);
	res = grep(txt,'Visual C++ 2008');
	if res==[]
	  EE_debug_printf('  #warning: Visual C++ 2008 not found!', fd_log);
	  EE_debug_printf('  #warning: Installation of Visual C++ 2008 Express Edition is required by the ScicosLab EE pack!', fd_log);
	waitbar('                                               \n..
					# Installation Warning #                \n..
					 Visual C++ not found!                  \n..
		Installation of Visual C++ 2008 Express Edition     \n..
			  is required by the ScicosLab EE pack!         \n..
															', winId_wait);
	else
	waitbar('                                               \n..
			Check Visual C++ 2008 presence...found!         \n..
															\n..
															\n..
															\n..
															', winId_wait);
	end

	// Check Java presence
	waitbar('                                               \n..
					Check Java presence...                  \n..
															\n..
															\n..
															\n..
															', winId_wait);
	res = grep(txt,'Java(TM)');
	if res==[]
	  EE_debug_printf('  #warning: Java not found!', fd_log);
	  EE_debug_printf('  #warning: Installation of Java is required by the ScicosLab EE pack!', fd_log);
	waitbar('                                               \n..
				   # Installation Warning #                 \n..
					   Java not found!                      \n..
															\n..
															\n..
															', winId_wait);
	else
	waitbar('                                               \n..
				 Check Java presence...found!               \n..
															\n..
															\n..
															\n..
															', winId_wait);
	end

	// Check MPLAB C30 presence 
	waitbar('                                               \n..
			 Check MPLAB C30 compiler presence...           \n..
															\n..
															\n..
															\n..
															', winId_wait);
	res_c30 = grep(txt,'MPLAB C30');
	res_c   = grep(txt,'MPLAB C');
	if res_c30==[] & res_c==[]
		EE_debug_printf('  #warning: C30 compiler for dsPIC not found!', fd_log);
	waitbar('                                               \n..
				  # Installation Warning #                  \n..
			  C30 compiler for dsPIC not found!             \n..
															\n..
															\n..
															', winId_wait);
		answ = buttondialog("The installation requires valid paths for C30 compiler and ASM30 assembler (Yes: to continue, No: to abort)","yes|no","question"); 
		if answ=='2'
	waitbar('                                               \n..
				  # Installation Warning #                  \n..
			  Please, install a valid compiler              \n..
		   otherwise code generator will not work!          \n..
				   Installation aborted!                    \n..
															', winId_wait);
		  EE_debug_printf('  #warning: Please, install a valid C30 compiler otherwise code generator will not work!', fd_log);
		  EE_debug_printf('  Installation aborted!', fd_log);
		  EE_debug_printf('### END ###', fd_log);
		  mclose(fd_log);
		  cd(OLDDIR); return;
		end
	else
	waitbar('                                               \n..
					  C30 compiler settings...              \n..
															\n..
															\n..
															\n..
															', winId_wait);
	end
end

// C30 path
CC_path_valid=%F;

if exists('c30_asm30_paths')==0
	c30_asm30_paths='c:/Programmi/Microchip/MPLAB C30';
	if exists('INSTALLER_BATCH_MODE')==1
		EE_debug_printf('  Installation in batch mode aborted!', fd_log);
		EE_debug_printf('### END ###', fd_log);
		mclose(fd_log);
		cd(OLDDIR); return;
	end
end

while CC_path_valid==%F
	  if exists('INSTALLER_BATCH_MODE')==0
		c30_asm30_paths = x_dialog(['Set preferences';'Enter C30 installation directory path:'],[c30_asm30_paths]);
	  end
	  //CC_path_valid = isdir( c30_asm30_paths );
	  c30_asm30_paths = strsubst(c30_asm30_paths,'\','/');
	  ind_slash = strindex(c30_asm30_paths,'/');
	  len_path = length(c30_asm30_paths);
	  if ind_slash(length(ind_slash)) ==  len_path
		strs_temp = strsplit(c30_asm30_paths,[ind_slash(length(ind_slash))-1, ind_slash(length(ind_slash))-1]);
		c30_asm30_paths = strs_temp(1);
	  end
	  [x,ierr]=fileinfo(c30_asm30_paths + '/bin/pic30-gcc.exe');  
	  if ierr<0
		CC_path_valid = %F;
		if exists('INSTALLER_BATCH_MODE')==0
			answ = buttondialog("Inserted path is not valid! (Yes: to retry, No: to abort)","yes|no","question"); 
		else
			answ = '2';
		end
		if answ=='2'
		  waitbar('                                               \n..
					   # Installation Error #               \n..
				  Please, install a valid compiler          \n..
				otherwise code generator will not work!     \n..
				 path + /bin/pic30-gcc.exe NOT FOUND!       \n..
					   Installation aborted!               ', winId_wait);
		  EE_debug_printf('  Installation aborted!', fd_log);
		  EE_debug_printf('### END ###', fd_log);
		  mclose(fd_log);
		  cd(OLDDIR); return;
		end
	  else
		CC_path_valid = %T;
	  end
end


// COMMON_OIL.PREF file is created 
[fd,err] = mopen(MYDIR+'scicos_ee\utils\common_oil.pref', 'w');
if err ~= 0
waitbar('                                               \n..
                   # Installation Error #               \n..
   Access denied! Is not possible to create a pref file!\n..
   Please, run ScicosLab with administrator privileges. \n..
                   Installation aborted!                \n..
                                                        ', winId_wait);
  EE_debug_printf('  #error: Access denied! Is not possible to create a preferences file!', fd_log);
  EE_debug_printf('  #error: Please, run ScicosLab with administrator privileges to install the toolbox', fd_log);
  EE_debug_printf('  Installation aborted!', fd_log);
  EE_debug_printf('### END ###', fd_log);
  mclose(fd_log);
  cd(OLDDIR); return;
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
waitbar('                                               \n..
  Creating preferences file common_oil.pref ... Done!   \n..
                                                        \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);

// 30%
waitbar(0.3, winId_wait);

// Check the existence of scicos_ee folder in contrib
res = isdir(SCIDIR+'\contrib\scicos_ee');
if res==%T
waitbar('                                               \n..
                # Installation Warning #                \n..
      An old version of the ScicosLab pack was found!   \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
  EE_debug_printf('  An old version of the ScicosLab pack was found!', fd_log);
  
  if exists('answ_old_inst_removing')==0
	answ_old_inst_removing = buttondialog("The old version of the ScicosLab pack will be removed.\nDo you want to continue? (Yes: to continue, No: to abort)","yes|no","question");
  end
  if answ_old_inst_removing=='1'
  
    // UNINSTALLER: Delete scicos_ee folder from contrib
waitbar('                                               \n..
                     Deleting files...                  \n..
                                                        \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
    EE_debug_printf('  Deleting files...', fd_log); 
    // ulink of DLLs
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
    if exists('lib_SMCflex_link_num')
      ulink(lib_SMCflex_link_num);
    end
    if exists('libsmcube_link_num')
      ulink(libsmcube_link_num);
    end
  
    // Removing scicos_ee
    cd(SCIDIR+'\contrib');
    cmd = 'rmdir /s /q scicos_ee';
    unix(cmd);

  else
waitbar('                                               \n..
            The installation was interrupted!           \n..
                                                        \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
  EE_debug_printf('  The installation was interrupted!', fd_log);
  EE_debug_printf('### END ###', fd_log);
  mclose(fd_log);
  cd(OLDDIR); return;
  end
end

// ask if the user wants to proceed

if exists('answ_inst_continue')==0
	answ_inst_continue = buttondialog("The ScicosLab pack will be installed.\nDo you want to continue? (Yes: to continue, No: to abort)","yes|no","question");
end
if answ_inst_continue=='1'
waitbar('                                               \n..
        The ScicosLab pack for FLEX and Easylab         \n..
                 will be installed...                   \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
  cd(SCIDIR+'\contrib');
  EE_debug_printf('  EE Scicos pack will be installed...', fd_log);
  cmd = 'mkdir scicos_ee';
  unix(cmd);
  cmd = 'start xcopy '+ascii(34)+MYDIR+'scicos_ee'+ascii(34)+' '+ascii(34)+SCIDIR+'\contrib\scicos_ee'+ascii(34)+' /s /e /y /i';
  unix(cmd);
  
  else
waitbar('                                               \n..
            The installation was interrupted!           \n..
                                                        \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
  EE_debug_printf('  The installation was interrupted!', fd_log);
  EE_debug_printf('### END ###', fd_log);
  mclose(fd_log);
  cd(OLDDIR); return;
  end

// 40%
waitbar(0.4, winId_wait);

// Check copy is finished
EE_debug_printf('  ...copying Scicos EE files...', fd_log); 
waitbar('                                               \n..
                      Copying files...                  \n..
             (Copy may take several seconds!)           \n..
          Please, do not close the MSDOS window.        \n..
 It will be closed automatically at the end of the copy.\n..
                                                        ', winId_wait);
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
waitbar('                                               \n..
                 # Installation Error #                 \n..
  Access denied! Is not possible to install the toolbox!\n..
   Please, run ScicosLab with administrator privileges. \n..
                 Installation aborted!                  \n..
                                                        ', winId_wait);
  EE_debug_printf('  #error: Access denied! Is not possible to create the directories!', fd_log);
  EE_debug_printf('  #error: Please, run ScicosLab with administrator privileges to install the toolbox', fd_log);
  EE_debug_printf('  Installation aborted!', fd_log);
  // winclose(winId_prog);
  EE_debug_printf('### END ###', fd_log);
  mclose(fd_log);
  cd(OLDDIR); return;
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
  waitbar('                                               \n..
                # Installation Warning #                \n..
   The installation should modify the .scilab script    \n..
          Please, if the .scilab file is open,          \n..
               close it before proceeding...            \n..
                                                        ', winId_wait);
  EE_debug_printf('  #warning: Please, if the .scilab file is open, close it before proceeding...', fd_log);
  if exists('answ_dot_scilab')==0
	answ_dot_scilab = buttondialog("The installation should modify the .scilab script (Yes: to continue, No: to abort)","yes|no","question"); 
  end
  // EE_debug_printf('  #warning: .scilab file found!', fd_log);
  txt=mgetl(SCIHOME+'\.scilab');
else
  // EE_debug_printf('  #warning: .scilab file not found!. The file will be created.', fd_log);
  txt=[];
  answ_dot_scilab = '1';
end
if answ_dot_scilab=='1'

    res = grep(txt,'### Scicos EE ###');

    if res ~= []
      // UNINSTALLER: .scilab will be cleaned
      // EE_debug_printf('  #warning: .scilab should be cleaned!', 0);
      res_begin = grep(txt,'### Scicos EE ###');
      res_end = grep(txt,'### Scicos EE - end ###');
      if res_end==[]
	waitbar('                                               \n..
                   # Installation Error #               \n..
                 .scilab file is corrupted!             \n..
         Please, delete the .scilab file and retry...   \n..
                   Installation aborted!                \n..
                                                        ', winId_wait);
      EE_debug_printf('  #error: .scilab file is corrupted!', fd_log);
      EE_debug_printf('  Installation aborted!', fd_log);
      EE_debug_printf('### END ###', fd_log);
      mclose(fd_log);
      cd(OLDDIR); return;
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

// 99%
waitbar(0.99, winId_wait);

// Build Scicos EE pack
waitbar('                                               \n..
                  Building ScicosLab EE pack...         \n..
                                                        \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
EE_debug_printf('  Building ScicosLab EE pack...', fd_log);
cd(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic");
exec builder.sce

// Create and move Scicos EE pack palettes
waitbar('                                               \n..
            Creating ScicosLab EE palettes...           \n..
                                                        \n..
                                                        \n..
                                                        \n..
                                                        ', winId_wait);
EE_debug_printf('  Creating ScicosLab EE palettes...', fd_log);
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\AMAZING");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-Communication");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-DMB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-MTB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\RT-Data-Exchange");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\EASYLAB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\SMCube");

cd(MYDIR+'scicos_ee\utils');
cmd = 'start cp_cosf.bat ' + ascii(34)+SCIDIR+ascii(34);
unix(cmd);
cmd = 'start xcopy '+ascii(34)+SCIDIR+'\contrib\scicos_ee\utils\common_oil.pref'+ascii(34)+' '+ascii(34)+SCIDIR+'\contrib\scicos_ee\RT-Druid\configuration'+ascii(34)+' /s /e /y /i';
unix(cmd);
if exists('NO_APPS_CHECK')==0
	cmd = 'del apps.list';
	unix(cmd);
end
[x,err] = fileinfo('common_oil.pref');
if err == 0
	cmd = 'del common_oil.pref';
	unix(cmd);
end
cd(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic");

// End
// 100%
waitbar(1, winId_wait);
waitbar('                                               \n..
           Installation completed successfully!         \n..
        Please, restart ScicosLab for the changes       \n..
                   to take effect...                    \n..
                                                        \n..
                                                        ', winId_wait);
EE_debug_printf('  Installation completed successfully!', fd_log); 
EE_debug_printf('  Please, restart ScicosLab for the changes to take effect...', fd_log);
EE_debug_printf('### END ###', fd_log);
mclose(fd_log);
cd(OLDDIR); return;


