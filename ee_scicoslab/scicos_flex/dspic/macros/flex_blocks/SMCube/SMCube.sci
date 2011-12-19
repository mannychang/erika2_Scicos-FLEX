function [result] = _getFileName(path)
  result = " ";
  tmp=strsubst(path,"\","/");
  indexes=[strindex(tmp,"/")];
  if isempty(indexes) == %F then
    strs=strsplit(tmp,indexes);
    strs_size=size(strs);
    if strs_size(1) > 0 then
      result=strs(strs_size(1), 1);
    end
  else
    result=path;
  end
endfunction

function [x,y,typ] = SMCube(job,arg1,arg2)

//** ------------------------------------ INPUT ---------------------------------

x=[];y=[];typ=[];

select job //** main state machine switch

case 'plot' then  //** plot the object
  graphics = arg1.graphics;
  exprs = graphics.exprs;
  name  = exprs(1)(4);
  smcube_filename = exprs(1)(5);
  standard_draw(arg1)

case 'getinputs' then //** inputs
  [x,y,typ] = standard_inputs(arg1) //**

case 'getoutputs' then //**
  [x,y,typ] = standard_outputs(arg1)

case 'getorigin' then  //**
  [x,y]=standard_origin(arg1)

case 'set' then //** set parameters
  x = arg1 ; //**
  model    = arg1.model;
  graphics = arg1.graphics;
  label    = graphics.exprs;
  sengine_file = stripblanks(label(1)(3));
  sengine_path = getenv("SMCUBEPATH","");
  [info_file,ierr] = fileinfo(sengine_path);
  if ierr <> 0 then
    message("SMCube application binary file " + sengine_path + " not found!");
  end
  res = message("Make your choice",["Editor","Properties"]);  
  //res = buttondialog("Make your choice","Editor|Properties|Cancel","question");
  //if res == 3 then
  //  return
  //end
  if res == 1 then
    if isempty(sengine_file) == %T then
        message("The SMCube file is empty");
        break
    end
    [info_file,ierr] = fileinfo(sengine_file);
    if ierr <> 0 then
      message("SMCube file " + sengine_file + " not found, an empty state machine will be created.")
    end
    if MSDOS == %T then
      sengine_path = msprintf("""%s""",sengine_path);
    end
    unix_s(sengine_path+ " -editor " + sengine_file);
    return
  end
  
  reserved = model.ipar(1);
  seventsin = string(length(model.evtin));
  ssimmode = string(model.ipar(2));
  sindata = stripblanks(label(1)(1));
  soutdata = stripblanks(label(1)(2));
  sengine_file = stripblanks(label(1)(3));
  while %t do
    dialog_box_banner = "SMCube Parameters";
    [ok, eventsin, simmode, indata, outdata, engine_file, lab] = getvalue(dialog_box_banner,...
     ['Number of input event ports';'Mode (1:interactive, 2:background)';'Input Description (''d'':double,''i'':int32)';...
      'Output Description (''d'':double,''i'':int32)';'SMCube File'],...
     list('vec',1,'vec',1,'str',1,'str',1,'str',1),...
     [seventsin;ssimmode;sindata;soutdata;sengine_file]);                                                
    if ~ok then break, end //** in case of error
    
    ng = [];
    z  = 0;
    nx = 0;
    nz = 0;
    nin = 1;
    if eventsin > 32 then
      eventsin = 32;
    elseif eventsin < 0 then
      eventsin = 0;
    end
    ci  = ones(eventsin,1);
    co = [];
    depu = %t; dept = %f;
    dep_ut = [depu dept];
    funam = 'smcube_block';
    funtyp = 4 ;
    indata=stripblanks(indata);
    outdata=stripblanks(outdata);
    engine_file=stripblanks(engine_file);
    result = 0;
    if ok then
      //** check for smcube path and file
      if isempty(engine_file) == %T then
        message("The SMCube file is empty");
        result = 1;
      else
        [info_file,ierr] = fileinfo(engine_file);
        if ierr <> 0 then
          message("SMCube file " + engine_file + " not found!");
          result = 1;
        end
      end
    
      //** parse input and output
      int32_type = 3;
      double_type = 1;

      i = [];
      it = [];
      in_descr = ascii(indata);
      in_descr_size = size(in_descr);
      new_descr = [];
      for k=1:in_descr_size(2)
        if in_descr(k) == ascii("i") then
          i = [i;1 1];
          it = [it;int32_type];
          new_descr = [new_descr in_descr(k)];
        elseif in_descr(k) == ascii("d") then
          i = [i;1 1];
          it = [it;double_type];
          new_descr = [new_descr in_descr(k)];
        else
          message("invalid input type: " + ascii2string(in_descr(k)));
        end
      end
    
      in_descr = new_descr;
    
      o = [];
      ot = [];
      out_descr = ascii(outdata);
      out_descr_size = size(out_descr);
      new_descr = [];
      for k=1:out_descr_size(2)
        if out_descr(k) == ascii("i") then
          o = [o;1 1];
          ot = [ot;int32_type]; 
          new_descr = [new_descr out_descr(k)];
        elseif out_descr(k) == ascii("d") then
          o = [o;1 1];
          ot = [ot;double_type];
          new_descr = [new_descr out_descr(k)];
        else  
          message("invalid output type: " + ascii2string(out_descr(k)));
        end
      end
      
      out_descr = new_descr;
      [model, graphics, ok] = set_io(model, graphics, list(i,it), list(o, ot), ci, co);
      if ~ok then
        result = 1;  
      end
    
      indata = ascii2string(in_descr);
      outdata = ascii2string(out_descr);
      
      if result <> 0 then
        //** Save current valid parameters
        if length(indata) == 0
          indata = ' ';
        end
        if length(outdata) == 0
          outdata = ' ';
        end   
        if length(engine_file) == 0
          engine_file = ' ';
        end
	smcube_filename=_getFileName(engine_file);
        label(1)(1) = indata;
        label(1)(2) = outdata;
        label(1)(3) = engine_file;
	label(1)(5) = smcube_filename;
        model.sim = list(funam,funtyp) ; //** computation function
        model.in  = i(:,1);
        model.in2  = i(:,2);
        model.out = o(:,1);
        model.out2 = o(:,2);
        model.evtin = ci;
        model.evtout = [];
        model.state = [];
        model.dstate = 0 ;
        model.rpar = [];
        model.ipar = [reserved;simmode;length(indata);ascii(indata)';length(outdata);ascii(outdata)';...
          length(engine_file);ascii(engine_file)'];
        model.firing = [];
        model.dep_ut = dep_ut;
        model.nzcross = 0 ;
        x.model = model ;
        graphics.exprs = label ;
        x.graphics = graphics ;   
	    break
      end
      // Check is ok!
      if length(indata) == 0
        indata = ' ';
      end
      if length(outdata) == 0
        outdata = ' ';
      end   
      if length(engine_file) == 0
        engine_file = ' ';
      end
      smcube_filename=_getFileName(engine_file);
      label(1)(1) = indata;
      label(1)(2) = outdata;
      label(1)(3) = engine_file;
      label(1)(5) = smcube_filename;
      model.sim = list(funam,funtyp) ; //** computation function
      model.in  = i(:,1);
      model.in2  = i(:,2);
      model.out = o(:,1);
      model.out2 = o(:,2); 
      model.evtin = ci;
      model.evtout = [];
      model.state = [];
      model.dstate = 0 ;
      model.rpar = [];
      model.ipar = [reserved;simmode;length(indata);ascii(indata)';length(outdata);ascii(outdata)';...
        length(engine_file);ascii(engine_file)'];
      model.firing = [];
      model.dep_ut = dep_ut;
      model.nzcross = 0 ;
      x.model = model ;
      graphics.exprs = label ;
      x.graphics = graphics ;
      break
    end
  end

case 'define' then      //** the standard define  
  indata = "";
  outdata = "";
  simmode = 2;
  engine_file = ' '
  reserved = -1;

  sengine_path = getenv("SMCUBEPATH","");
  if isempty(sengine_path) == %T then
    message("Please set the environment variable SMCUBEPATH");
  end
  name = 'SMCube'; //** default name

  model = scicos_model();
  funam = 'smcube_block';
  funtyp = 4 ;
  model.sim=list(funam, funtyp) //** simulating function

  model.in = [] ;
  model.in2 = [];
  model.out = [] ;
  model.out2 = [];
  model.evtin = [] ;
  model.evtout = [] ;
  model.state = [] ;
  model.dstate = [] ;
  model.rpar = [];
  model.ipar = [reserved;simmode;length(indata);ascii(indata)';length(outdata);ascii(outdata)';...
    length(engine_file);ascii(engine_file)'];
  model.blocktype = 'c';
  model.firing = [] ;
  model.dep_ut = [%t %f];
  model.nzcross = 0 ;
  
  smcube_filename=_getFileName(engine_file);

  label = list( [indata outdata engine_file name smcube_filename], [] ) ;

  gr_i=['xstringb(orig(1),orig(2),[name;smcube_filename],sz(1),sz(2),''fill'');']
  x = standard_define([4 2],model,label,gr_i)

case 'compile' then
  model = arg1;
  ipar = model.ipar;
  il = ipar(3);
  ol = ipar(3 + il + 1);
  fl = ipar(3+ il + 1 + ol + 1);
  if fl == 0 then
    error("SMCube file is empty");
  else
    pos = 3 + il + 1 + ol + 1 + 1;
    engine_file = ascii2string(ipar(pos:pos+fl-1)');
    [info_file,ierr] = fileinfo(engine_file);
    if ierr <> 0 then
      error("SMCube file " + engine_file + " not found!");
    end
  end
  
  sengine_path = getenv("SMCUBEPATH","");
  if isempty(sengine_path) == %T then
    error("Please set the environment variable SMCUBEPATH");
  end
  
 

end
endfunction

