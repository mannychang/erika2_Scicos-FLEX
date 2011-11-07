// Environment configuration 
INSTALLER_BATCH_MODE = [];
answ_install_dialog = '1';
NO_APPS_CHECK = [];
answ_old_inst_removing='1';
answ_dot_scilab = '1';
answ_inst_continue = '1';
//c30_asm30_paths = 'C:\Program Files (x86)\Microchip\MPLAB_C30_v325';
c30_asm30_paths = 'C:\Programmi\Microchip\MPLAB_C30_v325';

// Run the installer
SINST_DIR = get_absolute_file_path('silent_installer.sce');
exec(SINST_DIR + 'installer.sce');
quit