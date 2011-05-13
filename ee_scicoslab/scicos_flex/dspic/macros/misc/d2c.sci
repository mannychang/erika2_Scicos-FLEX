function [a,b] = d2c(phi,gm,t)

// Ouput variables initialisation (not found in input variables)
a=[];
b=[];

[m,n] = size(phi);
[m,nb] = size(gm);

if m==1 then
  if phi==1 then
    a = 0;  b = gm/t;
    return;
  end;
end;

b = zeros(m,nb);
nz = 0;
nonzero = [];
for i = 1:nb
  if or(gm(:,i))~=0 then
    nonzero = [nonzero,i];
    nz = nz+1;
  end;
end;

s = logm([phi,gm(:,nonzero);zeros(nz,n),eye(nz,nz)]);
s = s/t;
s = real(s);
a = s(1:n,1:n);
if max(size(b)) then
  b(:,nonzero) = s(1:n,n+1:n+nz);
end;
endfunction
