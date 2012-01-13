mode(-1) //force silent execution

path=get_absolute_file_path('loader.sce');//get the absolute path of this file

add_help_chapter("Amazing Ball Blockset",path+'amazing');
add_help_chapter("Flex Blockset",path+'flex');
add_help_chapter("Flex-DemoBoard Blockset",path+'flex_dmb');
add_help_chapter("Flex-MotionBoard Blockset",path+'flex_mtb');
add_help_chapter("RT Data Exchange Blockset",path+'rt_data_exchange');
add_help_chapter("Flex-Communication Blockset",path+'flex_com');
add_help_chapter("MCHP16-Sources Blockset",path+'misc');
add_help_chapter("EasyLab Blockset",path+'easylab');

// SMCube docs
SCIDIR = strsubst(SCI,'/','\');
[x,err] = fileinfo(SCIDIR + "\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\SMCube\SMCube.sci");
if err == 0
	add_help_chapter("SMCube Blockset",path+'SMCube');
end

//clear the variable stack
clear path add_help_chapter get_absolute_file_path

