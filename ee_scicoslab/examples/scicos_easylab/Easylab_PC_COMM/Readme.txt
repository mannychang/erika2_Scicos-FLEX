This demo shows how to estabilish a communication among a PC and Easylab board 
through a serial over USB connection.

N.B all path referenced in this file are relative paths to the base directory 
of scicos pack

# EASYLAB SIDE
* Easylab_PC_COMM.cosf
    contains the schematic to generate a simple program that send the read of ADC 1 and
    do the echo of whatever it receive from serial channel 1 to the serial it self. To have 
    a reference value to read with ADC ,PWM 1 is activated with a duty cycle equal to 0,5. 
    Use a cable to connect pin AN1 with pin AO1 to use this reference.
    
    You can use some MCP2200 pins to select if activate or not the buzzer or for swith second 
    serial channel to a generated sinus.
    
    USB 0 should be wired on DI 1 (to command output switch beetween generated sinus on board 
    and ADC1 read).
    
    USB 1 should be wired on DI 2 (to command the buzzer).
    
    You can wire any DO to USB 3 to monitoring it on PC.
    
# PC SIDE
* Easylab_PC_COMM_Monitor.cosf
    With this schematic you can plot values sent by Easylab. Acquisition and Sending UDP blocks 
    rely on an external application called EasylabSerialUDPGateway (you find the executable
    in scicos_flex/EasylabSerialUDPGateway) that convert serial communication to UDP packets.
    To establish communication run this application, choose the right COM port and click connect.
    The same thing is done automatically by "Serial Gateway Block".
       
    To excite the 'echo' behaviour of Easylab.cos application is used a sin function sent trough
    serial channel 1
    
    Change constant value as second input of MCP2200 to 1.0 block if you want enable Buzzer.
    
For other info go to the site: http://erika.tuxfamily.org/wiki