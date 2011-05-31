@echo off

xcopy %1\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\AMAZING\AMAZING.cosf %1\contrib\scicos_ee\scicos_flex\dspic\macros\palettes /s /e /y /i
xcopy %1\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX\FLEX.cosf %1\contrib\scicos_ee\scicos_flex\dspic\macros\palettes /s /e /y /i
xcopy %1\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-Communication\FLEX-Communication.cosf %1\contrib\scicos_ee\scicos_flex\dspic\macros\palettes /s /e /y /i
xcopy %1\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-DMB\FLEX-DMB.cosf %1\contrib\scicos_ee\scicos_flex\dspic\macros\palettes /s /e /y /i
xcopy %1\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-MTB\FLEX-MTB.cosf %1\contrib\scicos_ee\scicos_flex\dspic\macros\palettes /s /e /y /i
xcopy %1\contrib\scicos_ee\scicos_flex\dspic\macros\flex_blocks\FLEX-PC\FLEX-PC.cosf %1\contrib\scicos_ee\scicos_flex\dspic\macros\palettes /s /e /y /i
exit