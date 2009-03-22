
//==========================================================================
//scs_c_n2c_fmt : scicos C number to C format
//
//input : c_nb : a C scicos type
//
//output : txt : the string of the C format string
//               of the data of outtb
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=scs_c_n2c_fmt(c_nb)
 select c_nb
   //real matrix
   case 10 then
     txt = '%f';
   //complex matrix
   case 11 then
     txt = '%f,%f';
   //int8 matrix
   case 81 then
     txt = '%d';
   //int16 matrix
   case 82 then
     txt = '%d';
   //int32 matrix
   case 84 then
     txt = '%d';
   //uint8 matrix
   case 811 then
     txt = '%d';
   //uint16 matrix
   case 812 then
     txt = '%d';
   //uint32 matrix
   case 814 then
     txt = '%d';
   else
     txt='%f'
     break;
 end
endfunction
