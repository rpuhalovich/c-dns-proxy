CC=gcc

# can select build with: make -B build=<build>
build:=deb
ifeq ($(build), deb)
	CFLAGS=-Wall -Wpedantic -g
else ifeq ($(build), san)
	CFLAGS=-Wall -Wpedantic -g -fsanitize=address -fsanitize-address-use-after-scope
else ifeq ($(build), rel)
	CFLAGS:=-O2
endif

LIB:=#-lm

BIN_PHASE1=phase1
BIN_PHASE2=dns_svr
args=172.26.129.247 5353

# --- Sources ------------------------------------------------------------------
OBJDIR=obj
SRCDIR=src
SHAREDDIR=$(SRCDIR)/shared

SRC=$(wildcard $(SHAREDDIR)/*.c)
OBJ=$(subst $(SHAREDDIR)/,$(OBJDIR)/,$(SRC:.c=.o))
PHASE1OBJ:=$(OBJDIR)/$(BIN_PHASE1).o $(OBJ)
PHASE2OBJ:=$(OBJDIR)/$(BIN_PHASE2).o $(OBJ)

INC=-I$(SHAREDDIR)

# --- Targets ------------------------------------------------------------------
TARGETFLAGS=$(CC) $(CFLAGS) $(INC)

all: dir $(BIN_PHASE1) $(BIN_PHASE2)

$(BIN_PHASE1): $(PHASE1OBJ)
	$(TARGETFLAGS) -o $@ $^ $(LIB)

$(BIN_PHASE2): $(PHASE2OBJ)
	$(TARGETFLAGS) -o $@ $^ $(LIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(TARGETFLAGS) -c $^ -o $@ $(LIB)

$(OBJDIR)/%.o: $(SHAREDDIR)/%.c
	$(TARGETFLAGS) -c $^ -o $@ $(LIB)

san:
	make -B build=san

run:
	make -B build=deb && clear 
	./$(BIN_PHASE2) $(args)

gdb:
	make -B build=deb && gdb --args $(BIN_PHASE2) $(args)

clean:
	rm -rf *.dSYM *.bin dns_svr.log $(OBJDIR) $(BIN_PHASE1) $(BIN_PHASE2)

dir:
	mkdir -p $(OBJDIR)

.PHONY: all clean
