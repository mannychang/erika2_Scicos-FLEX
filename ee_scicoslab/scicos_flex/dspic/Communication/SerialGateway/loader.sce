// Script to load the serial gateway dynamic library.
// Author: Errico Guidieri, Evidence Srl.

serial_gateway_lib_path = get_absolute_file_path('loader.sce');

// -------- Modules to be imported ----------

// mchp16 sources for dspic
comp_fun   = ['serial_gateway_block'];

// Link to import the selected modules of the library
serial_gateway_link_num = link(serial_gateway_lib_path + 'Release\lib_serial_gateway.dll', comp_fun, 'c');