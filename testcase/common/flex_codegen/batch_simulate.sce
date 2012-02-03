// Simulate blocks diagram

sim_results = [];

sim_results = scicos_simulate(scs_m,list(),'nw');

if sim_results ~= []

	[fd_sim,err] = mopen('simulate_log.txt', 'w');
	if err == 0
		mfprintf(fd_sim,"Simulation OK!\n");
		mclose(fd_sim);
	end

end



