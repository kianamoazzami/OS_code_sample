CC=gcc
CFLAGS=-Wall -g
LD=gcc
LDFLAGS = -g -lz

SRCS = findpng.c pnginfo.c catpng.c crc.c zutil.c functions.c
LIB_UTIL = crc.o zutil.o functions.o
OBJS_FINDPNG = findpng.o  $(LIB_UTIL)
OBJS_PNGINFO = pnginfo.o $(LIB_UTIL)
OBJS_CATPNG = catpng.o $(LIB_UTIL)

all: findpng.out pnginfo.out catpng.out

findpng.out: $(OBJS_FINDPNG)
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS)

pnginfo.out: $(OBJS_PNGINFO)
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS)

catpng.out: $(OBJS_CATPNG)
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.d: %.c
	gcc -MM -MF $@ $< 

-include $(SRCS:.c=.d)

.PHONY: clean
clean:
	rm -f *.d *.o *.out