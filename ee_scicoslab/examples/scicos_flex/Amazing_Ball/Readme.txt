This folder contians the following demo files.

# FLEX SIDE
* servo_calibration.cos
	With this schematic you can generate code for the FLEX FULL/LIGHT Motion board 
	to rotate the Amazing Ball servos shafts to the zero angle position. 
	This is useful because you can remove the external shafts to adjust the horizontal 
	attitude of the plane.	

* pid_ctrl_usbudp_codegen.cos
	With this schematic you can generate code for the FLEX FULL/LIGHT Motion board
	to implement two simple control algorithms (PID-like) to bring the ball
	of the Amazing Ball system in the central position on the touch panel. 
	Data are sent using USB protocol.

* pid_ctrl_codegen_usbudp_tuning_square_circle.cos
	With this schematic you can generate code for the FLEX FULL Motion board
	to implement two simple algorithms (PID-like) to control position of the
	ball on the touch panel. You can also tune the PIDs coefficients and watch 
	simulation data using the pc_pid_usbudp_tuning.cos file (see below). You can also test
	the control system performance on circular/rectangular trajectory tracking.

* pid_ctrl_udp_codegen.cos
	With this schematic you can generate code for the FLEX FULL/LIGHT Motion board
	to implement two simple control algorithms (PID-like) to bring the ball
	of the Amazing Ball system in the central position on the touch panel. 
	Data are sent using UDP protocol.

* pid_ctrl_codegen_udp_tuning_square_circle.cos
	With this schematic you can generate code for the FLEX FULL Motion board
	to implement two simple algorithms (PID-like) to control position of the
	ball on the touch panel. You can also tune the PIDs coefficients and watch 
	simulation data using the pc_pid_udp_tuning.cos file (see below). You can also test
	the control system performance on circular/rectangular trajectory tracking.

# PC SIDE
* pc_pid_usbudp_tuning.cos
	With this schematic you can run an almost-real-time simulation in SCICOS environment
	to watch the behavior of the control system and tune the algorithm coefficients
	to increase its performance. 
	The application shows the received USB data on the multiple scope and sends the new 
	parameters to the FLEX FULL board. The simulation requires the simultaneous execution 
	of a program named "app_flex_scicos.exe". To compile the executable use the makefile 
	and the source files located in the following folder: 
  	- .\contrib\flex_usb2udp_gateway

* PCside_AmazingBall_UDP_PIDtuning.cos
	With this schematic you can run an almost-real-time simulation in SCICOS environment
	to watch the behavior of the control system and tune the algorithm coefficients
	to increase its performance. 
	The application shows the received UDP data on the multiple scope and sends the new 
	parameters to the Amazing Ball. The simulation requires PC ethernet port settings.

* PCside_AmazingBall_UDP_Monitor.cos
	With this schematic you can run an almost-real-time simulation in SCICOS environment
	to watch the behavior of the control system.
	The application shows the received UDP data on the multiple scope.
	The simulation requires PC ethernet port settings.

For other info go to the site: http://www.evidence.eu.com/

