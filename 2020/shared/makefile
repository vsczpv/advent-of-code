SRC = src
BUILD = build
INCLUDE = include

CC := gcc
CARGS := -I$(INCLUDE)

OBJS = $(shell find $(SRC) -type f -name '*.c' | sed 's/\.c*$$/\.o/;s/$(SRC)\//$(BUILD)\//')
HEADERS = $(shell find $(INCLUDE) -type f -name '*.h')

.PHONY: all clean

all: shared.a

clean:
	rm -vf $(OBJS) shared.a

$(OBJS): $(BUILD)/%.o: $(SRC)/%.c $(HEADERS)
	$(CC) -c $(CARGS) -o $@ $<

shared.a: $(OBJS)
	ar rcs $@ $(OBJS)
