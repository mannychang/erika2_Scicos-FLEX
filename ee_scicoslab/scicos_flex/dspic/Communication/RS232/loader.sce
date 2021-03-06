// ------------------------------------------------------
// Script to load dynamic-link library: Please do not edit this file
// ------------------------------------------------------

rs232_receiver_path = get_absolute_file_path('loader.sce');
rs232_dll_mode = 'Release';
//rs232_dll_mode = 'Debug';

rs232_link_num = link(rs232_receiver_path + rs232_dll_mode + '\libFlexRS232Comms.dll',['rs232_config','rs232_receiver','rs232_sender'],'c');
// remove temp. variables on stack
clear rs232_receiver_path rs232_dll_mode;
// ------------------------------------------------------
