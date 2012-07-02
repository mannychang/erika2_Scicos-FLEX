function [res] = _spf_remove_djolly(str,jolly)
  res = str;
  subindexes = [];
  i=1;
  while i < length(str)
    if part(str, i) == jolly then
      k = i + 1;
      if k <= length(str) then
        if part(str, k) == jolly then
          subindexes = [subindexes i];
          i = i + 2;
          continue;
        end
      end
    end
    i = i + 1;
  end
  n = length(subindexes);
  if n <> 0 then
    substrs = [];
    j = 1;
    for i=1:n
      substrs = [substrs part(str, j:1:subindexes(i)-1)];
      j = subindexes(i) + 2;
    end
    if j <= length(str) then
      substrs = [substrs part(str, j:1:length(str))];
    end
    res = strcat(substrs);
  end
endfunction
  

function [ok, errstr, types] = _spf_get_types(sl)
  types = [];
  ok = %T;
  errstr = "";
  if typeof(sl) <> "list"
    ok = %F;
    errstr = "Invalid type for input, list expected";
    return;
  end
  for i=1:length(sl)
    str = sl(i);
    if typeof(str) <> "string" then
      ok = %F;
      errstr = "Invalid list item type, string expected";
      return;
    end
    str = _spf_remove_djolly(str, "%");
    idx = strindex(str, "%");
    n = length(idx);
    if n == 0 then
      types = [types "none"];
      continue;
    elseif n > 1 then
      ok = %F;
      errstr = "Invalid format, only one paramater is allowed";
      return;
    end
    t = [];
    for j=idx:length(str)
      tmp = part(str, j);
      if tmp == " " then
        break;
      elseif tmp == "d" | tmp == "f" | tmp == "u" | tmp == "c" then
        t = tmp;
        break;
      end  
    end
    if isempty(t) == %T then
      ok = %F;
      errstr = "Invalid format for" + str;
      return;
    else
      types = [types t];
    end
  end
endfunction

function res = _spf_get_max_out_size()
  res = 64;    
endfunction
  

function [x,y,typ] = sprintf_block(job,arg1,arg2)

//** ------------------------------------ INPUT ---------------------------------

x=[];y=[];typ=[];

select job //** main state machine switch

