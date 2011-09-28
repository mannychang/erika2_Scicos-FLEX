This folder contains the sources for the generation of the
executable "app_flex_scicos.exe", required by the SCICOS 
schematics that work with a USB connection. 
For further info go to the site: http://www.evidence.eu.com/
To compile the executable use the makefile located in this folder.

Requirements:
Please install CygWin and the required libraries before the compilation.
The program needs CygWin Win32 usb library ("libusb-win32").
Donwload and install "libusb-win32" using the utility "Setup.exe", the
download is available from the Cygwin site (Cygwin Net Release Setup program).
After the program "app_flex_scicos.exe" is builded (using "make"), copy cygusb0.dll and cygwin1.dll 
into the program folder before to launch it.

Usage:
The program must be executed in a command prompt window, 
either the Microsoft Windows one or the CygWin one, 
before starting the SCICOS simulation. 
