#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dlfcn.h>
#include <err.h>

#include "value.h"
#include "pair.h"
#include "glue.h"
#include "suite.h"

typedef struct plugin plugin;
struct plugin
{
  void       *handle;
  suite    *(*alloc)(void);
  void      (*free)(suite *);
  uint64_t  (*test_lto)(void);
  uint64_t  (*set)(suite *, pair *, size_t);
  uint64_t  (*get)(suite *, pair *, size_t);
};

int plugin_load(plugin *plugin, char *path)
{
  char *error;

  plugin->handle = dlmopen(LM_ID_BASE, path, RTLD_LAZY);
  if (!plugin->handle)
    return -1;

  dlerror();
  plugin->test_lto = dlsym(plugin->handle, "suite_test_lto");
  plugin->alloc = dlsym(plugin->handle, "suite_alloc");
  plugin->free = dlsym(plugin->handle, "suite_free");
  plugin->set = dlsym(plugin->handle, "suite_set");
  plugin->get = dlsym(plugin->handle, "suite_get");
  error = dlerror();
  if (error)
    {
      dlclose(plugin->handle);
      return -1;
    }

  return 0;
}

static pair *create_data(size_t n)
{
  pair *data;
  value *value;
  char key[64];
  size_t i;

  data = calloc(n, sizeof data[0]);
  for (i = 0; i < n; i ++)
    {
      snprintf(key, sizeof key, "%lu", i);
      value = malloc(sizeof *value);
      value->self = value;
      data[i].key = strdup(key);
      data[i].value = value;
    }
  return data;
}

void plugin_unload(plugin *plugin)
{
  dlclose(plugin->handle);
}

int main()
{
  plugin plugin;
  size_t n;
  pair *data;
  suite *s;
  uint64_t t;
  int e;

  n = 100000;
  data = create_data(n);

  e = plugin_load(&plugin, "plugin/glue_libdynamic.so");
  if (e == -1)
    err(1, "plugin_load");

  t = plugin.test_lto();
  (void) fprintf(stdout, "test lto: %lu\n", t);

  s = plugin.alloc();
  t = plugin.set(s, data, n);
  (void) fprintf(stdout, "set: %lu\n", t);
  t = plugin.get(s, data, n);
  (void) fprintf(stdout, "get: %lu\n", t);

  plugin.free(s);


  plugin_unload(&plugin);
}

