This demo shows how to estabilish a communication 
among a PC and the FLEX FULL board through a USB connection.

* The file "demoboard_example_flexside.cos" contains the schematic for the generation
  (with SCICOS) of the FLEX FULL board program. With this application the FLEX FULL 
  board is able to: 
  - acquire data from the acceleromter and from the light sensor;
  - send the measures to a PC using the USB interface;
  - receive and display the USB data on the LCD screen;
  - perform elaborations for a pretty LEDs blinking;
  
* The file "demoboard_example_pcside.cos" contains the schematic for the PC-side. 
  The almost-real-time simulation runs in SCICOS environment. The application 
  shows the received USB data on the multiple scope and sends other data to the 
  FLEX FULL board. The simulation requires the simultaneous execution of a program 
  named "app_flex_scicos.exe". To compile the executable use 
  the makefile and the source files located in the following folder: 
  - .\contrib\USB_UDP_FLEX_SCICOS_App

For other info go to the site: http://www.evidence.eu.com/

