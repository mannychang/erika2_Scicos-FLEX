# Modified by chris
# SCICOS FLEX USB MAKEFILE

OBJ_DIR = obj
SRC_DIR = src
INC_DIR = inc

LIB_NAME = libflexusb.a
TEST_EXE = test
CFLAGS = -g -I $(INC_DIR)

OBJS = 	$(OBJ_DIR)/scicos_usb.o 
OBJS += $(OBJ_DIR)/channels.o 


.PHONY: all clear


all: $(LIB_NAME) $(TEST_EXE)


libflexusb.a: $(OBJS)
	@echo AR $@
	@ar -r $@ $^
	@echo

$(OBJ_DIR)/scicos_usb.o: $(SRC_DIR)/scicos_usb.c
	@echo CC $@ 
	@$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/channels.o: $(SRC_DIR)/channels.c
	@echo CC $@ 
	@$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/test.o: $(SRC_DIR)/test.c
	@echo CC $@ 
	@$(CC) -c $(CFLAGS) $< -o $@

test: $(OBJ_DIR)/test.o libflexusb.a
	@echo BUILD TEST $@
	@$(CC)  -g -o $@ $< -lpthread -L. -lflexusb -lusb
	@echo

clean:
	@rm -rf $(OBJ_DIR)/* $(LIB_NAME) $(TEST_EXE).exe $(TEST_EXE)
	@echo CLEAN COMPLETE
	@echo
