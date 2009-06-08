// Expected integer params 
// ipar[0] := local_address 
// ipar[1] := pan_id 
// ipar[2] := channel_id 
// ipar[3] := BeaconOrder 
// ipar[4] := SuperframeOrder 
// ipar[5] := gts1_addr 
// ipar[6] := gts1_len 
// ipar[7] := gts1_dir 
// ipar[8] := gts2_addr 
// ipar[9] := gts2_len 
// ipar[10] := gts2_dir 
// ipar[11] := gts3_addr 
// ipar[12] := gts3_len 
// ipar[13] := gts3_dir 
// ipar[14] := gts4_addr 
// ipar[15] := gts4_len 
// ipar[16] := gts4_dir 
// ipar[17] := gts5_addr 
// ipar[18] := gts5_len 
// ipar[19] := gts5_dir 
// ipar[20] := gts6_addr 
// ipar[21] := gts6_len 
// ipar[22] := gts6_dir 
// ipar[23] := gts7_addr 
// ipar[24] := gts7_len 
// ipar[25] := gts7_dir 

function [x,y,typ] = FLEX_802154c_config(job,arg1,arg2)
  x=[];y=[];typ=[];
  select job
  case 'plot' then
    exprs=arg1.graphics.exprs;
    local_address=exprs(1);
    pan_id=exprs(2);
    channel_id=exprs(3);
    BeaconOrder=exprs(4);
    SuperframeOrder=exprs(5);
    gts_list_addr=exprs(6);
    gts_list_len=exprs(7);
    gts_list_dir=exprs(8);
    standard_draw(arg1)
  case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
  case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
  case 'getorigin' then
    [x,y]=standard_origin(arg1)
  case 'set' then
    x=arg1
    model=arg1.model;graphics=arg1.graphics;
    exprs=graphics.exprs;
    while %t do
      [ok, local_address, pan_id, channel_id, BeaconOrder, SuperframeOrder,..
       gts_list_addr, gts_list_len, gts_list_dir, exprs]=..
      getvalue('802.15.4 Coordinator Params',..
      ['Local Address [1...255]:';..
       'PanID [1...255]';..
       'Radio Channel [11...26]';..
       'Beacon Order [3...14]';..
       'Superframe Order [3...14]';..
       'GTS Device Addresses [[0...255] ...]';..
       'GTS Lengths [[0,2] ...]';..
       'GTS Directions [[0=tx,1=rx] ...]'],..
      list('vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,..
           'vec',7,'vec',7,'vec',7),exprs)
      if ~ok then break,end
      if(local_address<1 | local_address>255) then
        warning('Invalid range for Local Address. Keeping previous values.');
        break;
      end
      if(pan_id<1 | pan_id>255) then
        warning('Invalid range for PanID. Keeping previous values.');
        break;
      end
      if(channel_id<11 | channel_id>26) then
        warning('Invalid range for Radio Channel. Keeping previous values.');
        break;
      end
      if(BeaconOrder<3 | BeaconOrder>14) then
        warning('Invalid range for Beacon Order. Keeping previous values.');
        break;
      end
      if(SuperframeOrder<3 | SuperframeOrder>14) then
        warning('Invalid range for Superframe Order. Keeping previous values.');
        break;
      end
      flag_error=0;
      for i=1:7,
        if(gts_list_addr(i)<0 | gts_list_addr(i)>255) then
          warning('Invalid Device Address for GTS. Keeping previous values.');
          flag_error=1;
          break;
        end
        if(gts_list_len(i)<0 | gts_list_len(i)>2) then
          warning('Invalid Length for GTS. Keeping previous values.');
          flag_error=1;
          break;
        end
        if(gts_list_dir(i)<0 | gts_list_dir(i)>1) then
          warning('Invalid Direction for GTS. Keeping previous values.');
          flag_error=1;
          break;
        end
      end
      if(flag_error==1)
        break;  
      end

      in=[],
      out=[],
      [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
      if ok then
        graphics.exprs=exprs;
        model.rpar=[];
        model.ipar=[local_address; pan_id; channel_id; BeaconOrder;..
                     SuperframeOrder; gts_list_addr(1); gts_list_len(1);..
                     gts_list_dir(1); gts_list_addr(2); gts_list_len(2);..
                     gts_list_dir(2); gts_list_addr(3); gts_list_len(3);..
                     gts_list_dir(3); gts_list_addr(4); gts_list_len(4);..
                     gts_list_dir(4); gts_list_addr(5); gts_list_len(5);..
                     gts_list_dir(5); gts_list_addr(6); gts_list_len(6);..
                     gts_list_dir(6); gts_list_addr(7); gts_list_len(7);..
                     gts_list_dir(7)];
        model.dstate=[1];
        x.graphics=graphics;x.model=model
        break
      end
    end
  case 'define' then
    local_address=1; 
    pan_id=1; 
    channel_id=24; 
    BeaconOrder=6; 
    SuperframeOrder=6; 
    gts_list_addr = [0, 0, 0, 0, 0, 0, 0];
    gts_list_len = [0, 0, 0, 0, 0, 0, 0];
    gts_list_dir = [0, 0, 0, 0, 0, 0, 0];

    model=scicos_model()
    model.sim=list('flex_simple802154_coordinator_config',4)
    model.in=[],
    model.out=[],
    model.evtin=1
    model.rpar=[]
        model.ipar=[local_address; pan_id; channel_id; BeaconOrder;..
                     SuperframeOrder; gts_list_addr(1); gts_list_len(1);..
                     gts_list_dir(1); gts_list_addr(2); gts_list_len(2);..
                     gts_list_dir(2); gts_list_addr(3); gts_list_len(3);..
                     gts_list_dir(3); gts_list_addr(4); gts_list_len(4);..
                     gts_list_dir(4); gts_list_addr(5); gts_list_len(5);..
                     gts_list_dir(5); gts_list_addr(6); gts_list_len(6);..
                     gts_list_dir(6); gts_list_addr(7); gts_list_len(7);..
                     gts_list_dir(7)];
    model.dstate=[1];
    model.blocktype='d'
    model.dep_ut=[%t %f]
    exprs=[sci2exp(local_address);..
           sci2exp(pan_id);..
           sci2exp(channel_id);..
           sci2exp(BeaconOrder);..
           sci2exp(SuperframeOrder);..
           sci2exp(gts_list_addr);..
           sci2exp(gts_list_len);..
           sci2exp(gts_list_dir)]
    gr_i=['xstringb(orig(1),orig(2),..
		[''FLEX 802.15.4'';..
		''COORDINATOR'';..
		''CONFIG''],..
		sz(1),sz(2),''fill'');']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction
