SHARED = ../../shared

CC := gcc
OPT_CARGS :=
CARGS := $(OPT_CARGS) -I$(SHARED)/include

SOURCES = $(shell find . -type f -name '*.c')

.PHONY: all clean run

all: a.out

clean:
	rm -vf a.out

a.out: $(SOURCES)
	cd $(SHARED) && make
	$(CC) $(SOURCES) $(SHARED)/shared.a $(CARGS) -o $@

run: a.out
	@echo -e '\033[01;31mWARNING:\033[00m day-14/part-2 can take a while.'
	./a.out

