
//==========================================================================
//scs_c_n2c_typ scicos C number to C type
//
//input : c_nb : a C scicos number
//
//output : txt : the string of the C format string
//               of the data of outtb
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=scs_c_n2c_typ(c_nb)
 select c_nb
   //real matrix
   case 10 then
     txt = 'double';
   //complex matrix
   case 11 then
     txt = 'double';
   //int8 matrix
   case 81 then
     txt = 'char';
   //int16 matrix
   case 82 then
     txt = 'short';
   //int32 matrix
   case 84 then
     txt = 'long';
   //uint8 matrix
   case 811 then
     txt = 'unsigned char';
   //uint16 matrix
   case 812 then
     txt = 'unsigned short';
   //uint32 matrix
   case 814 then
     txt = 'unsigned long';
   else
     txt='double'
     break;
 end
endfunction
