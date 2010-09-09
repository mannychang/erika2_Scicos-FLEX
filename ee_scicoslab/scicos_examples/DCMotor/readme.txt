* DC Motor PID demos *

Flex-side diagram:	DCMotor_pid_udp.cos
PC-side diagram: 	PCside_DCmotor_PIDtuning.cos

This demo uses the ROLLER GUI console in this way: 
Signal1: reference = 0, anti-windup = ON;
Signal2: reference = square wave, anti-windup = OFF;
Signal3: reference = square wave, anti-windup = ON;
Roller: Kpx, Tix, Tdx used to tune DC motor PID controller;
Roller: Kpy, Tiy, Tdy unused;


