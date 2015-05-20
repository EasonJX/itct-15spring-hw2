CFLAGS += -std=gnu99 -O2 -Wall -Wno-unused-result
LDLIBS += -lm
OBJS = jpeg_decoder.o struct.o parsers.o utils.o huffman.o scan.o dct.o
EXEC = jpeg_decoder

all: $(EXEC)

$(EXEC): $(OBJS)

clean:
	$(RM) $(EXEC) $(OBJS)
