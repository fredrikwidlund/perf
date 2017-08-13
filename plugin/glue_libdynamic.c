#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "value.h"
#include "pair.h"
#include "map.h"

typedef struct map glue;

typedef struct map_pair map_pair;
struct map_pair
{
  char  *key;
  value *value;
};

static map_pair map_pair_empty = {NULL, NULL};

static unsigned python_hash(const char *s)
{
  unsigned h;
  const unsigned char * cp = (const unsigned char *) s;
  h = * cp << 7;
  while (* cp)
    h = (1000003 * h) ^ * cp ++;
  h ^= (unsigned) (cp - (const unsigned char *) s);
  return h;
}

static void map_pair_set(map *m, void *dst, void *src)
{
  map_pair *d = dst, *s = src;

  (void) m;
  *d = *s;
}

static int map_pair_equal(map *m, void *obj1, void *obj2)
{
  map_pair *p1 = obj1, *p2 = obj2;

  (void) m;
  return p1 -> key == p2 -> key || (p1 -> key && p2 -> key && ! strcmp (p1 -> key, p2 -> key));
}

static size_t map_pair_hash(map *m, void *obj)
{
  map_pair *p = obj;

  (void) m;
  return python_hash(p->key);
}

int glue_test_lto(int n)
{
  return n + 1;
}

glue *glue_alloc(void)
{
  map *m;

  m = malloc(sizeof *m);
  assert(m);
  map_construct(m, sizeof(map_pair), &map_pair_empty, map_pair_set);
  return m;
}

void glue_free(map *m)
{
  map_clear(m, map_pair_equal, map_pair_set, NULL);
}

void glue_set(map *m, char *key, void *value)
{
  map_insert(m, (map_pair[]){{.key = key, .value = value}}, map_pair_hash, map_pair_equal, map_pair_set, NULL);
}

void *glue_get(glue *m, char *key)
{
  map_pair *p;

  p = map_at(m, (map_pair[]){{.key = key}}, map_pair_hash, map_pair_equal);
  return p ? p->value : NULL;
}
