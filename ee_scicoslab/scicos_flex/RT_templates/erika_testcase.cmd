[CCode,FCode]=gen_blocks();
[Code,Code_common]=make_standalonert();
files=write_code(Code,CCode,FCode,Code_common);
EE_get_diagram_info(rdnom,XX);
