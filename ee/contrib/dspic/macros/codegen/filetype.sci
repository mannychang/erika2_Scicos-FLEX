
//==========================================================================
function t=filetype(m)
  m=int32(m)
  filetypes=['Directory','Character device','Block device',...
             'Regular file','FIFO','Symbolic link','Socket']
  bits=[16384,8192,24576,32768,4096,40960,49152]
  m=int32(m)&int32(61440)
  t=filetypes(find(m==int32(bits)))
endfunction
