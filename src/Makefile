IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -D_GNU_SOURCE -ggdb -pedantic-errors -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include

ODIR=../obj
LDIR =../lib

LIBS=/usr/local/lib/libZydis.a -lelf -lglib-2.0

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o regs.o commands.o utils.o mem.o disassembly.o symbols.o breakpoints.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rmdbg: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
