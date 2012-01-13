# source files.
SRC = smcube_block.c  common/process_utils_linux.c common/comm_channel_linux.c common/data_model.c

OBJ = $(SRC:.c=.o)

OUT = libsmcube_block.so.1.0.1

# include directories
INCLUDES = -I. -I/usr/local/include -I/usr/lib/scicoslab-gtk-4.4bz5/routines

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g -Wall # -ansi -pedantic

# compiler
CCC = gcc

# library paths
LIBS = -L. -L/usr/local/lib -lm

# compile flags
LDFLAGS = -g

.SUFFIXES: .c

default: $(OUT)

.c.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(CCC) -shared -Wl,-soname,libsmcube_block.so.1 \
    -o $(OUT) $(OBJ) -lc 
	

clean:
	rm -f $(OBJ) $(OUT)

