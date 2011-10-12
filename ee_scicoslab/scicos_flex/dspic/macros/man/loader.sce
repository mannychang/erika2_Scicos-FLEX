mode(-1) //force silent execution

path=get_absolute_file_path('loader.sce');//get the absolute path of this file

add_help_chapter("Amazing Ball Blockset",path+'amazing');
add_help_chapter("Flex Blockset",path+'flex');
add_help_chapter("Flex-DemoBoard Blockset",path+'flex_dmb');
add_help_chapter("Flex-MotionBoard Blockset",path+'flex_mtb');
add_help_chapter("Flex-PC Blockset",path+'flex_pc');
add_help_chapter("Flex-Communication Blockset",path+'flex_com');
add_help_chapter("MCHP16-Sources Blockset",path+'misc');

//clear the variable stack
clear path add_help_chapter get_absolute_file_path

