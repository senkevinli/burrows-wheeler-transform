default: all

CC := gcc
CFLAGS :=
DIR := $(shell pwd)

.PHONY: check
check:
	$(CC) -o test.o $(DIR)/src/bwt.c $(DIR)/test/bwttest.c -lcunit
	./test.o

.PHONY: all
all:
	$(CC) -o bwt.o $(DIR)/src/bwt.c $(DIR)/src/main.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o