#ifndef PAIR_H_INCLUDED
#define PAIR_H_INCLUDED

typedef struct pair pair;
struct pair
{
  char *key;
  void *value;
};

#endif /* PAIR_H_INCLUDED */
