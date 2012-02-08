// ***********************************************************
// * ScicosLab Pack Installer return script                  *
// * Requires: Scicoslab v4.4.1                              *
// * Built/Tested On: XP, Windows 7                          *
// * Description: Clear variables used in conditional        *
// *              instructions.                              *
// * Copyright (c) 2012 Evidence Srl                         *
// * Author: Dario Di Stefano, Evidence                      *
// ***********************************************************

	if exists('ans_inst_dialog_up')==1
		clear ans_inst_dialog ans_inst_dialog_up
	end

	if exists('visualc_path_up')==1
		clear  visualc_path visualc_path_up
	end

	if exists('vcansw_retry_up')==1
		clear vcansw_retry vcansw_retry_up
	end

	if exists('answ_c30_up')==1
		clear answ_c30 answ_c30_up
	end

	if exists('c30_asm30_paths_up')==1
		clear c30_asm30_paths c30_asm30_paths_up
	end

	if exists('answ_retry_up')==1
		clear answ_retry answ_retry_up
	end

	if exists('ans_old_inst_rm_up')==1
		clear ans_old_inst_rm ans_old_inst_rm_up
	end
	

	if exists('ans_inst_go_up')==1
		clear ans_inst_go ans_inst_go_up
	end

	if exists('answ_dot_scilab_up')==1
		clear answ_dot_scilab answ_dot_scilab_up
	end

	if exists('ans_reg_fix_up')==1
		clear ans_reg_fix ans_reg_fix_up
	end
