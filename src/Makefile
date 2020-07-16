IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -D_GNU_SOURCE -ggdb

ODIR=../obj
LDIR =../lib

LIBS=/usr/local/lib/libZydis.a

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o regs.o commands.o utils.o mem.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rmdbg: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
