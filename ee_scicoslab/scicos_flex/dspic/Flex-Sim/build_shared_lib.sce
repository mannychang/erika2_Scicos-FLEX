exec("dll_builder.sci");

// list of computational functions
comp_fun = ['rt_sinus' 'rt_square' 'rt_step' 'rt_randm' 'rt_adcin' 'rt_buzzer' 'rt_gpin' 'rt_gpout' 'rt_lcd' 'rt_led' 'rt_pwmout'...
            'flex_dmb_adc' 'flex_dmb_button' 'flex_dmb_buzzer' 'flex_dmb_dac' 'flex_dmb_encoder' 'flex_dmb_gpin' 'flex_dmb_gpout' ...
            'flex_dmb_inertial' 'flex_dmb_lcd' 'flex_dmb_ldr' 'flex_dmb_leds' 'flex_dmb_ntc' 'flex_dmb_potin' 'flex_dmb_pwm' ] ;
// list of computational functions 

c_prog_lst = listfiles('*.c') ; // get all the *.c source files

prog_lst = strsubst(c_prog_lst, '.c', '.o'); // assemble the list of object files 

libs = "" ; //** no ext lib 

flag = "c"         ; //** C Code computational functions

makename = 'Makelib'; //** makefile.mak name 

loadername = 'loader.sce' ; // loader name 

libname ='flexsim' ; // name of the generated dll 

libn = dll_builder(comp_fun, prog_lst, libs, flag, makename, loadername, libname) ; // build the library

