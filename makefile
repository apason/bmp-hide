CC= gcc
STD= _GNU_SOURCE_
LDFLAGS= 
CFLAGS= -Werror -Wall
OBJECTS= main.o bitwise.o options.o files.o functions.o
BINARY= bmp_hide

.c.o:
	$(CC) -D$(STD) $(CFLAGS) -c $<

all:	$(BINARY)

$(BINARY):	$(OBJECTS)
	$(CC) -o $(BINARY) $(LDFLAGS) $(OBJECTS)

clean:
	rm -f *.o

clobber:	clean
	rm -f $(BINARY)
