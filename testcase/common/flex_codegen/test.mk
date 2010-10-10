# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2010  Evidence Srl
# 
# This file is part of ERIKA Enterprise.
# 
# ERIKA Enterprise is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation, 
# (with a special exception described below).
# 
# Linking this code statically or dynamically with other modules is
# making a combined work based on this code.  Thus, the terms and
# conditions of the GNU General Public License cover the whole
# combination.
# 
# As a special exception, the copyright holders of this library give you
# permission to link this code with independent modules to produce an
# executable, regardless of the license terms of these independent
# modules, and to copy and distribute the resulting executable under
# terms of your choice, provided that you also meet, for each linked
# independent module, the terms and conditions of the license of that
# module.  An independent module is a module which is not derived from
# or based on this library.  If you modify this code, you may extend
# this exception to your version of the code, but you are not
# obligated to do so.  If you do not wish to do so, delete this
# exception statement from your version.
# 
# ERIKA Enterprise is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License version 2 for more details.
# 
# You should have received a copy of the GNU General Public License
# version 2 along with ERIKA Enterprise; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
# ###*E*###

#
# ScicosLab FLEX codegen
#


#
# configurations
#

# flex
TESTLIST 		+= flex
OUTDIR_COMMANDS_flex 	= OUTDIR_COMMANDS_flex_source
CLEAN_flex           	= CLEAN_flex_source
CODEGEN_flex            = CODEGEN_flex_source
COMPILE_flex         	= COMPILE_flex_source


CONF_flex            	= CONF_flex_source
GLOBAL_CONF 		+=
DIST_flex            	=
RTDRUID_flex		= RTDRUID_flex_source
DEBUG_flex           	= DEBUG_flex_source

# -------------------------------------------------------------------

TMPDIR = $(SCIBASE)/testcase/tmp
FILE_LOCK = $(TMPDIR)/flex_manual.lock
RTDRUID_GENERATE_LOCK = $(TMPDIR)/rtdruid_generate_lock.lock
LOCKFILE= lockfile -1 -r-1
DIST_LOCK = $(TMPDIR)/dist.lock

# -------------------------------------------------------------------

# we cannot use -sf because ScicosLab seems not recognize symlinks.
OUTDIR_COMMANDS_flex_source = \
	( cd $@; cp -f ../test.cos .; \
	);

# -------------------------------------------------------------------

