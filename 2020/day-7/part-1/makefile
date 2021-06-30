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
	$(CC) $(CARGS) $(SOURCES) $(SHARED)/shared.a -o $@

run: a.out
	@echo -e '\033[01;31mWARNING:\033[00m day-7/part-1 is \033[5;03mreally\033[00m slow.'
	./a.out

