## the code generator run from the ScicosLab graphical interface at the
## end executes, into erika.cmd, a command named impl_dspicf(). The
## command basically copies a few files, and finally calls the
## compile.bat scriptfile inside a Windows terminal window.

## the compile.bat script receives as parameter the RT-Druid template
## name. The compile.bat file basically creates a "Cygwin" environment
## executing bash, and then run the compile.sh passing the RT-Druid
## tempalte name as parameter.

## when we run the testcases, we want to execute exactly the same but
## always staying in a Cygwin environment (we do not want to have the
## creation of the Windows terminal window). Moreover, the RT-Druid
## template name is passed to the scripts using the MUX/DEMUX things,
## and for that reason the test.mk cannot call directly compile.sh with
## the right parameter.

## for this reason, test.mk takes this file, parses it using the gcc
## preprocessor, and creates a script file in the target
## directory. then, it executes this file in the target directory.

#ifdef flex
./compile.sh ${SCIBASE}/contrib/scicos_ee/RT-Druid board_flex && touch ../doneflag.txt
#endif

#ifdef easylab
./compile.sh ${SCIBASE}/contrib/scicos_ee/RT-Druid board_easylab && touch ../doneflag.txt
#endif

exit 0
