// 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같�
// EE_search_SmcubeBlocks
// This function (recursive) returns the SMCube blocks list.
// Author: Dario Di Stefano, Evidence Srl
// 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같�

function [cpr, cor, xml_list, smb_id, smc_err] = EE_search_SmcubeBlocks(XX, cpr, cor, sbnum, xml_list, smb_id)

	smc_err = 0;
	scsm = XX.model.rpar;

	// recursive search...
	for i=1:size(scsm.objs)

		//Searching for 'Block' object
		if typeof(scsm.objs(i))=="Block" then   

			// if the block is a super-block restart...
			if scsm.objs(i).model.sim(1)=="super" then 
				//disp("super!!!");
				[cpr, cor, xml_list, smb_id, smc_err] = EE_search_SmcubeBlocks(scsm.objs(i), cpr, cor, sbnum + '('+string(i)+')', xml_list, smb_id);
				if smc_err ~= 0 then
					return;
				end
				continue; // to increment 'i'
			end

			// First condition: the length of the exprs field...
			iblock_exprs = scsm.objs(i).graphics.exprs(1); // take the exprs field
			//disp(iblock_exprs);
			ib_exp_size = size(iblock_exprs);
			if ib_exp_size(2) ~= 5 then
				continue; // This block cannot be a SMCUbe block...
			end

			// Second condition: the name of the block...
			if iblock_exprs(4) == "SMCube" then               // check if is a SMCube block
				if (scsm.objs(i).model.evtin ~= [] & scsm.objs(i).model.evtin > 1) then
					disp("#CodeGen error: Fatal Error");
					disp("    detected in EE_search_SmcubeBlocks, only one input event port is allowed in SMCube blocks!");
					smc_err = 1;
					return;
				end
				if scsm.objs(i).model.evtout ~= [] then
					disp("#CodeGen error: Fatal Error");
					disp("    detected in EE_search_SmcubeBlocks, output event ports are not allowed in SMCube blocks!");
					smc_err = 2;
					return;
				end
				
				[info_file,ierr] = fileinfo(iblock_exprs(3)); // Check if the XML file exists
				if ierr <> 0 then
					disp("#CodeGen error: Fatal Error");
					disp("#CodeGen error: SMCube file " + iblock_exprs(3) + " not found!"); 
					smc_err = 3;
					return;
				else
					if grep(xml_list, iblock_exprs(3))==[] then  // Check if this XML file is already in the list
						xml_list = [xml_list; iblock_exprs(3)];  // push the XML file in the list
					end
					smb_id = [smb_id; grep(xml_list, iblock_exprs(3))];

					//% Find the index to write the smb_id in the ipar vector 
					//% i: is the block index in the diagram
					//% cor(superblocks)(i): is the block index in the compiled structure 
					execstr('cmp_index = cor'+sbnum+'('+string(i)+')');
					//% write on the global copy, the pre-compiled structure of the scicos diagram ( used by make_standalonert() )
					cpr.sim.ipar(cpr.sim.ipptr(cmp_index)) = length(smb_id);  
				end
			end
		end
	end

	//disp("super END!!!");
endfunction

