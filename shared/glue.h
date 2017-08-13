#ifndef GLUE_H_INCLUDED
#define GLUE_H_INCLUDED

typedef struct glue glue;
struct glue;

int   glue_test_lto(int);
glue *glue_alloc(void);
void  glue_free(glue *);
void  glue_set(glue *, char *, void *);
void *glue_get(glue *, char *);

#endif /* GLUE_H_INCLUDED */
