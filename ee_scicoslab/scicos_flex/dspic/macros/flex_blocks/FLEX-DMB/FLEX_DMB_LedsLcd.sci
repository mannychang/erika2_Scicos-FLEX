function [x,y,typ] = FLEX_DMB_LedsLcd(job,arg1,arg2)

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
  block_type = model.ipar(1);
  sblock_id = string(model.ipar(2));
  
  // lcd_intyp
  lcd_intyp = 'dd';
  if model.ipar(3)==1 then
    if model.ipar(4)==1 then
      lcd_intyp = 'dd';
    else
      lcd_intyp = 'du';
    end
  else
    if model.ipar(4)==1 then
      lcd_intyp = 'ud';
    else
      lcd_intyp = 'uu';
    end
  end
  
  while %t do
    dialog_box_banner = "Flex LEDs/LCD Parameters";
    [ok, new_block_id, new_lcd_intyp] = getvalue(dialog_box_banner,...
      ['Flex Identifier';'LCD input type [d:real, u:uint8] '], list('vec',1,'str',1), [sblock_id; lcd_intyp]);

    if ~ok then break, end //** in case of error

    label(1)(1) = 'Flex_'+string(new_block_id);
 
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
    funam = 'flex_blocks';
    funtyp = 4 ;
    input_leds_type = 5;//int8

    new_lcd_intyp = ascii(new_lcd_intyp);
    if new_lcd_intyp(1)==ascii('d') then
      if new_lcd_intyp(2)==ascii('d') then
        input_lcd_type = [1;1]; //real,real
        i = [1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 1];
      else if new_lcd_intyp(2)==ascii('u') then
             input_lcd_type = [1;8]; //real,uint8
             i = [1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 -1];
           else
             message("Error: unknown type for LCD port 2");
           end
      end
    else if new_lcd_intyp(1)==ascii('u') then
           if new_lcd_intyp(2)==ascii('d') then
             input_lcd_type = [8;1]; //uint8,real
             i = [1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 -1;1 1];
           else if new_lcd_intyp(2)==ascii('u') then
                  input_lcd_type = [8;8]; //uint8,uint8
             i = [1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 -1;1 -1];
                else
                  message("Error: unknown type for LCD port 2");
                end
           end
         else
           message("Error: unknown type for LCD port 1");
         end
    end

    ipar = [block_type;new_block_id;input_lcd_type];
    it = [input_leds_type;input_leds_type;input_leds_type;input_leds_type;...
          input_leds_type;input_leds_type;input_leds_type;input_leds_type;...
          input_lcd_type(1);input_lcd_type(2)];
    o = [];
    ot = [];
    [model, graphics, ok] = set_io(model, graphics, list(i,it), list(o, ot), ones(ci,1), []);
  
    if ~ok then
      message("something wrong in in/out settings");
    end
    model.sim = list(funam,funtyp) ; //** computation function
    model.in  = i(:,1);
    model.in2  = i(:,2);
    model.out = [];
    model.out2 = [];
    model.evtin = ci;
    model.evtout = [];
    model.state = [];
    model.dstate = 0 ;
    model.rpar = [];
    model.ipar = ipar;
    model.firing = [];
    model.dep_ut = dep_ut;
    model.nzcross = 0 ;
    x.model = model ;
    graphics.exprs = label ;
    x.graphics = graphics ;
    break
  end

case 'define' then      //** the standard define  
  flex_block_type_buttons = 0;
  flex_block_type_ledslcd = 1;
  block_type = flex_block_type_ledslcd;
  block_id = 0;
  
  name = 'Flex_' + string(block_id);

  model = scicos_model();
  funam = 'flex_blocks';
  funtyp = 4 ;
  model.sim=list(funam, funtyp) //** simulating function

  i = [1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 1;1 -1;1 -1];
  model.in = i(:,1);
  model.in2 = i(:,2);
  input_leds_type = 5;//int8
  input_lcd_type = [8;8]; //uint8,uint8
  it = [input_leds_type;input_leds_type;input_leds_type;input_leds_type;...
        input_leds_type;input_leds_type;input_leds_type;input_leds_type;...
        input_lcd_type(1);input_lcd_type(2)];
  model.intyp = it;
  model.out = [] ;
  model.out2 = [];
  model.evtin = 1 ;
  model.evtout = [] ;
  model.state = [] ;
  model.dstate = [] ;
  model.rpar = [];
  model.ipar = [block_type;block_id;8;8];
  model.blocktype = 'c';
  model.firing = [] ;
  model.dep_ut = [%t %f];
  model.nzcross = 0 ;

  label = list( [name], [] ) ;

  gr_i=['[x,y,typ]=standard_inputs(o) ';
    'dd=sz(1)/16'
	'r = xstringl(0,0,''LED0'')';
    'if ~arg1.graphics.flip then'
	'  dd=sz(1)-r(3)-dd'
	'end'
    'xstring(orig(1)+dd,y(1)-r(4)/2,''LED0'')'
    'xstring(orig(1)+dd,y(2)-r(4)/2,''LED1'')'
    'xstring(orig(1)+dd,y(3)-r(4)/2,''LED2'')'
    'xstring(orig(1)+dd,y(4)-r(4)/2,''LED3'')'
    'xstring(orig(1)+dd,y(5)-r(4)/2,''LED4'')'
    'xstring(orig(1)+dd,y(6)-r(4)/2,''LED5'')'
    'xstring(orig(1)+dd,y(7)-r(4)/2,''LED6'')'
    'xstring(orig(1)+dd,y(8)-r(4)/2,''LED7'')'
    'xstring(orig(1)+dd,y(9)-r(4)/2,''LCD1'')'
    'xstring(orig(1)+dd,y(10)-r(4)/2,''LCD2'')'
	'rect = xstringl(0,0,name)'
	'if graphics.flip then'
	'  xstring(orig(1)+sz(1)-rect(3),orig(2)+sz(2)/2-rect(4),name);'
	'else'
	'  xstring(orig(1),orig(2)+sz(2)/2-rect(4),name);'
	'end' 
	];
 
  x = standard_define([4 6],model,label,gr_i)

case 'compile' then
  flex_path = getenv("FLEXPATH","");
  if isempty(flex_path) == %T then
    disp("Environment variable FLEXPATH not found!");
  else
      [info_file,ierr] = fileinfo(flex_path);
      if ierr <> 0 then
        error("Flex file " + flex_path + " not found!");
      end
  end
end
endfunction

