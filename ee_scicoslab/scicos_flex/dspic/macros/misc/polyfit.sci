function p=polyfit(x,y,grad,s)
// Calculates the polynomial of grad "grad" which fits the given
// points "x,y". "s" is the character associated to the polinomial

[lhs, rhs] = argn(0);
m = length(x);
if length(y) <> m then
   error('x and y must have same length'); 
end
a=[];
for n=0:1:grad
	a = [a,x'.^n];
end
p=a\y';
if rhs > 3 then
   p=poly(p,s,'c');
end

endfunction
