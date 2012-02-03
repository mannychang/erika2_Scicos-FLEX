# Author: 2003- Paolo Gai
# CVS: $Id: makefile,v 1.16 2006/09/04 06:37:56 pj Exp $

# Scicoslab codegen testcases

# the structure of these makefiles are similar to the 
# regression tests used for ERIKA Enterprise

# COMMENTS HAVE BEEN REMOVED FROM THIS MAKEFILES. SEE THE ERIKA
# ENTERPRISE TESTCASE MAKEFILES for more fdetails!

# the idea is to have one directory for each test, which is compiled
# batch to ensure that everything works fine.

normalize_path=$(shell cygpath -u "$(shell cygpath -d "$1")")

ifndef SCIBASE
missing_scibase:
else
export SCIBASE:=$(call normalize_path,$(SCIBASE))
endif

ifndef ARCH
help:
endif

ifndef TESTBASE
export TESTBASE:=$(SCIBASE)
warning_missing_testbase: missing_testbase all
else
export TESTBASE:=$(call normalize_path,$(TESTBASE))
endif

NOPRINTDIR = --no-print-directory

ifndef DIRS
DIRS := $(dir $(wildcard */conf.in))
endif

ifneq (ok,$(shell test -x "$(SCIBASE)/contrib/scicos_ee/bin/SMCube" && echo ok ))
$(warning SMCube.exe not found!)
DIRS := $(filter-out $(dir $(wildcard smcube*/conf.in)),$(DIRS))
endif

# remove duplicates and add trailing / if missing
SDIRS = $(addsuffix /,$(subst /,,$(sort $(DIRS))))

.PHONY: help clean missing_scibase missing_testbase

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
# analyze
#
# is the target that is used to analyze the results of the various blocks.
#
# the results are:
#
# tmp/globalblocklist.txt
#     contains the pair (test_directory, block_contained)
#
# tmp/globalpalettelist.txt
#     contains the pair (palette, block_contained)
#
# tmp/globalcoverage.txt
#     contains a text report with, for each block, 
#     the directories containing it and the palettes containing it
#
.PHONY: analyze
analyze:  analyze_palettes
	echo ANALYZE ended successfully!		;\

# rule used to print the blocks stored in each example
define analyze_template
.PHONY: analyze_$(1)
analyze_$(1): tmp
	+@$$(MAKE) $$(NOPRINTDIR) -C common -f arch_multiplexer.mk EXPERIMENT=$(1) analyze
endef
$(foreach d,$(SDIRS),$(eval $(call analyze_template,$(d))))

# this rule prints the palette content for each palette
analyze_dirs := $(addprefix analyze_, $(SDIRS))
analyze_palettes: tmp $(analyze_dirs)
	@echo ANALYZE_PALETTE

# creates tmp/palettelist, which contains all pair (palette_file,block)
	@echo ANALYZE_PALETTE Creating global palette list
	@for p in `find $(SCIBASE)/contrib/scicos_ee/scicos_flex/dspic/macros/palettes | grep cosf | grep -v svn`; do \
          echo Palette $${p}; \
          cd $(TESTBASE)/testcase/tmp; \
	  cp -f $${p} ./mypalette.cosf; \
	  chmod +r ./mypalette.cosf; \
	  echo ---------- >>$(TESTBASE)/testcase/tmp/scicoslab_log.txt ;\
          cat $(TESTBASE)/testcase/common/flex_codegen/batch_palette_info.sce | gcc -c - -E -P -DTESTDIR="`cygpath -ms $(TESTBASE)/testcase/tmp`" -o - >$(TESTBASE)/testcase/tmp/batch_palette_info_parsed.sce; \
          $(SCIBASE)/bin/cscilex.exe -nw -nb -f "`cygpath -ms $(TESTBASE)/testcase/tmp/batch_palette_info_parsed.sce`" >>$(TESTBASE)/testcase/tmp/scicoslab_log.txt; \
          export myscilabfile=$${p}; \
          cat $(TESTBASE)/testcase/tmp/blocklist.txt | awk '{ print ENVIRON["myscilabfile"], $$1}' >> $(TESTBASE)/testcase/tmp/globalpalettelist.txt; \
        done

