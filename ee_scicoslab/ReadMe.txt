# ScicosLab Pack

- contrib: Must be placed inside the ScicosLab 4.4b7 folder.
	   To compile FLEX block scripts type this commands:
	   cd C:\Program Files\scicoslab-44b7\contrib\dspic 	(you may have to change the path...)
	   exec builder.sce

- firmware: The precompileed firmware for the Pic18 MCU to enable USB communication.

- scicos_examples: A basic set of examples to test the Flex board.

- user: It contains the .scilab config file that needs to be placed in the user folder. 
	(example path: C:\Users\user_name\Scilab\4.4b7)

- common_oil.pref: The ERIKA makefiles tries to automatically detect the Microchip MPLAB 
installation directory. If the makefile does not find the compiler or assembler, please 
edit this file with the correct paths and put it into 
   c:\evidence\eclipse\configuration
to force a specific default directory.