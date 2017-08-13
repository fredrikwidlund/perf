LIB         = glue_libdynamic.so
FLAGS       = -std=gnu11 -g -O3 -Wall -Wextra -pedantic -flto -fuse-linker-plugin -march=native
CFLAGS      = $(FLAGS) -I../shared -I../implementation/libdynamic
LDFLAGS     = $(FLAGS) -shared -fPIC -fno-semantic-interposition
OBJS        = glue_libdynamic.o ../implementation/libdynamic/map.o ../shared/suite.o

$(LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(LIB) $(OBJS)
