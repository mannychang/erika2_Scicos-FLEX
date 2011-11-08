# Script to install the Scicos_pack in batch mode... 
#export ZIPBASE=C:/DOCUME~1/Dario/Desktop/SCICOS~1.1/SCICOS~1.1
#export SCIBASE=C:/PROGRA~1/scilab/SCICOS~1.1

if [ -z "$ZIPBASE" ]; then
  echo WARNING: Missing environment variable ZIPBASE
  exit 1;
fi
  
if [ -z "$SCIBASE" ]; then
  echo WARNING: Missing environment variable SCIBASE
  exit 1;
else
  cp $ZIPBASE/scicos_ee/utils/batch_installer/silent_installer.sce $ZIPBASE
  $SCIBASE/bin/cscilex.exe -nw -nb -f $ZIPBASE/silent_installer.sce > inst_batch.log
fi

