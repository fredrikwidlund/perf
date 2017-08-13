#ifndef VALUE_H_INCLUDED
#define VALUE_H_INCLUDED

typedef struct value value;
struct value
{
  char   data[64];
  value *self;
};

#endif /* VALUE_H_INCLUDED */
