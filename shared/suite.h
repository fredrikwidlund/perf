#ifndef SUITE_H_INCLUDED
#define SUITE_H_INCLUDED

typedef struct suite suite;
struct suite
{
  glue *glue;
};

suite    *suite_alloc(void);
void      suite_free(suite *);
uint64_t  suite_set(suite *, pair *, size_t);
uint64_t  suite_get(suite *, pair *, size_t);

#endif /* SUITE_H_INCLUDED */
