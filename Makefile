PROG   = perf
CFLAGS = -Wall -Werror -pedantic -O3

.PHONY: clean

$(PROG):

clean:
	rm -f $(PROG) *.o
