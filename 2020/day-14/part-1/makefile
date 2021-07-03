SHARED = ../../shared

CC := gcc
OPT_CARGS :=
CARGS := $(OPT_CARGS) -I$(SHARED)/include

SOURCES = $(shell find . -type f -name '*.c')

.PHONY: all clean shared run

all: a.out

clean:
	rm -vf a.out

shared:
	cd $(SHARED) && make

a.out: shared $(SOURCES)
	$(CC) $(SOURCES) $(SHARED)/shared.a $(CARGS) -o $@

run: a.out
	./a.out

