//###################################
// ScicosLab pack installation script
//###################################

winId_wait=waitbar('EE Scicos pack. Installation may take several minutes...');

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

EE_debug_printf('### EE Scicoslab pack installer ###', 1);

// Check Cygwin presence
EE_debug_printf('  Check Cygwin presence...', 1);
[x,ierr]=fileinfo('C:\cygwin\bin\bash.exe');
if ierr==0
  EE_debug_printf('  Cygwin found!', 1);
else
  EE_debug_printf('  #error: Cygwin not found!', 1);
  EE_debug_printf('  #error: Cygwin should be installed in C:\', 1);
  EE_debug_printf('  Installation aborted!', 1);
  waitbar('Error: Installation aborted!',winId_wait);
  EE_debug_printf('### ###', 1);
  return;
end

waitbar(0.1, winId_wait);

// Create apps.list (installed applications list) 
cd(MYDIR+'scicos_ee\utils');
cmd = 'installed_win_apps_list.bat > apps.list';
unix(cmd);

waitbar(0.2, winId_wait);

// Check Visual C++ presence
EE_debug_printf('  Check Visual C++ 2008 presence...', 1);
cd(MYDIR+'scicos_ee\utils');
txt=mgetl('apps.list');
res = grep(txt,'Visual C++ 2008');
if res==[]
  EE_debug_printf('  #warning: Visual C++ 2008 not found!', 1);
  EE_debug_printf('  #warning: Installation of Visual C++ 2008 Express Edition is required by the ScicosLab EE pack!', 1);
else
  EE_debug_printf('  Visual C++ 2008 found!', 1);
end

// Check Java presence
EE_debug_printf('  Check Java presence...', 1);
cd(MYDIR+'scicos_ee\utils');
txt=mgetl('apps.list');
res = grep(txt,'Java(TM)');
if res==[]
  EE_debug_printf('  #warning: Java not found!', 1);
  EE_debug_printf('  #warning: Installation of Java is required by the ScicosLab EE pack!', 1);
else
  EE_debug_printf('  Java found!', 1);
end

// Check MPLAB C30 presence 
EE_debug_printf('  Check MPLAB C30 compiler presence...', 1);
cd(MYDIR+'scicos_ee\utils');
txt=mgetl('apps.list');
res_c30 = grep(txt,'MPLAB C30');
res_c   = grep(txt,'MPLAB C');
if res_c30==[] & res_c==[]
    EE_debug_printf('  #warning: C30 compiler for dsPIC not found!', 1);
    answ = buttondialog("The installation requires valid paths for C30 compiler and ASM30 assembler (Yes: to continue, No: to abort)","yes|no","question"); 
    if answ=='2'
      EE_debug_printf('  #warning: Please, install a valid compiler otherwise code generator will not work!', 1);
      EE_debug_printf('  Installation aborted!', 1);
      waitbar('Error: Installation aborted!',winId_wait);
      EE_debug_printf('### ###', 1);
      return;
    end
else
  EE_debug_printf('  C30 compiler found!', 1);
end

c30_asm30_paths = x_dialog(['Set preferences';'Enter C30 and ASM30 paths [C30_path;ASM30_path]:'],['c:\Programmi\Microchip\MPLAB C30';'c:\Programmi\Microchip\MPLAB ASM30 Suite'])

[fd,err] = mopen('common_oil.pref', 'w');

if err ~= 0
  EE_debug_printf('  #error: Access denied! Is not possible to create a preferences file!', 1);
  EE_debug_printf('  #error: Please, run ScicosLab with administrator privileges to install the toolbox', 1);
  EE_debug_printf('  Installation aborted!', 1);
  waitbar('Error: Installation aborted!',winId_wait);
  EE_debug_printf('### ###', 1);
  return;
end

mfprintf(fd,"# Path to the ASM30 Assembler\n");
mfprintf(fd,"# NOT the assembler distributed with the C30 Compiler!\n");
mfprintf(fd,"preference_pic30__path_for_asm_compiler = ");
mfprintf(fd, strsubst(c30_asm30_paths(2),'\','\\\\') );
mfprintf(fd,"\n");
mfprintf(fd,"\n");
mfprintf(fd,"# Path to the C30 Assembler\n");
mfprintf(fd,"preference_pic30__path_for_gcc_compiler = ");
mfprintf(fd, strsubst(c30_asm30_paths(1),'\','\\\\') );
mfprintf(fd,"\n");
mfprintf(fd,"\n");
mclose(fd);

waitbar(0.3, winId_wait);

// Check the existence of scicos_ee folder in contrib
EE_debug_printf('  Check scicos_ee folder presence...', 1);
res = isdir(SCIDIR+'\contrib\scicos_ee');
if res==%F
  cd(SCIDIR+'\contrib');
  EE_debug_printf('  EE Scicos pack will be installed!', 1);
  cmd = 'mkdir scicos_ee';
  unix(cmd);
  cmd = 'start xcopy '+ascii(34)+MYDIR+'scicos_ee'+ascii(34)+' '+ascii(34)+SCIDIR+'\contrib\scicos_ee'+ascii(34)+' /s /e /y /i';
  unix(cmd);
else
  EE_debug_printf('  #error: A folder named scicos_ee is already in contrib. Please remove it or rename it before running the installer', 1);
  EE_debug_printf('  Installation aborted!', 1);
  waitbar(1, winId_wait);
  waitbar('Installation aborted!',winId_wait);
  EE_debug_printf('### ###', 1);
  return;
end

waitbar(0.4, winId_wait);

// Check copy is finished
realtimeinit(1); 
realtime(0);
winId_prog=progressionbar('EE Scicos pack. Copying files...');
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
  EE_debug_printf('  #error: Access denied!', 1);
  EE_debug_printf('  #error: Please, run ScicosLab with administrator privileges to install the toolbox', 1);
  EE_debug_printf('  Installation aborted!', 1);
  waitbar('Error: Installation aborted!',winId_wait);
  winclose(winId_prog);
  EE_debug_printf('### ###', 1);
  return;
end

EE_debug_printf('  ...copying Scicos EE files...', 1); 
EE_debug_printf('  Please, do not close DOS window.', 1); 
EE_debug_printf('  It will be closed automatically at the end of the copy.', 1);
  
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
  res_flex_msvc = isdir(SCIDIR+'\contrib\scicos_ee\scicos_flex\MSVC2008_Patch');
  res = res + res_flex_msvc*0.02;
  res_flex_rttemp = isdir(SCIDIR+'\contrib\scicos_ee\scicos_flex\RT_templates');
  res = res + res_flex_rttemp*0.02;
  res_user = isdir(SCIDIR+'\contrib\scicos_ee\user');
  res = res + res_user*0.04;
  res_utils = isdir(SCIDIR+'\contrib\scicos_ee\utils');
  res = res + res_utils*0.02;
  
  waitbar(res, winId_wait);
  
  i = i+1;
  realtime(i);
  progressionbar(winId_prog);
  
end
winclose(winId_prog);

waitbar(0.9, winId_wait);

// Check .scilab file presence
EE_debug_printf('  Elaborating the initial configuration script...', 1); 
res = grep(SCIHOME,'4.4.1');
if res==[]
  cd(MYDIR+'scicos_ee\user\Scilab\4.4b7'); // 4.4b7
else
  cd(MYDIR+'scicos_ee\user\ScicosLab\4.4.1'); // 4.4.1
end
[x,ierr]=fileinfo(SCIHOME+'\.scilab');
if ierr==0
  EE_debug_printf('  #warning: .scilab file found!', 1);
  txt=mgetl(SCIHOME+'\.scilab');
else
  EE_debug_printf('  #warning: .scilab file not found!. The file will be created.', 1);
  txt=[];
end
res = grep(txt,'### Scicos EE ###');
if res==[]
  answ = '1';
  if ierr==0
  EE_debug_printf('  #warning: We recommend you to get a backup of .scilab file before proceeding...', 1);
  EE_debug_printf('  #warning: Please, if the .scilab file is open, close it before proceeding...', 1);
  answ = buttondialog("The installation should modify the .scilab script (Yes: to continue, No: to abort)","yes|no","question");   
  end
  if answ=='1'
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
else
  EE_debug_printf('  #warning: .scilab is already updated!', 1);
end

waitbar(0.99, winId_wait);

// Build Scicos EE pack
EE_debug_printf('  Building ScicosLab EE pack...', 1);
cd(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic");
exec builder.sce

// Create and move Scicos EE pack palettes
EE_debug_printf('  Creating ScicosLab EE palettes...', 1);
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\AMAZING");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-Communication");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-DMB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-MTB");
create_palette(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-PC");

cd(MYDIR+'scicos_ee\utils');
cmd = 'start cp_cosf.bat ' + ascii(34)+SCIDIR+ascii(34);
unix(cmd);
cmd = 'start xcopy '+ascii(34)+MYDIR+'scicos_ee\utils\common_oil.pref'+ascii(34)+' '+ascii(34)+SCIDIR+'\contrib\scicos_ee\RT-Druid\configuration'+ascii(34)+' /s /e /y /i';
unix(cmd);
cmd = 'del apps.list';
unix(cmd);
cmd = 'del common_oil.pref';
unix(cmd);
cd(SCIDIR+"\contrib\scicos_ee\scicos_flex\dspic");

// End
waitbar(1, winId_wait);
waitbar('Installation completed successfully!',winId_wait);
EE_debug_printf('  Installation completed successfully!', 1); 
EE_debug_printf('  Please, restart ScicosLab for the changes to take effect...', 1);
EE_debug_printf('### ###', 1);
return;


