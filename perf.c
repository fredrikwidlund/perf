#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <time.h>

typedef struct value value;
struct value
{
  char *key;
  char data[64];
};

typedef struct pair pair;
struct pair
{
  char *key;
  void *value;
};

static uint64_t nano_time(void)
{
  struct timespec ts;

  (void) clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  return ((uint64_t) ts.tv_sec * 1000000000) + ((uint64_t) ts.tv_nsec);
}

static inline unsigned python_hash(const char *s)
{
  unsigned h;
  const unsigned char * cp = (const unsigned char *) s;
  h = * cp << 7;
  while (* cp)
    h = (1000003 * h) ^ * cp ++;
  h ^= (unsigned) (cp - (const unsigned char *) s);
  return h;
}

void shuffle(pair *p, size_t n)
{
  size_t i, j;
  pair tmp;

  for (i = n - 1; i > 0; i --)
    {
      j = random() % (i + 1);
      tmp = p[i];
      p[i] = p[j];
      p[j] = tmp;
    }
}

pair *data(size_t n)
{
  size_t i;
  char key[64];
  value *v;

  pair *p = calloc(n, sizeof *p);

  for (i = 0; i < n; i ++)
    {
      snprintf(key, sizeof key, "%lu", i);
      p[i].key = strdup(key);
      v = malloc(sizeof *v);
      v->key = p[i].key;
      p[i].value = v;
    }
  shuffle(p, n);
  return p;
}

#include "perf_htable.c"
#include "perf_map.c"

int main()
{
  size_t n = 100000;
  pair *pairs = data(n);

  printf("htable grow %lu\n", htable_grow(pairs, n));
  printf("htable hit %lu\n", htable_hit(pairs, n));
  printf("htable kbench %lu\n", htable_kbench(pairs, n));

  printf("map grow %lu\n", map_pair_grow(pairs, n));
  printf("map hit %lu\n", map_pair_hit(pairs, n));
  printf("map kbench %lu\n", map_pair_kbench(pairs, n));
}
