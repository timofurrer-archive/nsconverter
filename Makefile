# variables
CC     = gcc
LIBS   = -lm
CFLAGS = -Wall
BINDIR = $(DESTDIR)/usr/bin
NAME   = nsconverter

# make, clean & (un)install
all:
	$(CC) $(CFLAGS) -o $(NAME) src/nsconverter.c $(LIBS)

clean:
	rm -f $(NAME)

install:
	install --mode=755 $(NAME) $(BINDIR)/

uninstall:
	rm -f $(BINDIR)/$(NAME)
