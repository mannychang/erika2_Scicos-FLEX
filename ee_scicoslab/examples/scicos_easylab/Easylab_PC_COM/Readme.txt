This demo shows how to estabilish a communication among a PC and Easylab board 
through a serial over USB connection.

N.B all path referenced in this file are relative paths to the base directory 
of scicos pack

# EASYLAB SIDE
* Easylab_PC_COM.cos
    contains the schematic to generate a simple program that send the read of ADC 1 and
    do the echo of whatever it receive from serial channel 1 to the serial it self. To have 
    a reference value to read with ADC ,PWM 1 is activated with a duty cycle equal to 0,5. 
    Use a cable to connect pin AN1 with pin AO1 to use this reference.
  
# PC SIDE
* Easylab_PC_COM_Monitor.cos
    With this schematic you can plot values sent by Easylab. Acquisition and Sending UDP blocks 
    rely on an external application called EasylabSerialUDPGateway (you find the executable
    in scicos_flex/EasylabSerialUDPGateway) that convert serial communication to UDP packets.
    To establish communication run this application, choose the right COM port and click connect.
       
    To excite the 'echo' behaviour of Easylab.cos application is used a sin function sent trough
    serial channel 1
    
  	
For other info go to the site: http://erika.tuxfamily.org/wiki