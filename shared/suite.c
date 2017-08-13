#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

#include "value.h"
#include "glue.h"
#include "pair.h"
#include "suite.h"

static uint64_t nano_time(void)
{
  struct timespec ts;

  (void) clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  return ((uint64_t) ts.tv_sec * 1000000000) + ((uint64_t) ts.tv_nsec);
}

uint64_t suite_test_lto(void)
{
  int i, x = 0;
  uint64_t t1, t2;

  t1 = nano_time();
  for (i = 0; i < 1000000; i ++)
    x = glue_test_lto(x);
  assert(x == i);
  t2 = nano_time();

  return t2 - t1;
}

suite *suite_alloc(void)
{
  suite *s;

  s = malloc(sizeof *s);
  s->glue = glue_alloc();
  return s;
}

void suite_free(suite *s)
{
  glue_free(s->glue);
  free(s);
}

uint64_t suite_set(suite *s, pair *data, size_t n)
{
  size_t i;
  uint64_t t1, t2;

  t1 = nano_time();
  for (i = 0; i < n; i ++)
    glue_set(s->glue, data[i].key, data[i].value);
  t2 = nano_time();

  return t2 - t1;
}

uint64_t suite_get(suite *s, pair *data, size_t n)
{
  size_t i, hit;
  value *value;
  uint64_t t1, t2;

  hit = 0;
  t1 = nano_time();
  for (i = 0; i < n; i ++)
    {
      value = glue_get(s->glue, data[i].key);
      if (value && value->self == data[i].value)
        hit ++;
    }
  t2 = nano_time();
  assert(hit == i);

  return t2 - t1;
}
