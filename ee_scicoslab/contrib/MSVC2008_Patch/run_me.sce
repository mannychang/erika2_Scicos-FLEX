//** 20/03/2008 : Scilab 4.1.2 update for Visual C++ 2008 detection and usage
//** script file for Visual C++ 2008 
exec ("configure_msvc.sci", -1);
exec ("findmsvccompiler.sci", -1);
exec ("haveacompiler.sci", -1);
exec ("ilib_for_link.sci", -1);
exec ("ilib_gen_Make.sci", -1);
//**
if configure_msvc() then
  disp("Scilab has detected MSCV Express 2008 compiler on your system.")
else
  disp("Sorry: Scilab has NOT found compatible C compiler");
  disp("on your system. Please check the Scilab Wiki page ");
  disp("www.scilab.org")
end   

