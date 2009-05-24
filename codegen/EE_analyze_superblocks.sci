function [B_L,F_L,S_L] = EE_analyze_superblocks(XX,B_L,F_L,S_L)
 
	//disp("We are in " + XX.gui);
	SYMBOLS_DIR = SCI+'/contrib/dspic';
	WORK_BLOCKS_DIR = SCI+'/contrib/dspic';

	work_blocks = mgetl(WORK_BLOCKS_DIR + '/work_blocks');
	symbols = mgetl(SYMBOLS_DIR + '/symbols');

	for i=1:size(symbols,1)
		tmp = tokens(symbols(i),',');
		gui_names(i) = tmp(1);
		symbols(i) = tmp(2);
	end

	scs_m = XX.model.rpar;
	
	//disp(size(scs_m.objs));
	
	for i=1:size(scs_m.objs)
		//checking for Block object
		if typeof(scs_m.objs(i))=="Block" then     
		
			if scs_m.objs(i).model.sim(1)=="super" then 
				//disp("We have the superblock:" + scs_m.objs(i).gui);		

				[B_L,F_L,S_L] = EE_analyze_superblocks(scs_m.objs(i),B_L,F_L,S_L);
				continue;

			end	
		
			//Block
			//checking for non-working block
			
			//disp("we have the block: " + scs_m.objs(i).gui)
			index=find(work_blocks == scs_m.objs(i).gui);
			if(index==[]) then

				//TODO: take decision about block symbol due to blocks parameters
				
				//checking for redundant block
				index=find(B_L == scs_m.objs(i).gui)
				if(index==[]) then
					//Block not already present in list

					// disp("And it''s not redundant!");
					B_L = [B_L; scs_m.objs(i).gui];
	
					index = find(scs_m.objs(i).model.sim(1) == F_L)
					if(index==[]) then
						F_L = [F_L; scs_m.objs(i).model.sim(1)];					
					end

					//checking for symbol-associated Block
					index=find(gui_names == scs_m.objs(i).gui);
					if(index~=[]) then
					
						for k_index=1:size(index,2)
						
							index_kk = find(S_L == symbols(index(1,k_index)));
							
							//disp("finding " + symbols(index(1,k_index)) + " in");
							//disp(S_L);
							//disp("found in ");
							//disp(index_kk);
							
							if(index_kk==[]) then												
							
								S_L = [S_L; symbols(index(1,k_index))];	
								
							end
						end
					end
				end
			end
		end
	end
endfunction
