CFLAGS += -O2 -Wall -Wno-unused-result
OBJS = jpeg_decoder.o struct.o parsers.o
EXEC = jpeg_decoder

all: $(EXEC)

$(EXEC): $(OBJS)

clean:
	$(RM) $(EXEC) $(OBJS)
