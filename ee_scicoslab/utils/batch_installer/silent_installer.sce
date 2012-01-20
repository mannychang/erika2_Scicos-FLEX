// Environment configuration 
INSTALLER_BATCH_MODE = [];
ans_inst_dialog = '1';
NO_APPS_CHECK = [];
answ_c30 = '1';
ans_old_inst_rm = '1';
answ_dot_scilab = '1';
ans_inst_go = '1';
c30_asm30_paths = 'C:\Programmi\Microchip\mplabc30\v3.30';
visualc_path = 'C:\Programmi\Microsoft Visual Studio 9.0\VC';

// Run the installer
SINST_DIR = get_absolute_file_path('silent_installer.sce');
exec(SINST_DIR + 'installer.sce');
quit