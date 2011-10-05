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

    lab_tool =     ['dspic'];
    lab_template = ['board_flex', 'board_easylab'];
    lab_ode =      ['ode1', 'ode2', 'ode4'];
    lab_step =     ['5', '10', '20'];

    //** If the clicked/selected block is really a superblock 
    //**         <k>
    if scs_m.objs(k).model.sim(1)=="super" then
      
      disablemenus()
      lab3 = scs_m.objs(k).model.rpar.props.void3;
      lab2 = scs_m.objs(k).model.rpar.props.void2;

      //** field "scs_m.objs(k).model.rpar.props.void3" is used to store the proprieties ; 
      //** <cross compilation chain> 'dspic'
      //** <fixed step solver>       'ode4'
      //** <internal steps>          '10'
      //** <target board template>   'board_flex'
      
      //** field "scs_m.objs(k).model.rpar.props.void2" is used to store the name and the path ; 
      //** <block_title>             'Superblock'
      //** <project folder path>     'C:\...'
      
      //** Mark the super-block with a specific propriety
      if lab3==[] then
          mytarget   = 'dspic'; //** default compilation chain 
          mytemplate = 'board_flex'; //** default values for this version  
          myodefun   = 'ode4';  //** default solver 
          myodestep  = '10';   //** default continous step size
      else
          //** back compatibility with old diagrams
          if or(size(lab3)==[1 3])  then
            lab3 = [lab3, 'board_flex'];
          else
            mytarget  =  lab3(1); //** user defined parameters 
            myodefun  =  lab3(2);
            myodestep =  lab3(3);
            mytemplate = lab3(4);
          end
      end

      if lab2 == []
          myrdnom  =   scs_m.objs(k).model.rpar.props.title(1); 
          mypath =     getcwd()+'/'+myrdnom+"_scig";
      else
          myrdnom =    lab2(1);
          mypath =     lab2(2);
      end
  
      user_tool =     vectorfind(lab_tool,     mytarget,   'c');
      user_template = vectorfind(lab_template, mytemplate, 'c');
      user_ode =      vectorfind(lab_ode,      myodefun,  'c');
      user_step =     vectorfind(lab_step,     myodestep,  'c');

      //** Open a dialog box and wait user interaction  
        //  [ok, target, odefun, stp, template, new_user_wdir] = getvalue("Embedded Code Generation ",..
        //                             ["Toolchain : ";
        //                              "ODE cont. function solver: ";
        //                              "Step between sampling: ";
        //                              "Target Board: ";
        //                              "Created files Path: "],..
        //                             list('str',1,'str',1,'str',1,'str',1,'str',1), lab);
        // if ~ok then
        //    break ; //** the case of "cancel" exit 
        // end

        l_tool =     list('Toolchain : ',                                    user_tool,     lab_tool);
        l_template = list('Target Board : ',                                 user_template, lab_template);
        l_ode =      list('ODE cont. function solver : ',                    user_ode,      lab_ode);
        l_step =     list('Step between sampling (for time cont. blocks): ', user_step,     lab_step);

        user_choices = x_choices('Code Generation Target Settings', list(l_tool, l_template, l_ode, l_step));
        if user_choices==[] then return; end

        utarget =   lab_tool(user_choices(1));
        utemplate = lab_template(user_choices(2));
        uodefun =   lab_ode(user_choices(3));
        ustp =      lab_step(user_choices(4));
        
        scs_m.objs(k).model.rpar.props.void3(1) = utarget;
        scs_m.objs(k).model.rpar.props.void3(2) = uodefun;
        scs_m.objs(k).model.rpar.props.void3(3) = ustp;
        scs_m.objs(k).model.rpar.props.void3(4) = utemplate;

        scs_m.objs(k).model.rpar.props.void2(1) = myrdnom;
        scs_m.objs(k).model.rpar.props.void2(2) = mypath;
 
        edited = %t ;
        enablemenus()

  else
   
    message("Generation Code only work for a Super Block ! ");
  
  end

endfunction

