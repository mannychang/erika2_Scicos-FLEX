# Author: 2003- Paolo Gai
# CVS: $Id: makefile,v 1.16 2006/09/04 06:37:56 pj Exp $

# Scicoslab codegen testcases

# the structure of these makefiles are similar to the 
# regression tests used for ERIKA Enterprise

# COMMENTS HAVE BEEN REMOVED FROM THIS MAKEFILES. SEE THE ERIKA
# ENTERPRISE TESTCASE MAKEFILES for more fdetails!

# the idea is to have one directory for each test, which is compiled
# batch to ensure that everything works fine.

ifndef SCIBASE
missing_scibase::
endif


ifndef ARCH
help::
endif

NOPRINTDIR = --no-print-directory

ifndef DIRS
DIRS := $(dir $(wildcard */conf.in))
endif

# remove duplicates and add trailing / if missing
SDIRS = $(addsuffix /,$(subst /,,$(sort $(DIRS))))

.PHONY: help clean

#
# Compiling the targets
#
.PHONY: all
all_dirs := $(addprefix all_, $(SDIRS))
all: $(all_dirs)
# create the compile_all.log
	@for x in `find . -iname compile.log`; do echo ----- >>tmp/compile_all.log; echo $${x} >>tmp/compile_all.log; cat $${x} >>tmp/compile_all.log; done
# print the errors.log
	@if test -e tmp/errors.log; then \
		echo ERRORS				;\
		echo ---------------------------------	;\
		cat tmp/errors.log			;\
		echo ---------------------------------	;\
		echo Use this command to redo the erroneous testcases ;\
		echo make ARCH=$(ARCH) DIRS=\"\\        ;\
		awk '{ print $$2, "\\" }' tmp/errors.log;\
		echo \"                                 ;\
		echo ---------------------------------	;\
	else						\
		echo ALL ended successfully!		;\
	fi




define all_template
.PHONY: all_$(1)
all_$(1): tmp
	+@$$(MAKE) $$(NOPRINTDIR) -C common -f arch_multiplexer.mk EXPERIMENT=$(1) all

endef
$(foreach d,$(SDIRS),$(eval $(call all_template,$(d))))



#
# Generating the TMP dir
#
.PHONY: tmp
tmp:
	@echo RM tmp directory
	@rm -rf tmp
	@mkdir -p tmp








clean:
	rm -rf `ls -d */out*` tmp

ifdef ARCH
supported:
	@for x in $(wildcard */conf.in); do grep $(ARCH) $${x} | xargs echo $${x} ------; done
else
supported:
	@echo Please specify a value for ARCH
	@echo Example:
	@echo
	@echo make supported ARCH=pic30
endif

include $(wildcard common/*/test.mk)
help::
	@cat readme
	@$(foreach t, $(TESTLIST), echo make ARCH=$(t);)

missing_scibase::
	@echo ERROR: Missing environment variable SCIBASE
	@echo ...maybe you can try with the following command:
	@echo export SCIBASE=`pwd`/..
