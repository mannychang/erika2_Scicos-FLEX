//###################################
// ScicosLab pack installation script
//###################################

winId_wait=waitbar('EE Scicos pack. Uninstallation may take a few seconds...');

// Set global variables
EE_DEBUG = 1;
MYDIR = get_absolute_file_path('uninstaller.sce');
OLDDIR = pwd();
SCIDIR = strsubst(SCI,'/','\');

// Change working directory
cd(MYDIR);

// Load libraries
getf scicos_ee\utils\utils.sci 

// Check .scilab file presence
cd(MYDIR+'scicos_ee\user\Scilab\4.4b7');
[x,ierr]=fileinfo(home+'\Scilab\4.4b7\.scilab');
if ierr==0
  EE_debug_printf('.scilab file found!', 1);
  txt=mgetl(home+'\Scilab\4.4b7\.scilab');
  res_begin = grep(txt,'### Scicos EE ###');
  res_end = grep(txt,'### Scicos EE - end ###');
  if res_begin==[]
    EE_debug_printf('#warning: .scilab is already cleaned!', 1);
  elseif res_end==[]
    EE_debug_printf('#error: .scilab file is corrupted!', 1);
  else
    EE_debug_printf('#warning: .scilab will be modified!', 1);
    res = size(txt);
    new_txt = [txt(1:res_begin-1);txt(res_end+1:res(1))];
    mputl(new_txt, home+'\Scilab\4.4b7\.scilab');
  end
else
  EE_debug_printf('#warning: .scilab file not found!.', 1);
end

waitbar(0.5, winId_wait);

// Delete scicos_ee folder from contrib
res = isdir(SCIDIR+'\contrib\scicos_ee');
if res==%T
  cd(SCIDIR+'\contrib');
  EE_debug_printf('EE Scicos pack will be uninstalled!', 1);
  EE_debug_printf('...removing Scicos EE files...', 1); 
  rmdir('scicos_ee', 's');
  
  // Check deletion is finished
  realtimeinit(1); 
  realtime(0);
  winId_prog=progressionbar('EE Scicos pack. Removing files...');
  res = %T;
  i = 0;
  while res==%T,
    res = isdir(SCIDIR+'\contrib\scicos_ee');
    i = i+1;
    realtime(i);
    progressionbar(winId_prog);
  end
  winclose(winId_prog);
  
else
  EE_debug_printf('#warning: EE Scicos pack is already uninstalled!', 1);
  EE_debug_printf('If you want to install the Scicos EE pack, use the installer.sce file.', 1);
  waitbar(1, winId_wait);
  waitbar('Uninstallation finished!',winId_wait);
  return;
end

waitbar(1, winId_wait);
waitbar('Uninstallation completed successfully!',winId_wait);

// Change the working directory:
cd(home);

// End
return;
