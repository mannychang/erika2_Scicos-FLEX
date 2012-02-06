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
TESTLIST                    += flex
OUTDIR_COMMANDS_flex         = OUTDIR_COMMANDS_flex_source
CLEAN_flex                   = CLEAN_flex_source
CODEGEN_flex                 = CODEGEN_flex_source
COMPILE_flex                 = COMPILE_flex_source

# easylab
TESTLIST                    += easylab
OUTDIR_COMMANDS_easylab      = OUTDIR_COMMANDS_flex_source
CLEAN_easylab                = CLEAN_flex_source
CODEGEN_easylab              = CODEGEN_flex_source
COMPILE_easylab              = COMPILE_flex_source

# -------------------------------------------------------------------

EE_TMPDIR = $(TESTBASE)/testcase/tmp
FILE_LOCK = $(EE_TMPDIR)/flex_manual.lock
RTDRUID_GENERATE_LOCK = $(EE_TMPDIR)/rtdruid_generate_lock.lock
LOCKFILE= lockfile -1 -r-1
DIST_LOCK = $(EE_TMPDIR)/dist.lock

SCI_TIMEOUT ?= 120

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
	cat flex_codegen/batch_simulate.sce | gcc -c - -E -P $$(addprefix -D, $$(shell $$(DEMUX2) $(1))) -D$$(thearch) -DTESTDIR="`cygpath -ms $$(OUTDIR_PREFIX)$(1)`" -o - >$$(OUTDIR_PREFIX)$(1)/batch_simulate_parsed.sce; \
	cd $$(OUTDIR_PREFIX)$(1); \
	timeout $(SCI_TIMEOUT) $(SCIBASE)/bin/cscilex.exe -nw -nb -f "`cygpath -ms $$(OUTDIR_PREFIX)$(1)/batch_codegen_parsed.sce`" >$$(OUTDIR_PREFIX)$(1)/scicoslab_log.txt || echo "codegen failed"

# -------------------------------------------------------------------

# takes the compile.sh script and copies it in the target directory
# creates a script file run_compile.sh which is used to call compile.sh
# with the same parameters used by the codegenerator
# finally runs the compile.sh
COMPILE_flex_source = \
	cat $(SCIBASE)/contrib/scicos_ee/scicos_flex/dspic/compile.sh | dos2unix > $$(OUTDIR_PREFIX)$(1)/test_scig/compile.sh; \
	chmod +x $$(OUTDIR_PREFIX)$(1)/test_scig/compile.sh; \
	cat flex_codegen/run_compile.sh | gcc -c - -E -P $$(addprefix -D, $$(shell $$(DEMUX2) $(1))) -D$$(thearch) -DTESTDIR="`cygpath -ms $$(OUTDIR_PREFIX)$(1)`" -o - >$$(OUTDIR_PREFIX)$(1)/test_scig/run_compile.sh; \
	chmod +x $$(OUTDIR_PREFIX)$(1)/test_scig/run_compile.sh; \
	cd $$(OUTDIR_PREFIX)$(1)/test_scig; \
	./run_compile.sh 2>&1 >$$(OUTDIR_PREFIX)$(1)/compile_log.txt


# -------------------------------------------------------------------
