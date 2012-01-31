mode(-1)
path=get_absolute_file_path('builder.sce');

myhelps=[path+'amazing', "Amazing Ball Blockset";..
         path+'flex', "Flex Blockset";..
         path+'flex_dmb', "Flex-DemoBoard Blockset";..
         path+'flex_mtb', "Flex-MotionBoard Blockset";..
         path+'rt_data_exchange', "RT Data Exchange Blockset";..
         path+'flex_com', "Flex-Communication Blockset";..
         path+'misc', "MCHP16-Sources Blockset";..
         path+'easylab', "EasyLab Blockset";..
         path+'miscellaneous', "Miscellaneous Blockset";..
         path+'smcube', "SMCube Blockset"];

// process the xml files to produce html files 
// update %helps for cross reference
%helps_save=%helps
%helps=[%helps;myhelps] 
xmltohtml(myhelps(:,1),myhelps(:,2));

//restore the previous help table
%helps=%helps_save
//erase temporary variables
clear %helps_save xmltohtml myhelps path get_absolute_file_path
