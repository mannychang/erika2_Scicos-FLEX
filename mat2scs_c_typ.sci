

//==========================================================================
//mat2scs_c_typ matrix to scicos C type
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=mat2scs_c_typ(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        txt = "SCSREAL_N"
      else
        txt = "SCSCOMPLEX_N"
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           txt = "SCSINT32_N"
         case 'int16' then
           txt = "SCSINT16_N"
         case 'int8' then
           txt = "SCSINT8_N"
         case 'uint32' then
           txt = "SCSUINT32_N"
         case 'uint16' then
           txt = "SCSUINT16_N"
         case 'uint8' then
           txt = "SCSUINT8_N"
      end
   else
     break;
 end
endfunction