CLEAN_flex_source = \
	(rm -rf $$(OUTDIR_PREFIX)$(1)/*_scig; \
	);

# -------------------------------------------------------------------

# parses the codegen.sce using the option passed in conf.in
# then calls ScicosLab to generate the code WITHOUT executing at the end the compile.sh script
CODEGEN_flex_source = \
	cat flex_codegen/batch_codegen.sce | gcc -c - -E -P $$(addprefix -D, $$(shell $$(DEMUX2) $(1))) -D$$(thearch) -DTESTDIR="`cygpath -ms $$(OUTDIR_PREFIX)$(1)`" -o - >$$(OUTDIR_PREFIX)$(1)/batch_codegen_parsed.sce; \
	cd $$(OUTDIR_PREFIX)$(1); \
	$(SCIBASE)/bin/cscilex.exe -nw -nb -f "`cygpath -ms $$(OUTDIR_PREFIX)$(1)/batch_codegen_parsed.sce`" >$$(OUTDIR_PREFIX)$(1)/scicoslab_log.txt

# -------------------------------------------------------------------

# takes the compile.sh script and copies it in the target directory
# creates a script file run_compile.sh which is used to call compile.sh
# with the same parameters used by the codegenerator
# finally runs the compile.sh
COMPILE_flex_source = \
	cat $(SCIBASE)/contrib/dspic/compile.sh | dos2unix > $$(OUTDIR_PREFIX)$(1)/test_scig/compile.sh; \
	chmod +x $$(OUTDIR_PREFIX)$(1)/test_scig/compile.sh; \
	cat flex_codegen/run_compile.sh | gcc -c - -E -P $$(addprefix -D, $$(shell $$(DEMUX2) $(1))) -D$$(thearch) -DTESTDIR="`cygpath -ms $$(OUTDIR_PREFIX)$(1)`" -o - >$$(OUTDIR_PREFIX)$(1)/test_scig/run_compile.sh; \
	chmod +x $$(OUTDIR_PREFIX)$(1)/test_scig/run_compile.sh; \
	cd $$(OUTDIR_PREFIX)$(1)/test_scig; \
	./run_compile.sh 2>&1 >$$(OUTDIR_PREFIX)$(1)/compile_log.txt


# -------------------------------------------------------------------



# # # These are the commands used by flex_dist_src

# # this simply parses the OIL file and then raises a flag if there is need to generate a source distribution
# CONF_flex_source = \
# 	@echo TMPDIR=$(TMPDIR) \
# 	@echo CONF $$(OUTDIR_PREFIX)$(1); \
# 	cat $$(OUTDIR_PREFIX)$(1)/appl.oil | gcc -c - -E -P -I$$(EEBASE)/pkg $$(addprefix -D, $$(shell $$(DEMUX2) $(1))) -D$$(thearch) -o - >$$(OUTDIR_PREFIX)$(1)/ee.oil; \
# 	touch $$(TMPDIR)/flex_dist_src_buildsourcedistribution.flg;

# # if the flag has been raised, generate the source distribution
# GLOBAL_CONF_flex_source = \
# 	( if test -e tmp/flex_dist_src_buildsourcedistribution.flg; then \
# 		make -C $${EEBASE}/dist/source DIST=flex_TESTCASE flex_MOVE=Y >tmp/flex_dist_src_buildsourcedistribution.log 2>&1; \
# 	fi );

# # Generate the rt-druid files...
# RTDRUID_flex_source = \
# 	@echo RTDRUID $$(OUTDIR_PREFIX)$(1); \
# 	echo \<rtdruid.Oil.Configurator inputfile=\"$$(OUTDIR_PREFIX)$(1)/ee.oil\" outputdir=\"$$(OUTDIR_PREFIX)$(1)/Debug\" /\> >> $$(TMPDIR)/flex_rtdruid_partial.xml;

# # take also a look to GLOBAL_RTDRUID at the top of the file!!!

# COMPILE_flex_source = +if $$(MAKE) $(PARAMETERS) NODEPS=1 -C $$(OUTDIR_PREFIX)$(1)/Debug >$$(OUTDIR_PREFIX)$(1)/compile.log 2>&1; then echo OK $$(EXPERIMENT) $$(OUTDIR_PREFIX)$(1) >>$$(TMPDIR)/ok.log; else echo ERROR $$(EXPERIMENT) $$(OUTDIR_PREFIX)$(1) >>$$(TMPDIR)/errors.log; fi

# DEBUG_flex_source = \
# 	@cp flex/t32.cmm $$(OUTDIR_PREFIX)$(1); \
# 	$$(LOCKFILE) $$(FILE_LOCK); \
# 		echo chdir $$(OUTDIR_PREFIX)$(1) >> $$(TMPDIR)/t32_jobs.cmm; \
# 		echo "write \#1 \"$$(OUTDIR_PREFIX)$(1)\"" >> $$(TMPDIR)/t32_jobs.cmm; \
# 		echo area.select Messages >> $$(TMPDIR)/t32_jobs.cmm; \
# 		echo print >> $$(TMPDIR)/t32_jobs.cmm; \
# 		echo print \"$$(OUTDIR_PREFIX)$(1)\" >> $$(TMPDIR)/t32_jobs.cmm; \
# 		echo area.select A000 >> $$(TMPDIR)/t32_jobs.cmm; \
# 		echo do t32.cmm >> $$(TMPDIR)/t32_jobs.cmm; \
# 		cp -u flex/t32_quit.cmm $$(TMPDIR)/t32.cmm; \
# 	rm -f $$(FILE_LOCK);

