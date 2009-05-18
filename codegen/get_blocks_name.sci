function [XX] = get_blocks_name(name,XX)

	scs_m = XX.model.rpar;
	
	SYMBOLS_DIR = SCI+'/contrib/dspic';
	WORK_BLOCKS_DIR = SCI+'/contrib/dspic';

	work_blocks = mgetl(WORK_BLOCKS_DIR + '/work_blocks');
	symbols = mgetl(SYMBOLS_DIR + '/symbols');

	for i=1:size(symbols,1)
		tmp = tokens(symbols(i),',');
		gui_names(i) = tmp(1);
		symbols(i) = tmp(2);
	end

	disp(symbols);

	no_malloc_symbol = "__NO_MALLOC__";
	
	// blocks list (redundant)
	blocks_list_r = [];	
	
	// blocks list (non-redundant)
	blocks_list = [];
	blocks_list_mk = [];
	
	// functions list
	functions_list = [];
	
	// symbols list 
	symbols_list = [];
	symbols_list_mk = [];
	  
	disp(size(scs_m.objs));

	// **  blocks scanning
	for i=1:size(scs_m.objs)
		// checking for Block object
		if typeof(scs_m.objs(i))=="Block" then     
			// Block
			// checking for non-working block
			
			//disp("we have a block!!"  + scs_m.objs(i).gui)

			index=find(work_blocks == scs_m.objs(i).gui);
			if(index==[]) then
				// non-work Block
				blocks_list_r = [blocks_list_r; scs_m.objs(i).gui];
				
				// TODO: take decision about block symbol due to blocks parameters
				
				// checking for redundant block
				index=find(blocks_list == scs_m.objs(i).gui)
				if(index==[]) then
					// Block not already present in list

					disp("new block! save it! " + scs_m.objs(i).gui);

					blocks_list = [blocks_list; scs_m.objs(i).gui];
					functions_list = [functions_list; scs_m.objs(i).model.sim(1)];					


					// checking for symbol-associated Block
					index=find(gui_names == scs_m.objs(i).gui)
					if(index~=[]) then

						disp("symbol!! save it!! " + scs_m.objs(i).gui + symbols(index));
						
						// symbol-associated block
						symbols_list = [symbols_list; symbols(index)];	

					end
				end
			end
		end
	end

	//disp("A");

	if(blocks_list~=[]) then

		// tailing C extension to block_list
		// blocks_list = 'sci_' + blocks_list + '.c';
		blocks_list = functions_list + '.c';
		blocks_list_mk = blocks_list;
		blocks_list_mk(1) = 'EE_SRCS_SCICOS := ' + blocks_list_mk(1);
		blocks_number = size(blocks_list_mk);
		for i=1:blocks_number(1)
			if(i~=1) then
				blocks_list_mk(i) = '		' + blocks_list_mk(i);
			end
			if(i~=blocks_number(1)) then
				blocks_list_mk(i) = blocks_list_mk(i) + ' \';
			end
		end
	end
	  
	//disp("B");

	symbols_number = size(symbols_list,1);

	if(symbols_list~=[]) then
		// not empty symbols_list
		symbols_list_mk = symbols_list;
		symbols_list_mk(1) = 'EEOPT += ' + symbols_list_mk(1); 
		symbols_number = size(symbols_list_mk,1);
		for i=1:symbols_number
			if(i~=1) then
				symbols_list_mk(i) = '		' + symbols_list_mk(i);
			end
			if(i~=symbols_number) then
				symbols_list_mk(i) = symbols_list_mk(i) + ' \';
			end
		end
	end

	
	//disp("C");

	scicos_symbols_h = "-D" + "MODELNAME=" + name;

	// TODO: write test to dyn mem

	scicos_symbols_h = scicos_symbols_h + " -D" + no_malloc_symbol; 

	for i=1:symbols_number
		scicos_symbols_h = scicos_symbols_h + " -D" + symbols_list(i);
	end

	//disp("D");

	app_mk = ["APP_SRCS += " + name + ".c";
						"EEOPT += " + "MODELNAME=" + name;
						"EEOPT += " + no_malloc_symbol;
						symbols_list_mk;
						blocks_list_mk ];
	
	//disp(scicos_symbols_h);		
	//disp(app_mk);
	
	mputl(app_mk,rpat + '/' + 'app.mk'); 
	
	mputl(scicos_symbols_h,rpat + '/' + 'scicos_symbols');
	
endfunction	
