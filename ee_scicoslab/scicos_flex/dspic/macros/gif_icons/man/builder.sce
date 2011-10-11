mode(-1)
path=get_absolute_file_path('builder.sce');

myhelps=[path+'amazing', "Amazing Ball"];

// process the xml files to produce html files 

// update %helps for cross reference
%helps_save=%helps
%helps=[%helps;myhelps] 
//xmltohtml(myhelps(:,1),myhelps(:,2));
xmltohtml(myhelps(:,1));

//restore the previous help table
%helps=%helps_save
//erase temporary variables
clear %helps_save xmltohtml myhelps path get_absolute_file_path
