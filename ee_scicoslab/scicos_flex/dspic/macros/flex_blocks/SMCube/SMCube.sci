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

function [result] = _buildPath(path)
  folderSeparator = "/";
  if MSDOS == %T then
    folderSeparator = "\";
  end
  path = stripblanks(path);
  while length(path) > 0 & part(path,length(path)) == folderSeparator then
    path = part(path, 1:length(path)-1);
  end
  if isempty(path) == %F then
    result = path + folderSeparator;
  end
endfunction

function [res] = _translateOldInOut(str)
  res = str;
  res = strsubst(res, "d", "1 1;");
  res = strsubst(res, "i", "3 1;");
endfunction

function [res, err] = _parseInOut(str)
  ok = %t;
  res = [];
  err = "";
  _translateOldInOut(str)
  try
      res = evstr(str);
  catch
      ok = %f;
  end
  if ok == %f then
      err = lasterror(%t);
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
  sengine_exe_name = "SMCube";
  if MSDOS == %T then
    sengine_exe_name = sengine_exe_name + ".exe";
  end
  sengine_path = getenv("SMCUBEPATH","");
  
  if isempty(sengine_path) == %T then
    disp("Environment variable SMCUBEPATH not found!");
    disp("SMCube has not been installed!");
    disp("Please note that the SMCube is included only with the full version of the ScicosLab-pack.");
  else
    sengine_path = _buildPath(sengine_path) + sengine_exe_name;
    [info_file,ierr] = fileinfo(sengine_path);
    if ierr <> 0 then
      disp("SMCube application binary file " + sengine_path + " not found!");
    end
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

    if isempty(sengine_path) == %T then
      disp("Environment variable SMCUBEPATH not found!");
      disp("SMCube has not been installed!");
      disp("Please note that the SMCube is included only with the full version of the ScicosLab-pack.");
      message("SMCube has not been installed!");
      return
    else
      if MSDOS == %T then
        sengine_path = msprintf("""%s""",sengine_path);
      end
      unix_s(sengine_path+ " -editor " + sengine_file);
      return
    end
  end
  
  reserved = model.ipar(1);
  seventsin = string(length(model.evtin));
  seventsout = string(length(model.evtout));
  ssimmode = string(model.ipar(2));
  sindata = stripblanks(label(1)(1));
  soutdata = stripblanks(label(1)(2));
  sengine_file = stripblanks(label(1)(3));
  sindata = _translateOldInOut(sindata);
  soutdata = _translateOldInOut(soutdata);
  while %t do
    dialog_box_banner = "SMCube Parameters";
    [ok, eventsin, eventsout, simmode, indata, outdata, engine_file, lab] = getvalue(dialog_box_banner,...
     ['Number of input event ports';'Number of output event ports';'Mode (1:interactive, 2:background)';...
      'Input Description (''d'':double,''i'':int32)';'Output Description (''d'':double,''i'':int32)';'SMCube File'],...
     list('vec',1,'vec',1,'vec',1,'str',1,'str',1,'str',1),...
     [seventsin;seventsout;ssimmode;sindata;soutdata;sengine_file]);                                                
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
    if eventsout > 32 then
      eventsout = 32;
    elseif eventsin < 0 then
      eventsout = 0;
    end
    ci  = ones(eventsin,1);
    co = ones(eventsout,1);
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
      [intypes, errstr] = _parseInOut(indata);
      if isempty(errstr) == %f then
        message("Input error " + errstr);
      end
      [outtypes, errstr] = _parseInOut(outdata);
      if isempty(errstr) == %f then
        message("Output error " + errstr);
      end
      i = [intypes(:, 2) ones(intypes(:,2))];
      it = intypes(:, 1);
      o = [outtypes(:, 2) ones(outtypes(:,2))];
      ot = outtypes(:, 1);
      [model, graphics, ok] = set_io(model, graphics, list(i,it), list(o, ot), ci, co);
      if ~ok then
        result = 1;  
      end
      
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
        model.evtout = co;
        model.state = [];
        model.dstate=[] ;
        model.rpar = [];
        model.ipar = [reserved;simmode; length(engine_file);ascii(engine_file)'];
        model.firing = -co;
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
      model.evtout = co;
      model.state = [];
      model.dstate=[] ;
      model.rpar = [];
      model.ipar = [reserved;simmode; length(engine_file);ascii(engine_file)'];
      model.firing = -co;
      model.dep_ut = dep_ut;
      model.nzcross = 0 ;
      x.model = model ;
      graphics.exprs = label ;
      x.graphics = graphics ;
      break
    end
  end

case 'define' then      //** the standard define  
  indata = "[]";
  outdata = "[]";
  simmode = 2;
  engine_file = ' '
  reserved = -1;

  sengine_path = getenv("SMCUBEPATH","");
  if isempty(sengine_path) == %T then
    disp("Environment variable SMCUBEPATH not found!");
    disp("SMCube has not been installed!");
    disp("Please note that the SMCube is included only with the full version of the ScicosLab-pack.");
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
  model.ipar = [reserved;simmode; length(engine_file);ascii(engine_file)'];
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
  fl = ipar(3);
  if fl == 0 then
    error("SMCube file is empty");
  else
    pos = 3 + 1;
    engine_file = ascii2string(ipar(pos:pos+fl-1)');
    [info_file,ierr] = fileinfo(engine_file);
    if ierr <> 0 then
      error("SMCube file " + engine_file + " not found!");
    end
  end
  
  sengine_path = getenv("SMCUBEPATH","");
  if isempty(sengine_path) == %T then
    disp("Environment variable SMCUBEPATH not found!");
    disp("SMCube has not been installed!");
    disp("Please note that the SMCube is included only with the full version of the ScicosLab-pack.");
    error("SMCube has not been installed!");
  else
      sengine_exe_name = "SMCube";
      sengine_conf_name = "Configuration.ini";
      sengine_path = _buildPath(sengine_path);
      if MSDOS == %T then
        sengine_exe_name = sengine_exe_name + ".exe";
      end
      sengine_binary_file = sengine_path + sengine_exe_name;
      [info_file,ierr] = fileinfo(sengine_binary_file);
      if ierr <> 0 then
        error("SMCube application binary file " + sengine_binary_file + " not found!");
      end
      sengine_conf_file = sengine_path + sengine_conf_name;
      [info_file,ierr] = fileinfo(sengine_conf_file);
      if ierr <> 0 then
        error("Cannot open the configuration file.");
      end
  end 
  
  // Test user privileges for pwd
  [status_test, msg_test] = mkdir(pwd(),'test_admin');
  [fd_test,err_test] = mopen(pwd() + '\test_admin\test_admin.x', 'w');
  if err_test == 0
    mfprintf(fd_test, 'test');
    mclose(fd_test);
  end
  [testadmin_x,testadmin_err] = fileinfo(pwd() + '\test_admin\test_admin.x');
  if testadmin_err ~= 0
    sm3_msg = "#SMCube error: Access denied! Please, change the working directory or run ScicosLab with administrator privileges!";
    disp(sm3_msg);
    error("#SMCube error: Access denied for the current working directory!");
    return;
  else
    rmdir(pwd() + '/test_admin','s');
  end

end
endfunction

