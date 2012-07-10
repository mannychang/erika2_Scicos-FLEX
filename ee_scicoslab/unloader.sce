//% -------------------------------------------------
//% Scicos pack for Erika (free version)
//% Unloader script
//% -------------------------------------------------

//% Unlink DLLs
unlink_err = 0;
while (1)
  exec(SCI + '/contrib/scicos_ee/utils/unlink_dll.sce', -1);
  if (unlink_err == 0)
    break;
  end
end

//% Clear environment variables
setenv('CGTOLL_VERSION','');
setenv('FLEXPATH','');
setenv('SMCUBEPATH','');
setenv('COMMONDIR','');

//% Remove GIF references
row = grep(%scicos_gif,'macros/man/amazing');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/easylab');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/flex');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/flex_com');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/flex_dmb');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/flex_mtb');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/rt_data_exchange');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/misc');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/NativeInteger');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/miscellaneous');
if (row~=[])
  %scicos_gif(row(1)) = [];
end
row = grep(%scicos_gif,'macros/man/smcube');
if (row~=[])
  %scicos_gif(row(1)) = [];
end

//% Remove Palettes references
row = grep(scicos_pal,'MCHP16-Sources');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'MCHP16-Sinks');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'MCHP16-Linear');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'MCHP16-NonLinear');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'MCHP16-Logic');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'MCHP16-TypeConversion');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'MCHP16-Branching');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'CG-Sources');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'FLEX');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'FLEX-DemoBoard');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'FLEX-MotionBoard');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'FLEX-Communication');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'RT Data Exchange');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'Amazing Ball');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'Easylab');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'Miscellaneous');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'SMCube');
if (row~=[])
  scicos_pal(row(1),:) = [];
end
row = grep(scicos_pal,'FLEX-iSim');
if (row~=[])
  scicos_pal(row(1),:) = [];
end

//% Remove other references
row = vectorfind(%scicos_short,['w' 'Identification'],'r');
if (row ~= [])
  %scicos_short(row,:) = [];
end

clear row;

disp("Scicos Pack was successfully unloaded!!!");