CC = gcc
CARGS =

.PHONY: day-*/part-*

help:
	@echo
	@echo 'You can compile and run each day & part by running "make day-N/part-N", N being its number'
	@echo
	@echo 'For example, to run day 7, part 2: make day-7/part-2'
	@echo
	@echo "You can alternatively run make all, but it's not recommended"
	@echo
	
all: day-*/part-*
	
day-*/part-*:
	cd '$@' && CC=$(CC) OPT_CARGS=$(CARGS) make run
