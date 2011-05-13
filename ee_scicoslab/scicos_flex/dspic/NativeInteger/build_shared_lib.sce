exec("dll_builder.sci");

comp_fun = ['nat_gainblk_i32n'   'nat_gainblk_i16n'   'nat_gainblk_i8n' ...
            'nat_summation_i32n' 'nat_summation_i16n' 'nat_summation_i8n'...
            'nat_product_i32n' ] ;
// list of computational functions 

c_prog_lst = listfiles('*.c') ; // get all the *.c source files

prog_lst = strsubst(c_prog_lst, '.c', '.o'); // assemble the list of object files 

libs = "" ; //** no ext lib 

flag = "c"         ; //** C Code computational functions

makename = 'Makelib'; //** makefile.mak name 

loadername = 'loader.sce' ; // loader name 

libname = 'nativeinteger' ; // name of the generated dll 

libn = dll_builder(comp_fun, prog_lst, libs, flag, makename, loadername, libname) ; // build the library