# creates tmp/globalblocklist, which contains all pair (diagram_file,block)
	@echo ANALYZE_PALETTE Creating global block list
	@cat */*/blocklist.txt | sort | uniq > $(TESTBASE)/testcase/tmp/globalblocklist.txt

# creates coverage
	@echo ANALYZE_PALETTE Creating global block coverage
	@rm -f $(TESTBASE)/testcase/tmp/globalcoverage.txt
	@for x in `cat $(TESTBASE)/testcase/tmp/globalpalettelist.txt | awk '{print $$2}'`; do \
          echo ------------ >> $(TESTBASE)/testcase/tmp/globalcoverage.txt; \
          echo $${x} >> $(TESTBASE)/testcase/tmp/globalcoverage.txt; \
          echo >> $(TESTBASE)/testcase/tmp/globalcoverage.txt; \
          grep $${x} $(TESTBASE)/testcase/tmp/globalpalettelist.txt | awk '{print "  Palette " $$1}' >> $(TESTBASE)/testcase/tmp/globalcoverage.txt;\
          grep $${x} $(TESTBASE)/testcase/tmp/globalblocklist.txt | awk '{print "    Diagram " $$1}' >> $(TESTBASE)/testcase/tmp/globalcoverage.txt;\
        done

#
# Hudson
#
# this generates a file named tmp/results.xml with the result for each demo compiled
#




# rule used to print an XML result for each demo that can be compiled
.PHONY: hudson_print_dirs
print_dirs := $(addprefix print_, $(SDIRS))
hudson_print_dirs: $(print_dirs)

define print_template
.PHONY: print_$(1)
print_$(1): tmp
	+@$$(MAKE) $$(NOPRINTDIR) -C common -f arch_multiplexer.mk EXPERIMENT=$(1) print
endef
$(foreach d,$(SDIRS),$(eval $(call print_template,$(d))))

.PHONY: hudson
hudson: tmp
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >tmp/results.xml
	@echo "<testsuites tests=\"\" failures=\"\" errors=\"\" time=\"\" name=\"$(ARCH)\">" >>tmp/results.xml
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >tmp/not_tested.xml
	@echo "<testsuites tests=\"\" failures=\"\" errors=\"\" time=\"\" name=\"$(ARCH)\">" >>tmp/not_tested.xml
# check each single example
	@$(MAKE) $(NOPRINTDIR) -C . -f makefile hudson_print_dirs

# add an entry for each block that has no tests
	@for x in `cat $(TESTBASE)/testcase/tmp/globalpalettelist.txt | awk '{print $$2}'`; do \
          echo Checking Block $${x}; \
	if (test -z "`grep $${x} $(TESTBASE)/testcase/tmp/globalblocklist.txt`") then \
		if (test -z "`grep $${x} $(TESTBASE)/testcase/hudson_ignoreblocks.txt`") then \
			echo "  <testsuite name=\"scicos_block__$${x}\" tests=\"\" failures=\"\" errors=\"\" time=\"\">" >> tmp/not_tested.xml;\
			echo "    <testcase name=\"$${x}\" status=\"run\" time=\"\" classname=\"scicos_block.$${x}\">" >> tmp/not_tested.xml;\
			echo "    </testcase>" >> tmp/not_tested.xml;\
			echo "  </testsuite>" >> tmp/not_tested.xml;\
			echo "" >> tmp/not_tested.xml;\
		fi \
	fi \
	done
	@echo "</testsuites>" >>tmp/not_tested.xml
	@echo "</testsuites>" >>tmp/results.xml


#
# Generating the TMP dir
#
.PHONY: tmp
tmp:
	@echo tmp directory
	@mkdir -p tmp








clean:
	rm -rf `ls -d */out*` tmp



#
# supported
#
# makes a list of projects supporting a specific ARCH


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


#
# miscellaneous
#


include $(wildcard common/*/test.mk)
help:
	@cat readme
	@$(foreach t, $(TESTLIST), echo make ARCH=$(t);)

missing_scibase:
	@echo ERROR: Missing environment variable SCIBASE
	@echo
	@echo ...maybe you can try with the following command:
	@echo export SCIBASE=`pwd`
	
missing_testbase:
	@echo WARNING: Missing environment variable TESTBASE
	@echo
	@echo ...maybe you can try with the following command:
	@echo export TESTBASE=`pwd`/..
	@echo
	@echo for this execution TESTBASE is $(TESTBASE)/testcase
	@echo
