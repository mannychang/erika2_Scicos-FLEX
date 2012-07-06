// Script to load the Flex-Sim dynamic library.
// Author: Dario Di Stefano, Evidence Srl.

flexsim_path = get_absolute_file_path('loader.sce');

// -------- Modules to be imported ----------

// Flex
comp_fun = ['flex_adcin',..
            'flex_gpin',..
            'flex_gpout',..
            'flex_lcd',..
            'flex_led'];

// Flex-DMB
comp_fun =[comp_fun,..
            'flex_daughter_acc',..
            'flex_daughter_adc',..
            'flex_daughter_button',..
            'flex_daughter_buzzer',..
            'flex_daughter_dac',..
            'flex_daughter_encoder',..
            'flex_daughter_gpin',..
            'flex_daughter_gpout',..
            'flex_daughter_leds_barrier',..
            'flex_daughter_lcd',..
            'flex_daughter_ldr',..
            'flex_daughter_leds',..
            'flex_daughter_pwm',..
            'flex_daughter_therm',..
            'flex_daughter_trimmer',..
            'flex_blocks'];

// Flex-MTB
// 'flex_daughter_button', 'flex_daughter_encoder', 'flex_daughter_leds', 'flex_daughter_pwm' -> see DMB
comp_fun =[comp_fun,..        
            'flex_daughter_dcm_pwm',..
            'flex_daughter_switch',..
            'flex_daughter_touch'];

// Flex-Comm
comp_fun =[comp_fun,.. 
            'flex_simple802154_coordinator_config',..
            'flex_simple802154_device_config',..
            'flex_simple802154_receive',..
            'flex_simple802154_send',..
            'flex_can_config',..
            'flex_can_in',..
            'flex_can_out',..
            'flex_serial_receive',..
            'flex_serial_send',..
            'flex_serial_config',..
            'flex_udp_config',..
            'flex_udp_receive',..
            'flex_udp_send',..
            'flex_usb_receive',..
            'flex_usb_send'];

// Easylab
comp_fun =[comp_fun,.. 
            'easylab_adc',..
            'easylab_buzzer',..
            'easylab_gpin',..
            'easylab_gpout',..
            'easylab_leds',..
            'easylab_pwm',..
            'easylab_serial_receive',..
            'easylab_serial_send',..
            'easylab_sys_led'];

// Amazing Ball
// 'flex_daughter_leds' -> see DMB
// 'flex_daughter_switch' -> see MTB
comp_fun =[comp_fun,.. 
            'amazing_button',..
            'amazing_pwm',..
            'amazing_reset',..
            'amazing_touch',..
            'amazing_tuner'];

// Link to import the selected modules of the library
// Flex Demo Board simulation (QT executable)
if (execstr('getenv(''FLEXPATH'')','errcatch') == 0)
	flexsim_link_num = link(flexsim_path + 'Release\libflexsim.dll', comp_fun, 'c');
else
	flexsim_link_num = link(flexsim_path + 'Release_fake\libflexsim.dll', comp_fun, 'c');
end

clear flexsim_path comp_fun x_x_x dmbexe_err;
