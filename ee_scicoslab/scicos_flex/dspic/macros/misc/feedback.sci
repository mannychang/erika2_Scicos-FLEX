function [gtot]=feedback(g,h)
// Calculate the feedback of g(s) and h(s)
gtot=g/.(h)
 
endfunction
