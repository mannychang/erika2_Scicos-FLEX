[CCode,FCode]=gen_blocks();
[Code,Code_common]=make_standalonert();
files=write_code(Code,CCode,FCode,Code_common);
imp_dspicf(rpat,template);
EE_get_diagram_info(rdnom,XX);
