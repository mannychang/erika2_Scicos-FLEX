# Script to install the Scicos_pack in batch mode... 
#export PACKBASE=C:/DOCUME~1/Dario/Desktop/SCICOS~1.1/SCICOS~1.1
#export SCIBASE=C:/PROGRA~2/SCICOS~1.1

if [ -z "$PACKBASE" ]; then
  echo WARNING: Missing environment variable PACKBASE
  exit 1;
fi
  
if [ -z "$SCIBASE" ]; then
  echo WARNING: Missing environment variable SCIBASE
  exit 1;
else
  cp $PACKBASE/scicos_ee/utils/batch_installer/silent_installer.sce $PACKBASE
  $SCIBASE/bin/cscilex.exe -nw -nb -f $PACKBASE/silent_installer.sce > inst_batch.log
fi

