#### GLOBAL VARIABLES ####
NAME 		:= TEST_USB
EXEC_EXTENSION 	:= cof
OUTDIR 		:= $(shell pwd)/Debug

#### ERIKA LOCAL TREE: SVN VERSION ####		
RTDRUID := /cygdrive/c/develop/svn_evidence/bin/rtdruid_launcher.bat

#### ERIKA PUBLIC TREE: DISTRIBUTION VERSION ####
#RTDRUID	:= /cygdrive/c/Evidence/Evidence/bin/rtdruid_launcher.bat

all: 
	$(RTDRUID) --oil conf.oil --outdir $(OUTDIR)
	cd $(OUTDIR); make && \
	cp -v pic30.$(EXEC_EXTENSION) ../pic30-$(NAME).$(EXEC_EXTENSION); \
	cd -;

clean:
	rm -rf $(OUTDIR)

