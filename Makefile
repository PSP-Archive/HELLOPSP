
TITLE = "Hello World(V1.01 GCC4.0.0)"

#DEFS += -DUSE_GPU
#DEFS += -DDEBUG

PBP = EBOOT.PBP
BINARY = out
OBJS = startup.o hellopsp.o pg.o 
LIBS = -lc

BINARY   = hw.out
BINARY_P = hw.outp
OUTPATCH = ./outpatch.exe
ELF2PBP  = ./elf2pbp.exe
ICON0    = ICON0.PNG

all: $(PBP)

$(PBP): $(BINARY)
	$(OUTPATCH) $(BINARY) $(BINARY_P) "USERPROG"
	$(ELF2PBP) $(BINARY_P) $(TITLE) $(ICON0)

$(BINARY): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	$(STRIP) $(BINARY)

%.o: %.c
	$(CC) $(DEFS) $(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(DEFS) $(ARCHFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.map $(BINARY) $(BINARY_P) 

include Makefile.psp
