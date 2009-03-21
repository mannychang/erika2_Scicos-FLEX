
//==========================================================================
//get_blank : return blanks with a length
//            of the given input string
//
//input : str : a string
//
//output : txt : blanks
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt] = get_blank(str)
 txt='';
 for i=1:length(str)
     txt=txt+' ';
 end
endfunction
