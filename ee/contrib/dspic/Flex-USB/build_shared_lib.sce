//**
//** 24 June 2008 : Scilab/Scicos shared library build script 
//**
comp_fun = ["flex_usb_dummy_read" "flex_usb_dummy_write"] ; // list of computational functions 

c_prog_lst = listfiles('*.c') ; // get all the *.c source files

prog_lst = strsubst(c_prog_lst, '.c', '.o'); // assemble the list of object files 

libs = "flex_usbdll" ; //** external USB communication library 

flag = "c"         ; //** C Code computational functions

makename = 'Makelib'; //** makefile.mak name 

loadername = 'loader.sce' ; // loader name 

libname ='flex_usb_comm' ; // name of the generated dll 

libn = ilib_for_link(comp_fun, prog_lst, libs, flag, makename, loadername, libname) ; // build the library

