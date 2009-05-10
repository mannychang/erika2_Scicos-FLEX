//** INRIA / SCILAB / Roberto Bucher / Simone Mannori / Paolo Gai
//** 16 Jan 2008
//**
// Modified by Roberto Bucher roberto.bucher@supsi.ch from the original
// CodeGeneration_.sci
//
// Input editor function of Scicos code generator
//
//
//** 10 Set 2007 : cleaner startup code by Simone Mannori

function FlexCodeGen_()

//** Code execution tracing and profiling 
global %tb ;
%tb = ["FlexCodeGen"]; 
//** 


//** ------------- Preliminary I/O section ___________________________________________________________________________
    k = [] ; //** index of the CodeGen source superbloc candidate

    xc = %pt(1); //** last valid click position 
    yc = %pt(2); 
    disp(%pt); 
    
    
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
      
        XX = scs_m.objs(k); //** isolate the super block to use 
        
//---------------------------------------------------->       THE REAL CODE GEN IS HERE --------------------------------
        //** the real code generator is here 
        [ok, XX, alreadyran, flgcdgen, szclkINTemp, freof] =  do_compile_superblock42(XX, scs_m, k, alreadyran);
 
        //**quick fix for sblock that contains scope
        gh_curwin = scf(curwin)
    
    else
      //** the clicked/selected block is NOT a superblock 
      message("Generation Code only work for a Super Block ! ")
    end

endfunction