case 'plot' then  //** plot the object
  graphics = arg1.graphics;
  exprs = graphics.exprs;
  name = exprs(1)(1);
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
  outsize = [model.out model.out2];
  sizes = model.ipar(2:1:2+model.ipar(1)-1);
  lfmts = list();
  for i=1:length(model.opar)
    astr = model.opar(i);
    if length(astr) > 0 then
      str = ascii2string(astr(1:length(astr)-1));
      lfmts = lstcat(lfmts, str);
    end
  end
  ssizes = sci2exp(sizes);
  sfmts = sci2exp(lfmts);
  soutsize = sci2exp(outsize);
  while %t do
    dialog_box_banner = ["sprintf Parameters";
        "According to the format tags of the sprintf function ";
        """%[flags][width][.precision][length]specifier"" this implementation:";
        "    - Does not support the [length] sub-specifier";
        "    - Supports only the following specifiers: d(int32), u(uint32), f(real), c(int8)";
        "The output is an int8 row/column vector representing the ASCII codes of the whole string"];
    [ok, fmts, sizes, outsize] = getvalue(dialog_box_banner,...
      ['Formats (must be list(""fmt1"", ""fmt2"", ...) and each ""fmt"" supports up to one parameter)';...
        'Sizes (The length of each ""fmt"" not including the null-terminator)';...
        'Output size (Max '+sci2exp(_spf_get_max_out_size())+' including the null-terminator)'],...
      list('lis',-1,'vec',-1,'row',-1), [sfmts; ssizes; soutsize]);
    if ~ok then break, end //** in case of error
    
    opar=list();
    for i=1:length(fmts)
       str = fmts(i);
       opar = lstcat(opar, int8([ascii(str) 0]));
    end
    // Check outsize
    if(length(outsize)==1)
      outsize = [1 outsize];
      message('SPRINTF block: For compatibility with old GUI, output array is a row vector if only one dimension is specified.');
    elseif(length(outsize)>2)
      message('SPRINTF block: Bad size for the output vector!');
      break;
    elseif(outsize(1)*outsize(2) > outsize(1) & outsize(1)*outsize(2) > outsize(2))
      message('SPRINTF block: Bad size for the output array! It should be a vector!');
      break;
    end
    //parse formats to find out the inputs types
    [valid, errstr, intypes] = _spf_get_types(fmts);   
    if valid == %F | outsize(1)*outsize(2) > _spf_get_max_out_size() then 
        model.opar = opar;
        s = size(sizes);
        if s(1) < s(2) then
          sizes = sizes';
        end
        nsizes = length(sizes);
        model.ipar = [n;sizes;-ones(nsizes)];
        x.model = model;
        error(errstr);
    end
    i = [];
    it = [];
    types("d") = 3; //int32
    types("u") = 6; //uint32
    types("f") = 1; //real
    types("c") = 5; //int8
    types("none") = -1;
    len = size(intypes);
    ipar_types = [];
    for k=1:max(len(1),len(2))
      intype = intypes(k);
      ipar_types = [ipar_types;types(intypes(k))];
      if intype <> "none" then
        it = [it types(intypes(k))];
        i = [i;1 1];
      end
    end
  
    o = outsize;
    ot = 8; //uint8
  
    ng = [];
    z  = 0;
    nx = 0;
    nz = 0;
    nin = 1;
    ci  = 1;
    nevin = 1;
    co = [];
    nevout = 0;
    depu = %t; dept = %f;
    dep_ut = [depu dept];
    funam = 'sprintf_block';
    funtyp = 4 ;
    s = size(sizes);
    if s(1) < s(2) then
      sizes = sizes';
    end
    nsizes = length(sizes);
    ipar = [nsizes;sizes;ipar_types];
    [model, graphics, ok] = set_io(model, graphics, list(i,it), list(o, ot), ones(ci,1), []);
  
    if ~ok then
      message("something wrong in in/out settings");
    end
    model.sim = list(funam,funtyp) ; //** computation function
    model.in  = i(:,1);
    model.in2  = i(:,2);
    model.out = o(:,1);
    model.out2 = o(:,2);
    model.evtin = ci;
    model.evtout = [];
    model.state = [];
    model.dstate=[] ;
    model.rpar = [];
    model.ipar = ipar;
    model.opar = opar;
    model.firing = [];
    model.dep_ut = dep_ut;
    model.nzcross = 0 ;
    x.model = model ;
    graphics.exprs = label ;
    x.graphics = graphics ;
    if length(fmts) <> length(sizes) then
      message("WARNING: Formats/Sizes dimensions disagree, cannot run the simulation");
      break;
    end
    break;
  end

case 'define' then      //** the standard define  
  
  name = 'sprintf';

  model = scicos_model();
  funam = 'sprintf_block';
  funtyp = 4 ;
  model.sim=list(funam, funtyp) //** simulating function

  model.in = [];
  model.in2 = [];
  it = [];
  model.intyp = it;
  model.out = 1;
  model.out2 = _spf_get_max_out_size();
  model.outtyp = 8; //uint8
  model.evtin = 1 ;
  model.evtout = [] ;
  model.state = [] ;
  model.dstate = [] ;
  model.rpar = [];
  model.ipar = [0];
  model.opar = list();
  model.blocktype = 'c';
  model.firing = [] ;
  model.dep_ut = [%t %f];
  model.nzcross = 0 ;
  model.label = "";
  label = list( [name], [] ) ;

  gr_i=['xstringb(orig(1),orig(2),[''sprintf''],sz(1),sz(2),''fill'');'];
 
  x = standard_define([4 2],model,label,gr_i);

case 'compile' then
  model = arg1;
  sizes = model.ipar(1);
  sfmt = length(model.opar);
  if sizes <> sfmt then
    error("Formats/Sizes dimensions disagree");
  end
end
  
endfunction
