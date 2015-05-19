CFLAGS += -std=gnu99 -O2 -Wall -Wno-unused-result
OBJS = jpeg_decoder.o struct.o parsers.o utils.o huffman.o scan.o
EXEC = jpeg_decoder

all: $(EXEC)

$(EXEC): $(OBJS)

clean:
	$(RM) $(EXEC) $(OBJS)
