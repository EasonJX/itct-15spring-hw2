CFLAGS += -std=gnu99 -O2 -Wall -Wno-unused-result
LDLIBS += -Llibbmp/src/.libs -lbmp -lm
LDFLAGS += -static
OBJS = jpeg_decoder.o struct.o parsers.o utils.o huffman.o scan.o dct.o output.o
EXEC = jpeg_decoder

all: $(EXEC)

$(EXEC): $(OBJS)

clean:
	$(RM) $(EXEC) $(OBJS)
