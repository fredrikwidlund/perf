PROG    = perf
FLAGS   = -std=gnu11 -g -O3 -Wall -Wextra -flto -fuse-linker-plugin -march=native
CFLAGS  = $(FLAGS) -Ishared
LDFLAGS = $(FLAGS) -ldl
OBJS    = main.o

.PHONY: clean

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(PROG) $(OBJS)
