
//==========================================================================
//scs_c_nb2scs_nb : scicos C number to scicos number
//
//input : c_nb  : the scicos C number type
//
//output : scs_nb : the scilab number type
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [scs_nb]=scs_c_nb2scs_nb(c_nb)
 scs_nb=zeros(size(c_nb,1),size(c_nb,2));
 for i=1:size(c_nb,1)
   for j=1:size(c_nb,2)
     select (c_nb(i,j))
       case 10 then
         scs_nb(i,j) = 1
       case 11 then
         scs_nb(i,j) = 2
       case 81 then
         scs_nb(i,j) = 5
       case 82 then
         scs_nb(i,j) = 4
       case 84 then
         scs_nb(i,j) = 3
       case 811 then
         scs_nb(i,j) = 8
       case 812 then
         scs_nb(i,j) = 7
       case 814 then
         scs_nb(i,j) = 6
       else
         scs_nb(i,j) = 1
     end
   end
 end
endfunction
