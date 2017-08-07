#include "htable.c"

static size_t htable_hash(const void *key, void *unused)
{
  return python_hash(key);
}

static bool htable_compare(const void *object, void *key)
{
  const value *v = object;

  return !strcmp(v->key, key);
}

static size_t htable_hash_rehash(const void *object, void *unused)
{
  const value *v = object;

  return python_hash(v->key);
}

uint64_t htable_grow(pair *pairs, size_t n)
{
  uint64_t t1, t2;
  struct htable ht;
  size_t i;

  htable_init(&ht, htable_hash_rehash, NULL);
  t1 = nano_time();
  for (i = 0; i < n; i ++)
    htable_add(&ht, htable_hash(pairs[i].key, NULL), pairs[i].value);
  t2 = nano_time();
  htable_clear(&ht);

  return t2 - t1;
}

uint64_t htable_hit(pair *pairs, size_t n)
{
  uint64_t t1, t2;
  struct htable ht;
  size_t i;
  value *v;

  htable_init(&ht, htable_hash_rehash, NULL);

  for (i = 0; i < n; i ++)
    htable_add(&ht, htable_hash(pairs[i].key, NULL), pairs[i].value);

  t1 = nano_time();
  for (i = 0; i < n; i ++)
    {
      v = htable_get(&ht, htable_hash(pairs[i].key, NULL), htable_compare, pairs[i].key);
      assert(v);
    }
  t2 = nano_time();

  return t2 - t1;
}

uint64_t htable_kbench(pair *pairs, size_t n)
{
  uint64_t t1, t2;
  struct htable ht;
  size_t i;
  value *v;

  htable_init(&ht, htable_hash_rehash, NULL);

  for (i = 0; i < n; i ++)
    htable_add(&ht, htable_hash(pairs[i].key, NULL), pairs[i].value);

  t1 = nano_time();
  for (i = 0; i < n; i ++)
    {
      v = htable_get(&ht, htable_hash(pairs[i].key, NULL), htable_compare, pairs[i].key);
      assert(v);
      htable_del(&ht, htable_hash(v->key, NULL), v);
    }
  t2 = nano_time();
  assert(ht.elems == 0);

  return t2 - t1;
}
