CFLAGS += -std=c99 -O2 -Wall -Wno-unused-result
LDLIBS += -Llibbmp/src/.libs -lbmp -lm
LDFLAGS += -static
OBJS = struct.o parsers.o utils.o huffman.o scan.o dct.o output.o encode.o
EXEC = jpeg_decoder jpeg_encoder

all: $(EXEC)

$(EXEC): $(OBJS)

clean:
	$(RM) $(EXEC) $(OBJS)
