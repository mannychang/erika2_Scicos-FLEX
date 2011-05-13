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
					B_L = [B_L; scs_m.objs(i).gui];
				end
				
				index = find(scs_m.objs(i).model.sim(1) == F_L)
				if(index==[]) then
					F_L = [F_L; scs_m.objs(i).model.sim(1)];					
				end

				//checking for symbol-associated Block
				index=find(gui_names == scs_m.objs(i).gui);
				if(index~=[]) then
					for k_index=1:size(index,2)
						symbol_to_insert = symbols(index(1,k_index));
						toks = tokens(symbol_to_insert,':');
						if(size(toks,1) > 2) then
							//disp(toks);
							chooser = asciimat(toks(1))-asciimat('0')
							//disp('Valore del parametro in questione:');
							//disp(scs_m.objs(i).model.ipar(chooser))
							symbol_to_insert = toks(scs_m.objs(i).model.ipar(chooser)+1)
							//disp('Simbolo da inserire:');
							//disp(symbol_to_insert);
						end
						index_kk = find(S_L == symbol_to_insert);
						if(index_kk==[]) then
							S_L = [S_L; symbol_to_insert];	
						end
					end
				end
			end
		end
	end
endfunction
