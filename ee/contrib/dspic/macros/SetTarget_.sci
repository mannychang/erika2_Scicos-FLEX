function  SetTarget_()
//** INRIA / SCILAB / Roberto Bucher / Simone Mannori / Paolo Gai
//** 16 Jan 2008

//** Note: this code SHOULD be rewritten because respect the organization
//**       and style now obsolete. Future compatibility is not garantee.
  
//** ------------- Preliminary I/O section ___________________________________________________________________________
    k = [] ; //** index of the CodeGen source superbloc candidate

    xc = %pt(1); //** last valid click position 
    yc = %pt(2); 
    
    %pt = []   ;
    Cmenu = [] ;

    k  = getobj(scs_m,[xc;yc]) ; //** look for a block 
    //** check if we have clicked near an object
    if k==[] then
      return
    //** check if we have clicked near a block
    elseif typeof(scs_m.objs(k))<>"Block" then
      return
    end

    //** If the clicked/selected block is really a superblock 
    //**         <k>
    if scs_m.objs(k).model.sim(1)=="super" then
      
      disablemenus()
      lab = scs_m.objs(k).model.rpar.props.void3;
      //** Mark the super-block with a specific propriety
      if lab==[] then
	      lab = ['dspic','ode4','10'];
      end

      ode_x = ['ode1';'ode2';'ode4'];

      //** Open a dialog box and wait user interaction  
      while %t
        [ok, target, odefun, stp] = getvalue(..
                   "Embedded Code Generation ",..
                   ["Toolchain : ";
	                "ODE cont. function solver: ";
	                "Step between sampling: "],..
                    list('str',1,'str',1,'str',1),lab);
      if ~ok then
          break ; //** the the case of "cancel" exit 
      end

      TARGETDIR = SCI+"/contrib/RT_templates";

//      if exists("TARGET_DIR") then
//        [fd,ierr]=mopen(TARGET_DIR+'/'+target+'.gen','r');
//        if ierr==0 then
//	       TARGETDIR = TARGET_DIR;
//           mclose(fd);
//        end
//      end

      [fd,ierr] = mopen(TARGETDIR+'/'+target+'.gen','r');
      if ierr==0 then
        mclose(fd);
      else
        x_message("Target not valid "+target+".gen");
        ok = %f ;
      end
      
      if grep(odefun,ode_x) == [] then
         x_message("Ode function not valid");
         ok = %f;
      end
        
      if ok then
         lab = [target,odefun,stp];
	     scs_m.objs(k).model.rpar.props.void3 = lab;
         break ; //** EXIT 
      end
    end

    edited = %t ;
  
  enablemenus()
  else
   
    message("Generation Code only work for a Super Block ! ");
  
  end

endfunction

