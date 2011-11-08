# Environment settings
export SCIBASE=C:/PROGRA~2/SCICOS~1.1
export TESTBASE=C:/PROGRA~2/SCICOS~1.1/contrib
export ZIPBASE=C:/DOCUME~1/admin/Desktop/SCICOS~1/SCICOS~1.1/SCICOS~1.1

if [ -z "$ZIPBASE" ]; then
  echo WARNING: Missing environment variable ZIPBASE
  exit 1;
fi
  
if [ -z "$SCIBASE" ]; then
  echo WARNING: Missing environment variable SCIBASE
  exit 1;
fi

if [ -z "$TESTBASE" ]; then
  echo WARNING: Missing environment variable TESTBASE
  exit 1;
fi

# ScicosLab toolbox installation
cp $ZIPBASE/scicos_ee/utils/batch_installer/silent_installer.sce $ZIPBASE
#$SCIBASE/bin/cscilex.exe -nw -nb -f $ZIPBASE/silent_installer.sce > inst_batch.log
$SCIBASE/bin/cscilex.exe -nw -nb -f $ZIPBASE/silent_installer.sce

# Testcase update
#cd $SCIBASE/contrib/scicos_ee
#svn co svn://svn.tuxfamily.org/svnroot/erika/erikae/repos/scilab_codegen/trunk/testcase
cd $TESTBASE/testcase
#svn up

# Easylab section
make ARCH=easylab
#salva qualche info (es. la cartella tmp)
mv tmp/ tmp_easylab

# Flex section
make ARCH=flex
#salva qualche info (es. la cartella tmp)
mv tmp/ tmp_flex
