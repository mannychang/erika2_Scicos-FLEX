//###################################
// ScicosLab pack installation script
//###################################

winId_wait=waitbar('EE Scicos pack. Uninstallation may take several minutes...');

// Set global variables
EE_DEBUG = 1;
MYDIR = get_absolute_file_path('uninstaller.sce');
OLDDIR = pwd();
SCIDIR = strsubst(SCI,'/','\');
MYDIR = strsubst(MYDIR,'\','/')

// Change working directory
cd(MYDIR)

// Load libraries
getf scicos_ee\utils\utils.sci 

waitbar(0.2, winId_wait);

EE_debug_printf('### EE Scicoslab pack uninstaller ###', 1);

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
    EE_debug_printf('  #warning: .scilab is already cleaned!', 1);
  elseif res_end==[]
    EE_debug_printf('  #error: .scilab file is corrupted!', 1);
  else
    EE_debug_printf('  #warning: .scilab was successfully restored!', 1);
    res = size(txt);
    new_txt = [txt(1:res_begin-1);txt(res_end+1:res(1))];
    mputl(new_txt, SCIHOME+'\.scilab');
  end
else
  EE_debug_printf('  #warning: .scilab file not found!.', 1);
end

waitbar(0.5, winId_wait);

// Delete scicos_ee folder from contrib
res = isdir(SCIDIR+'\contrib\scicos_ee');
if res==%T
  cd(SCIDIR+'\contrib');
  //EE_debug_printf('  EE Scicos pack will be uninstalled!', 1);
  EE_debug_printf('  ...removing Scicos EE folders...', 1); 
  //rmdir('scicos_ee', 's');
  cmd = 'rmdir /s /q scicos_ee';
  unix(cmd);
  
  EE_debug_printf('  #warning: If scicos_ee folder is still present in contrib, close ScicosLab and remove it by hand!', 1);

  // Check deletion is finished
  //realtimeinit(1); 
  //realtime(0);
  //winId_prog=progressionbar('EE Scicos pack. Removing files...');
  //res = %T;
  //i = 0;
  //while res==%T,
  //  res = isdir(SCIDIR+'\contrib\scicos_ee');
  //  i = i+1;
  //  realtime(i);
  //  progressionbar(winId_prog);
  //end
  //winclose(winId_prog);
  
else
  EE_debug_printf('  #warning: EE Scicos pack is already uninstalled!', 1);
  EE_debug_printf('  If you want to install the Scicos EE pack, use the installer.sce script.', 1);
end

waitbar(1, winId_wait);
waitbar('Uninstallation successfully completed!',winId_wait);
EE_debug_printf('  Uninstallation completed successfully!', 1); 
EE_debug_printf('  Please, restart ScicosLab for the changes to take effect...', 1);
EE_debug_printf('### ###', 1);

// Change the working directory:
cd(home);

// End
return;
