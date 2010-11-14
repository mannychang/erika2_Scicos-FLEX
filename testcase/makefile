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
# Analyzing the target blocks
#
.PHONY: analyze
analyze_dirs := $(addprefix analyze_, $(SDIRS))
analyze: $(analyze_dirs) analyze_palettes
	echo ANALYZE ended successfully!		;\


define analyze_template
.PHONY: analyze_$(1)
analyze_$(1): tmp
	+@$$(MAKE) $$(NOPRINTDIR) -C common -f arch_multiplexer.mk EXPERIMENT=$(1) analyze

endef
$(foreach d,$(SDIRS),$(eval $(call analyze_template,$(d))))


#
# Palettes analysis
#

analyze_palettes: tmp
	@echo ANALYZE_PALETTE

# creates tmp/palettelist, which contains all pair (palette_file,block)
	@echo ANALYZE_PALETTE Creating global palette list
	@for p in `find /cygdrive/c/Evidence/scicoslab-44b7/contrib/dspic/macros/palettes | grep cosf | grep -v svn`; do \
          echo Palette $${p}; \
          cd $(SCIBASE)/testcase/tmp; \
	  cp -f $${p} ./mypalette.cosf; \
	  chmod +r ./mypalette.cosf; \
	  echo ---------- >>$(SCIBASE)/testcase/tmp/scicoslab_log.txt ;\
          cat $(SCIBASE)/testcase/common/flex_codegen/batch_palette_info.sce | gcc -c - -E -P -DTESTDIR="`cygpath -ms $(SCIBASE)/testcase/tmp`" -o - >$(SCIBASE)/testcase/tmp/batch_palette_info_parsed.sce; \
          $(SCIBASE)/bin/cscilex.exe -nw -nb -f "`cygpath -ms $(SCIBASE)/testcase/tmp/batch_palette_info_parsed.sce`" >>$(SCIBASE)/testcase/tmp/scicoslab_log.txt; \
          export myscilabfile=$${p}; \
          cat $(SCIBASE)/testcase/tmp/blocklist.txt | awk '{ print ENVIRON["myscilabfile"], $$1}' >> $(SCIBASE)/testcase/tmp/globalpalettelist.txt; \
        done

# creates tmp/globalblocklist, which contains all pair (diagram_file,block)
	@echo ANALYZE_PALETTE Creating global block list
	@cat */*/blocklist.txt | sort | uniq > $(SCIBASE)/testcase/tmp/globalblocklist.txt

# creates coverage
	@echo ANALYZE_PALETTE Creating global block coverage
	@rm -f $(SCIBASE)/testcase/tmp/globalcoverage.txt
	@for x in `cat $(SCIBASE)/testcase/tmp/globalpalettelist.txt | awk '{print $$2}'`; do \
          echo ------------ >> $(SCIBASE)/testcase/tmp/globalcoverage.txt; \
          echo $${x} >> $(SCIBASE)/testcase/tmp/globalcoverage.txt; \
          echo >> $(SCIBASE)/testcase/tmp/globalcoverage.txt; \
          grep $${x} $(SCIBASE)/testcase/tmp/globalpalettelist.txt | awk '{print "  Palette " $$1}' >> $(SCIBASE)/testcase/tmp/globalcoverage.txt;\
          grep $${x} $(SCIBASE)/testcase/tmp/globalblocklist.txt | awk '{print "    Diagram " $$1}' >> $(SCIBASE)/testcase/tmp/globalcoverage.txt;\
        done


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
