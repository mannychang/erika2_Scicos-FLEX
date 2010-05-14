# # Date             : 3/5/2010
# RT-Druid version : 
#

# these lines override the .bashrc definitions
#EEBASE = ../../..
#APPBASE = .

###
### Setting EEOPT variable:
###

EEOPT =


############################################################################
#
# User options
#
############################################################################
EEOPT += DEBUG
EEOPT += NODEPS
EEOPT += __ADD_LIBS__


############################################################################
#
# Automatic options
#
############################################################################
EEOPT += __RTD_CYGWIN__
EEOPT += __PIC30__
EEOPT += __MICROCHIP_DSPIC30__
EEOPT += __PIC30_ICD2__
EEOPT += __PIC30_SPLIM__
EEOPT += __33FJ256MC710__
EEOPT += __EE_FLEX__
EEOPT += __FP__
EEOPT += __MONO__
EEOPT += __ALARMS__
EEOPT += __ALLOW_NESTED_IRQ__


############################################################################
#
# PIC 30
#
############################################################################
ifndef EEBASE
    EEBASE  := $(shell cygpath `cygpath -ms "C:\Evidence\eclipse\plugins\com.eu.evidence.ee_1.5.1.201003011152\ee_base"`)
endif
APPBASE := ..
OUTBASE := Debug

PIC30_ASMDIR := $(shell $(EEBASE)/pkg/cfg/arch/cc_pic30_detect.sh ASM "c:\Programmi\Microchip\MPLAB ASM30 Suite")
PIC30_GCCDIR := $(shell $(EEBASE)/pkg/cfg/arch/cc_pic30_detect.sh GCC "c:\Programmi\Microchip\MPLAB C30")


############################################################################
#
# Init MCU for PIC30 (PIC33FJ256MC710)
#
############################################################################
PIC30_MODEL           := 33FJ256MC710
PIC30_LINKERSCRIPT    := p33FJ256MC710.gld
PIC30_DEV_LIB         := libp33FJ256MC710-elf.a
PIC30_INCLUDE_C       := p33FJ256MC710.h
PIC30_INCLUDE_S       := p33FJ256MC710.inc


############################################################################
#
# Flags
#
############################################################################
CFLAGS  += -mlarge-data
ASFLAGS += 
LDFLAGS += 
LDDEPS  += 
LIBS    += 

LIB_OPT  += __LIB_SCICOS__


############################################################################
#
# Sources
#
############################################################################
APP_SRCS += $(OUTBASE)/eecfg.c dspic_main.c common.c


############################################################################
#
# end
#
############################################################################
include $(EEBASE)/pkg/cfg/rules.mk
