

//==========================================================================
//mat2scs_c_nb  matrix to scicos C number (sci2sci_n)
//
//input : outtb : a matrix
//
//output : c_nb : the scicos C number
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [c_nb]=mat2scs_c_nb(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        c_nb = 10
      else
        c_nb = 11
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           c_nb = 84
         case 'int16' then
           c_nb = 82
         case 'int8' then
           c_nb = 81
         case 'uint32' then
           c_nb = 814
         case 'uint16' then
           c_nb = 812
         case 'uint8' then
           c_nb = 811
      end
   else
     break;
 end
endfunction
