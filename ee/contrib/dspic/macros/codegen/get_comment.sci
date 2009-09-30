
//==========================================================================
// get_comment : return a C comment
//               for generated code
//
//input : typ : a string
//        param : a list
//
//output : a C comment
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=get_comment(typ,param)
  txt = [];
  select typ
    //** main flag
    case 'flag' then
        select param(1)
          case 0 then
             txt = '/* Continuous state computation */'
          case 1 then
             txt = '/* Output computation */'
          case 2 then
             txt = '/* Discrete state computation */'
          case 3 then
             txt = '/* Output Event computation */'
          case 4 then
             txt = '/* Initialization */'
          case 5 then
             txt = '/* Ending */'
          case 9 then
             txt = '/* Update zero crossing surfaces */'
        end
    //** blocks activated on event number
    case 'ev' then
       txt = '/* Blocks activated on the event number '+string(param(1))+' */'

    //** blk calling sequence
    case 'call_blk' then
        txt = ['/* Call of '''+param(1) + ...
               ''' (type '+string(param(2))+' - blk nb '+...
                    string(param(3))];
        if ztyp(param(3)) then
          txt=txt+' - with zcross) */';
        else
          txt=txt+') */';
        end
    //** proto calling sequence
    case 'proto_blk' then
        txt = ['/* prototype of '''+param(1) + ...
               ''' (type '+string(param(2))];
        if ztyp(param(3)) then
          txt=txt+' - with zcross) */';
        else
          txt=txt+') */';
        end
    //** ifthenelse calling sequence
    case 'ifthenelse_blk' then
        txt = ['/* Call of ''if-then-else'' blk (blk nb '+...
                    string(param(1))+') */']
    //** eventselect calling sequence
    case 'evtselect_blk' then
        txt = ['/* Call of ''event-select'' blk (blk nb '+...
                    string(param(1))+') */']
    //** set block structure
    case 'set_blk' then
        txt = ['/* set blk struc. of '''+param(1) + ...
               ''' (type '+string(param(2))+' - blk nb '+...
                    string(param(3))+') */'];
    //** Update xd vector ptr
    case 'update_xd' then
        txt = ['/* Update xd vector ptr */'];
    //** Update g vector ptr
    case 'update_g' then
        txt = ['/* Update g vector ptr */'];
    else
      break;
  end
endfunction
