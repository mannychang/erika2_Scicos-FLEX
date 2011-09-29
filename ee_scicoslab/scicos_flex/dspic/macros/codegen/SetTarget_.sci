function  SetTarget_()
//** INRIA / METALAU /Roberto Bucher / Simone Mannori / Paolo Gai
//** 29 Sept 2011
 
//** ------------- Preliminary I/O section -----------------------
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
      //** field "scs_m.objs(k).model.rpar.props.void3" is used to
      //** store the proprieties ; 
      //** <cross compilation chain> 'dspic'
      //** <fixed step solver>       'ode4'
      //** <internal steps>          '10'
      //** <target board template>   'board_flex'
      
      //** Mark the super-block with a specific propriety
      if lab==[] then
	      lab = ['dspic','ode4','10', 'board_flex'];
      end

      //** back compatibility with old diagrams
      if or(size(lab)==[1 3])  then
	      lab = [lab, 'board_flex'];
      end

      ode_x      = ['ode1';'ode2';'ode4'];
      template_x = ['board_flex';'board_easylab'];

      //** Open a dialog box and wait user interaction  
      while %t
        [ok, target, odefun, stp, template] = getvalue("Embedded Code Generation ",..
                                   ["Toolchain : ";
	                              "ODE cont. function solver: ";
	                              "Step between sampling: ";
                                    "Target Board: "  ],..
                                   list('str',1,'str',1,'str',1,'str',1),lab);

      if ~ok then
          break ; //** the the case of "cancel" exit 
      end

      TARGETDIR = SCI+"/contrib/scicos_ee/scicos_flex/RT_templates";

      [fd,ierr] = mopen(TARGETDIR+'/'+target+'.gen','r');
      if ierr==0 then
        mclose(fd);
      else
        message("Target not valid "+target+".gen");
        ok = %f ;
      end
     
      if grep(odefun, ode_x) == [] then
         message("ODE function not valid");
         ok = %f;
      end

      if evstr(stp)<3 then
         message("At least 3 steps are required for minimum ODE convergence");
         ok = %f;
      end

      if grep(template, template_x) == [] then
         message("Target Board not valid");
         ok = %f;
      end

      if ok then
         lab = [target, odefun, stp, template];
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

