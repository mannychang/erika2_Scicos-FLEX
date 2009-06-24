MAIN_DIR := $(shell pwd)
SRC_DIR  := src
#INC_DIR  := "$(shell cygpath -d "$(MAIN_DIR)\inc")"
INC_DIR  := inc
OBJ_DIR  := obj
LNK_DIR  := lnk
LNK_SCRIPT := $(LNK_DIR)/FLEX_USB.lkr
MAP_FILE   := FLEX_USB_PIC18.map
OUT_FILE   := FLEX_USB_PIC18.cof

CC := mcc18
LINKER := mplink

ARCH := 18F2550 
CC_INCLUDES := "C:\Programmi\Microchip\MCC18\h"
LINKER_DIRS := "C:\Programmi\Microchip\MCC18\lib"

VPATH	:= $(SRC_DIR) $(PROG_DIR)
SRC_FILES  := $(shell ls $(SRC_DIR)/*.c;cd $(CURR_DIR);)
OBJ_FILES  := ${SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o}

#mcc18.exe -p=18F2550 /i"C:\develop\svn_feanor\flex\flex2pc_usb\pic18\autofiles" -I"C:\Programmi\Microchip\MCC18\h" "usbdsc.c" -fo="_output\usbdsc.o" -O-


all: $(OBJ_FILES) 
	$(LINKER) /l$(LINKER_DIRS) /k$(LNK_DIR) $(LNK_SCRIPT) $(OBJ_FILES) /z__MPLAB_BUILD=1 /m$(MAP_FILE) /o$(OUT_FILE)

clean: 
	rm -rf $(OBJ_DIR)/*.o
	rm -rf *.i *.err
	
#C:\Programmi\Microchip\MCC18\bin\mplink.exe" /l"C:\Programmi\Microchip\MCC18\lib" /k"C:\Evidence\Usb\pic18" "..\FLEX_USB.lkr" "..\autofiles\_output\usbdsc.o" "..\system\usb\class\hid\_output\hid.o" "..\system\usb\usb9\_output\usb9.o" "..\system\usb\usbctrltrf\_output\usbctrltrf.o" "..\system\usb\usbdrv\_output\usbdrv.o" "..\system\usb\_output\usbmmap.o" "..\user\_output\pk_isr.o" "main.o" /z__MPLAB_BUILD=1 /m"FLEX_USB.map" /o"FLEX_USB.cof"
	



################################################
#       Overriding Implict Build Rule          #
################################################

$(OBJ_DIR)/%.o : %.c 
	$(CC) -p=$(ARCH) -I$(CC_INCLUDES) -I$(INC_DIR) $< -fo=$@ -O-


################################################
