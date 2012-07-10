//% -------------------------------------------------
//% Scicos pack for Erika (free version)
//% Loader/Unloader script
//% -------------------------------------------------

ierr = execstr("getenv(''CGTOOL_VERSION'');",'errcatch');
if(ierr > 0)
  exec(SCI + '/contrib/scicos_ee/user/ScicosLab/4.4.1/.scilab', -1);
else
  exec(SCI + '/contrib/scicos_ee/unloader.sce', -1);
end
