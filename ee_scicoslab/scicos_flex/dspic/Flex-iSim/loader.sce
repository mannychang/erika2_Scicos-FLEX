// Script to load the Flex-Sim dynamic library.
// Author: Dario Di Stefano, Evidence Srl.

// -------- Modules to be imported ----------

// Flex-block interactive simulators
comp_fun = 'flex_blocks';

// Link to import the selected modules of the library
// Flex Demo Board simulation (QT executable)
if (execstr('getenv(''FLEXPATH'')','errcatch') == 0)
	flexsim_link_num = link(get_absolute_file_path('loader.sce') + 'release\libflex_iSim.dll', comp_fun, 'c');
else
	flexsim_link_num = link(get_absolute_file_path('loader.sce') + 'release_fake\libflex_iSim.dll', comp_fun, 'c');
end

clear comp_fun;
