// Script to load the serial gateway dynamic library.
// Author: Errico Guidieri, Evidence Srl.

// -------- Modules to be imported ----------

// Link to import the selected modules of the library
serial_gateway_link_num = link(get_absolute_file_path('loader.sce') + 'Release\lib_serial_gateway.dll', 'serial_gateway_block', 'c');
