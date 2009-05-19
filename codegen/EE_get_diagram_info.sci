function EE_get_diagram_info(name,XX)

	blocks_list = [];
	symbols_list = [];
	functions_list = [];

	blocks_list_mk = [];
	symbols_list_mk = [];
	app_mk = [];

	txt =  ["Compilation optimization";
					"Enjoy";
					"";
					"Current directory: " + pwd();
					""];

	FUNCS_DEP_DIR = SCI+'/contrib/dspic';

	txt = [txt;
				"Loading functions dependencies file from " + FUNCS_DEP_DIR + "/functions_dependencies..."];

	funcs_dep = mgetl(FUNCS_DEP_DIR + '/functions_dependencies');
	
	for i=1:size(funcs_dep,1)
		tmp = tokens(funcs_dep(i),',');
		present(i) = tmp(1);
		necessary(i) = tmp(2);
	end

	txt = [txt;
				"Blocks dependencies file loaded successfully.";
				"";
				"Analyzing the superblock..."];

	[blocks_list,functions_list,symbols_list] = EE_analyze_superblocks(XX,blocks_list,functions_list,symbols_list);

	txt = [txt;
				"Superblock analyzed successfully.";
				"Functions list follows:";
				""];

	for i=1:size(functions_list,1)
		
		txt = [txt;
					string(i) + ": " + functions_list(i)];

	end

	txt = [txt;
				"";
				"Checking dependencies..."];

	for i=1:size(present,1)

		txt = [txt;
					"Function in analysis: " + present(i)];


		// Searching for a block which needs another source from the static list
		index = find(present(i) == functions_list)

		if(index~=[]) then
			// Check if the necessary block is already in our list
			
			txt = [txt;
						"This one needs: " + present(i)];

			index = find(necessary(i) == functions_list)

			if(index==[]) then

				txt = [txt;
							"and we need it because " + necessary(i) + " source is not already in our list."];
				// Adding necessary block to out list
				functions_list = [functions_list; necessary(i)];

			end
		end	
	end

	txt = [txt;
				"";	
				"Functions list follows:";
				""];

	for i=1:size(functions_list,1)
		
		txt = [txt;
					string(i) + ": " + functions_list(i)];

	end

	DYN_MEM_BLOCKS_DIR = SCI+'/contrib/dspic';

	txt = [txt;
				"";
				"Dependencies checking finished.";
				"";
				"Dynamic memory checking...";
				"";
				"Loading dynamic memory needing blocks file from " + DYN_MEM_BLOCKS_DIR + "/dynmem_blocks..."];

	dynmem_blocks = mgetl(DYN_MEM_BLOCKS_DIR + '/dynmem_blocks');

	txt = [txt;
				"";
				"Dynamic memory needing blocks file loaded successfully."];

	no_malloc_symbol = "__NO_MALLOC__";

	dynmem_flag = 1;

	for i=1:size(dynmem_blocks,1)

		// Searching for a block which needs dynamic memory
		index = find(dynmem_blocks(i) == functions_list)

		if(index~=[]) then

			txt = [txt;
						"";
						"Awfully, " + dynmem_blocks(i) + " needs dynamic memory; avoiding " + no_malloc_symbol + "symbol creation.";
						"Search interrupted."];

			dynmem_flag = 0;

			break	
		end	
	end

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

	scicos_symbols_h = "-D" + "MODELNAME=" + name;

	if(dynmem_flag == 1) then
		scicos_symbols_h = scicos_symbols_h + " -D" + no_malloc_symbol; 
		app_mk = ["EEOPT += " + no_malloc_symbol];
	end


	for i=1:symbols_number
		scicos_symbols_h = scicos_symbols_h + " -D" + symbols_list(i);
	end

	app_mk = [app_mk;
						"EEOPT += " + "MODELNAME=" + name;
						symbols_list_mk;
						blocks_list_mk;
						"APP_SRCS += " + name + ".c"];

	txt = [txt;
				 "scicos_symbol.h:";
				 "";
					scicos_symbols_h;		
				 "app.mk:";
				 "";
					app_mk];

	mputl(txt,rpat + '/analysis');	
	mputl(app_mk,rpat + '/app.mk'); 
	mputl(scicos_symbols_h,rpat + '/scicos_symbols');
	
endfunction	
