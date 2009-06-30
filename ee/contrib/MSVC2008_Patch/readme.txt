WHY
These macros update active detection and usage of Microsoft C++ 2008 for Scilab 4.1.2.
A C compiler is essential for Scicos internal CodeGeneration, stand alone compilation and
Modelica simulation.
A C compiler is also indispensable for some Scilab development, like creation of shared library
for new Scicos blocks, Scicos Hardware In the Loop and FLEX code generation.   


HOW-TO
- Please copy these macros in a folder
- Run Scilab
- go to the folder using 
     cd("<path_to_the_folder>");
- Run
     exec run_me.sce

INSTALL
This update is not "permanent": 
You need to run this update each time that you run Scilab. You can run this update in a 
automatic way adding the two command line (see "how-to") to the ".scilab" script file.
The".scilab" script is just another Scilab script that is automatically run each time
you start Scilab.

".scilab" is located in  

     C:\Documents and Settings\<user_name>\Scilab\scilab-4.1.2

Scicos-HIL and Scicos-FLEX users already have this file, so they can just add two line at the
bottom 

cd("<path_to_the_folder>");
exec run_me.sce ;

