@set EE_BASH_PATH=C:\cygwin\bin\bash

@%EE_BASH_PATH% -c "/bin/bash --login -c \"cd `/bin/cygpath/ -ms \"$PWD\"`; ./scicos_build.sh\""