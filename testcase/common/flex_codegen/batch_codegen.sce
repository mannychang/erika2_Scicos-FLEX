// the "cd" command below is expanded by the gcc preprocessor
cd TESTDIR

// load the program to be tested
load test.cos;

// reference function seems to load all functions in the .sci file, see Restrictions of http://www.scilab.org/product/man/genlib.html
x = FlexCodeGen_;

// load all the other libraries
load SCI/macros/scicos/lib;
exec(loadpallibs,-1);

// set the parameters which are usually asked in the code generator popup window
#ifdef BOARDFLEX
TARGETBOARD = 'board_flex';
TARGETARCH = 'dspic_testcase';
TERGETNAME = 'test';
TARGETPATH = getcwd()+'/test_scig';
#endif

// set the parameters which are usually asked in the code generator popup window
#ifdef BOARDEASYLAB
TARGETBOARD = 'board_easylab';
TARGETARCH = 'dspic_testcase';
TERGETNAME = 'test';
TARGETPATH = getcwd()+'/test_scig';
#endif

// search for the block tagged with the right ID. If found, call the code generator
ID='cg'
for i=1:size(scs_m.objs)
  obj=scs_m.objs(i);
  if typeof(obj)=='Block' & obj.graphics.id==ID then
    needcompile=4;
    ALL = %f;      //@@ entire diagram generation
    with_gui = %t; //@@ entire diagram generation with gui
    scs_m_top = obj;
    k = i; 
    do_compile_superblock42(scs_m, k, TARGETBOARD, TARGETARCH, TERGETNAME, TARGETPATH, '', '', 'testcase');
 end
end
quit

