#include "map.c"

typedef struct map_pair map_pair;
struct map_pair
{
  char  *key;
  value *value;
};

static size_t map_pair_hash(map *m, void *obj)
{
  map_pair *p = obj;

  return python_hash(p->key);
}

static int map_pair_equal(map *m, void *obj1, void *obj2)
{
  map_pair *p1 = obj1, *p2 = obj2;

  return p1 -> key == p2 -> key || (p1 -> key && p2 -> key && ! strcmp (p1 -> key, p2 -> key));
}

static void map_pair_set(map * m, void *dst, void *src)
{
  map_pair *d = dst, *s = src;

  *d = *s;
}

uint64_t map_pair_grow(pair *pairs, size_t n)
{
  uint64_t t1, t2;
  map_pair empty = {NULL, NULL};
  map m;
  size_t i;

  map_construct(&m, sizeof(map_pair), &empty, map_pair_set);
  t1 = nano_time();
  for (i = 0; i < n; i ++)
    map_insert (&m, (map_pair[]){{.key = pairs[i].key, .value = pairs[i].value}}, map_pair_hash, map_pair_equal, map_pair_set, NULL);
  t2 = nano_time();
  map_clear(&m, map_pair_equal, map_pair_set, NULL);

  return t2 - t1;
}

uint64_t map_pair_hit(pair *pairs, size_t n)
{
  uint64_t t1, t2;
  map_pair empty = {NULL, NULL}, *p;
  map m;
  size_t i;

  map_construct(&m, sizeof(pair), &empty, map_pair_set);
  for (i = 0; i < n; i ++)
    map_insert (&m, (map_pair[]){{.key = pairs[i].key, .value = pairs[i].value}}, map_pair_hash, map_pair_equal, map_pair_set, NULL);

  t1 = nano_time();
  for (i = 0; i < n; i ++)
    {
      p = map_at(&m, (map_pair[]){{.key = pairs[i].key}}, map_pair_hash, map_pair_equal);
      assert(p);
    }
  t2 = nano_time();
  map_clear(&m, map_pair_equal, map_pair_set, NULL);

  return t2 - t1;
}

uint64_t map_pair_kbench(pair *pairs, size_t n)
{
  uint64_t t1, t2;
  map_pair empty = {NULL, NULL}, *p;
  map m;
  size_t i;

  map_construct(&m, sizeof(pair), &empty, map_pair_set);
  for (i = 0; i < n; i ++)
    map_insert (&m, (map_pair[]){{.key = pairs[i].key, .value = pairs[i].value}}, map_pair_hash, map_pair_equal, map_pair_set, NULL);

  t1 = nano_time();
  for (i = 0; i < n; i ++)
    {
      p = map_at(&m, (map_pair[]){{.key = pairs[i].key}}, map_pair_hash, map_pair_equal);
      assert(p);
      map_erase (&m, p, map_pair_hash, map_pair_equal, map_pair_set, NULL);
    }
  t2 = nano_time();
  assert(map_size(&m) == 0);
  map_clear(&m, map_pair_equal, map_pair_set, NULL);

  return t2 - t1;
}
