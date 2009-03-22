
//==========================================================================
//mat2c_typ : matrix to C type
//sci2c_ttyp : get the C string of a scicos type
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=mat2c_typ(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        txt = "double"
      else
        txt = "double"
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           txt = "long"
         case 'int16' then
           txt = "short"
         case 'int8' then
           txt = "char"
         case 'uint32' then
           txt = "unsigned long"
         case 'uint16' then
           txt = "unsigned short"
         case 'uint8' then
           txt = "unsigned char"
      end
   else
     break;
 end
endfunction
