* DC Motor PID demos *

DCMotor_pid.cos:
    Simple DC motor control example. 
    PID algorithm is implemented. 

DCMotor_pid_udp.cos:
    DC motor control tuning example. 
    PID algorithm can be tuned using "PCside_DCmotor_PIDtuning.cos" diagram.

DCMotors_pid.cos:
    Simple DC motor control example for 2 motors. 
    PID algorithm is implemented for both motors. 

DCMotors_pid_udp.cos: 
    DC motor control tuning example for 2 motors. 
    PID algorithm can be tuned using "PCside_DCmotor_PIDtuning.cos" diagram.

PCside_DCmotor_PIDtuning.cos: 
    This demo uses the ROLLER GUI console in this way: 
      Signal1: reference = 0, anti-windup = ON;
      Signal2: reference = square wave, anti-windup = OFF;
      Signal3: reference = square wave, anti-windup = ON;
      Roller: Kpx, Tix, Tdx used to tune PID 1 (if implemented);
      Roller: Kpy, Tiy, Tdy used to tune PID 2 (if implemented);

DCmotor_Simulation.cos:
    Simple DC Motor simulation demo that shows how to simulate a controller and how to 
    generate the code.

