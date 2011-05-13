//###################################
// ScicosLab pack installation script
//###################################

winId_wait=waitbar('EE Scicos pack. Installation may take a few seconds...');

// Set global variables
EE_DEBUG = 1;
MYDIR = get_absolute_file_path('installer.sce');
OLDDIR = pwd();
SCIDIR = strsubst(SCI,'/','\');

// Change working directory
disp(MYDIR);
cd(MYDIR);

// Load libraries
getf scicos_ee\utils\utils.sci 

// Check Cygwin presence
[x,ierr]=fileinfo('C:\cygwin\bin\bash.exe');
if ierr==0
  EE_debug_printf('Cygwin found!', 1);
else
  EE_debug_printf('#error: Cygwin not found!', 1);
  waitbar('Error: Installation aborted!',winId_wait);
  return;
end

waitbar(0.1, winId_wait);

// Create apps.list (installed applications list) 
cd(MYDIR+'scicos_ee\utils');
cmd = 'installed_win_apps_list.bat > apps.list';
unix(cmd);

waitbar(0.2, winId_wait);

// Check Visual C++ presence
cd(MYDIR+'scicos_ee\utils');
txt=mgetl('apps.list');
res = grep(txt,'Visual C++');
if res==[]
  EE_debug_printf('#error: Visual C++ not found!', 1);
  waitbar('Error: Installation aborted!',winId_wait);
  return;
else
  EE_debug_printf('Visual C++ found!', 1);
end

// Check MPLAB C30 presence 
cd(MYDIR+'scicos_ee\utils');
txt=mgetl('apps.list');
res = grep(txt,'C30');
if res==[]
  EE_debug_printf('#warning: C30 compiler for dsPIC not found!', 1);
  EE_debug_printf('#warning: Please, install a valid compiler otherwise code generator will not work!', 1);
else
  EE_debug_printf('C30 compiler found!', 1);
end

waitbar(0.3, winId_wait);

// Check the existence of scicos_ee folder in contrib
res = isdir(SCIDIR+'\contrib\scicos_ee');
if res==%F
  cd(SCIDIR+'\contrib');
  EE_debug_printf('EE Scicos pack will be installed!', 1);
  EE_debug_printf('...copying Scicos EE files...', 1); 
  EE_debug_printf('Please, do not close DOS window.', 1); 
  EE_debug_printf('It will be closed automatically at the end of the copy.', 1);
  cmd = 'mkdir scicos_ee';
  unix(cmd);
  cmd = 'start xcopy '+ascii(34)+MYDIR+'scicos_ee'+ascii(34)+' '+ascii(34)+SCIDIR+'\contrib\scicos_ee'+ascii(34)+' /s /e /y /i';
  unix(cmd);
else
  EE_debug_printf('#error: EE Scicos pack is already installed!', 1);
  EE_debug_printf('If you want to uninstall the Scicos EE pack, use the uninstaller.sce file.', 1);
  EE_debug_printf('Uninstaller will completely remove the scicos_ee from contrib and will change .scilab file', 1);
  EE_debug_printf('so we recommend you to get a backup before running the uninstaller...', 1);
  waitbar(1, winId_wait);
  waitbar('Installation finished!',winId_wait);
  return;
end

waitbar(0.4, winId_wait);

// Check copy is finished
realtimeinit(1); 
realtime(0);
winId_prog=progressionbar('EE Scicos pack. Copying files...');
res = %F;
i = 0;
while res==%F,
  res = isdir(SCIDIR+'\contrib\scicos_ee\utils');
  i = i+1;
  realtime(i);
  progressionbar(winId_prog);
end
winclose(winId_prog);

waitbar(0.9, winId_wait);

// Check .scilab file presence
cd(MYDIR+'scicos_ee\user\Scilab\4.4b7');
[x,ierr]=fileinfo(home+'\Scilab\4.4b7\.scilab');
if ierr==0
  EE_debug_printf('.scilab file found!', 1);
  txt=mgetl(home+'\Scilab\4.4b7\.scilab');
else
  EE_debug_printf('#warning: .scilab file not found!. The file will be created.', 1);
  txt=[];
end
res = grep(txt,'### Scicos EE ###');
if res==[]
  answ = 1;
  if ierr==0
  EE_debug_printf('#warning: .scilab will be modified!', 1);
  EE_debug_printf('We recommend you to get a backup of .scilab file before proceeding...', 1);
  answ = buttondialog("Do you want to modify .scilab file?","yes|no","question");   
  end
  if answ=='1'
    [fd,err]=mopen(home+'\Scilab\4.4b7\.scilab', 'a');
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
    mclose(home+'\Scilab\4.4b7\.scilab');
  end
else
  EE_debug_printf('#warning: .scilab is already updated!', 1);
end

waitbar(0.99, winId_wait);

// Build Scicos EE pack
cd(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic");
exec builder.sce

waitbar(1, winId_wait);
waitbar('Installation completed successfully!',winId_wait);

// End
EE_debug_printf('Done!', 1); 
EE_debug_printf('Please, restart ScicosLab for the changes to take effect...', 1);
return;


