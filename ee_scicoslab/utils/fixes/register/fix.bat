@echo off
xcopy %1\contrib\scicos_ee\utils\fixes\register\REGISTER.sci %1\macros\scicos_blocks\Linear /s /e /y /i
xcopy %1\contrib\scicos_ee\utils\fixes\register\REGISTER.bin %1\macros\scicos_blocks\Linear /s /e /y /i
exit