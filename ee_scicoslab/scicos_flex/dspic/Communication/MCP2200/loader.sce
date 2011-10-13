// Script to load the Flex-Sim dynamic library.
// Author: Dario Di Stefano, Evidence Srl.

mcp2200lib_path = get_absolute_file_path('loader.sce');

// -------- Modules to be imported ----------

// mchp16 sources for dspic
comp_fun = ['mcp2200_read',..
            'mcp2200_write',..
            'mcp2200_cfg'];

// Link to import the selected modules of the library
mcp2200_link_num = link(mcp2200lib_path + 'Release\libmcp2200.dll', comp_fun, 'c');
