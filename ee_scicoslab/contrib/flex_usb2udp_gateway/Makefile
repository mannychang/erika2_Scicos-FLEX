# Modified by chris
# SCICOS FLEX USB MAKEFILE

OBJ_DIR = obj
SRC_DIR = src
INC_DIR = inc

LIB_NAME = libflexusb.a
DLL_NAME = lib_usbdll.dll
CFLAGS = -g -I $(INC_DIR)

OBJS = 	$(OBJ_DIR)/udp_comms.o 
OBJS +=	$(OBJ_DIR)/flex_usb.o 
OBJS += $(OBJ_DIR)/flex_bus.o 
OBJS += $(OBJ_DIR)/flex_usbscicos.o 
#OBJS += 	$(OBJ_DIR)/scicos_flex_usb.o 



.PHONY: all clear


all: $(LIB_NAME) app_flex_scicos


$(LIB_NAME): $(OBJS)
	@echo AR $@
	@ar -r $@ $^
	@echo

$(OBJ_DIR)/udp_comms.o: $(SRC_DIR)/udp_comms.c
	@echo CC $@ 
	@$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/flex_usb.o: $(SRC_DIR)/flex_usb.c
	@echo CC $@ 
	@$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/flex_bus.o: $(SRC_DIR)/flex_bus.c
	@echo CC $@ 
	@$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/flex_usbscicos.o: $(SRC_DIR)/flex_usbscicos.c
	@echo CC $@ 
	@$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/app_flex_scicos.o: $(SRC_DIR)/app_flex_scicos.c
	@echo CC $@ 
	@$(CC) -c $(CFLAGS) $< -o $@

app_flex_scicos: $(OBJ_DIR)/app_flex_scicos.o libflexusb.a 
	@echo BUILD TEST $@
	@$(CC)  -g -o $@ $< -lpthread -L. -lflexusb -lusb
	@echo

clean:
	@rm -rf $(OBJ_DIR)/* $(LIB_NAME) app_flex_scicos.exe app_flex_scicos
	@echo CLEAN COMPLETE
	@echo
