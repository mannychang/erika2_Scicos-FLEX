function [XX] = get_blocks_name(XX)

	scs_m = XX.model.rpar;
	
	//disp(XX);
	
	//disp(cpr.sim.funs);

	work_blocks = [	"EVTGEN_f";
					"CLKSPLIT_f";
					"SPLIT_f"
					"CLKINV_f"];
	   
	symbols =	[	"FLEX_led","__USE_LEDS__";
					"FLEX_usb_read","__USE_USB__";
					"FLEX_usb_write","__USE_USB__"];

	// blocks list (redundant)
	blocks_list_r = [];	
	
	// blocks list (non-redundant)
	blocks_list = [];
	
	// functions list
	functions_list = [];
	
	// symbols list 
	symbols_list = [];
	  
	// **  blocks scanning
	for i=1:size(scs_m.objs)
		// checking for Block object
		if typeof(scs_m.objs(i))=="Block" then     
			// Block
			// checking for non-working block
			index=find(work_blocks == scs_m.objs(i).gui);
			if(index==[]) then
				// non-work Block
				blocks_list_r = [blocks_list_r; scs_m.objs(i).gui];
				
				// TODO: take decision about block symbol due to blocks parameters
				
				// checking for redundant block
				index=find(blocks_list == scs_m.objs(i).gui)
				if(index==[]) then
					// Block not already present in list
					blocks_list = [blocks_list; scs_m.objs(i).gui];
					functions_list = [functions_list; cpr.sim.funs(i)];					
					// checking for symbol-associated Block
					index=find(symbols == scs_m.objs(i).gui)
					if(index~=[]) then
						// symbol-associated block
						symbols_list = [symbols_list; symbols(index,2)];	
					end
				end
			end
		end
	end

	if(blocks_list~=[]) then
		// tailing C extension to block_list
		
		blocks_list = 'sci_' + blocks_list + '.c';
		//blocks_list = blocks_list + '.c';
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
	  
	if(symbols_list~=[]) then
		// not empty symbols_list
		symbols_list_mk = symbols_list;
		symbols_list_mk(1) = 'EEOPT += ' + symbols_list_mk(1); 
		symbols_number = size(symbols_list_mk);
		for i=1:symbols_number(1)
			if(i~=1) then
				symbols_list_mk(i) = '		' + symbols_list_mk(i);
			end
			if(i~=symbols_number(1)) then
				symbols_list_mk(i) = symbols_list_mk(i) + ' \';
			end
		end
	end
	
	disp('functions list is:');
	disp(functions_list+'.c');
	
	app_mk = [
				blocks_list_mk;
				symbols_list_mk  
			];
			
	mputl(blocks_list,rpat + '/' + 'app.txt');
	mputl(app_mk,rpat + '/' + 'app.mk'); 
	
endfunction	