// the "cd" command below is expanded by the gcc preprocessor
//cd "c:/Evidence/scicoslab-44b7/testcase/mchp16_sources_7"
cd TESTDIR

// load the program to be tested
//load test.cos;
load THEFILENAME;

// reference function seems to load all functions in the .sci file, see Restrictions of http://www.scilab.org/product/man/genlib.html
x = FlexCodeGen_;

// load all the other libraries
load SCI/macros/scicos/lib;
exec(loadpallibs,-1);

// search for the block tagged with the right ID. If found, call the code generator
ID='cg'
for i=1:size(scs_m.objs)
  obj=scs_m.objs(i);
  if typeof(obj)=='Block' & obj.graphics.id==ID then
    blocks_list = [];
    symbols_list = [];
    functions_list = [];
    txt = [];

    [blocks_list,functions_list,symbols_list] = EE_analyze_superblocks(obj,blocks_list,functions_list,symbols_list);

    for i=1:size(blocks_list,1)
      txt = [txt; TESTCASE+" "+blocks_list(i)];
    end
				
    mputl(txt, 'blocklist.txt');	
  end
end
quit




