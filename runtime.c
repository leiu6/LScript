#include "runtime.h"

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

struct runtime_library *runtime_make_callable(const char *path) {
  // Try to load the library
  void *handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
  if (handle == NULL) {
    error(RUNTIME_ERROR, dlerror(), NULL, 0, 0);
    return NULL;
  }

  struct runtime_library *lib = malloc(sizeof(struct runtime_library));
  if (lib == NULL) {
    error(RUNTIME_ERROR, "could not allocate memory for runtime library", NULL,
          0, 0);
    return NULL;
  }
  lib->handle = handle;

  // Try to get the module definition
  struct runtime_module_def *def;
  def = dlsym(handle, "__module_def__");
  if (def == NULL) {
    error(RUNTIME_ERROR, "invalid runtime library", NULL, 0, 0);
    error(RUNTIME_ERROR, dlerror(), NULL, 0, 0);
  }

  lib->fn_ptrs = ht_create();
  lib->num_functions = def->num_methods;
  // Load the functions
  for (int i = 0; i < def->num_methods; i++) {
    const char *name = def->methods[i].method_name;
    struct runtime_function *fun = malloc(sizeof(struct runtime_function));
    fun->function = def->methods[i].ptr;
    fun->num_args = def->methods[i].num_args;
    ht_set(lib->fn_ptrs, name, fun);
    // printf("Imported function: %s\n", name);
  }

  return lib;
}

void runtime_close(struct runtime_library *lib) {
  hti it = ht_iterator(lib->fn_ptrs);
  while (ht_next(&it))
    free(it.value);
  ht_destroy(lib->fn_ptrs);
  int e = dlclose(lib->handle);
  free(lib);
  if (e != 0) {
    error_fatal(RUNTIME_ERROR, "could not properly close library", NULL, 0, 0, e);
  }
} 

struct runtime_function *
runtime_get_function(struct runtime_library *lib, const char *name) {
  struct runtime_function *function = ht_get(lib->fn_ptrs, name);
  if (function == NULL) {
    error(RUNTIME_ERROR, "function not found in library", NULL, 0, 0);
    return NULL;
  }

  return function;
}

double
runtime_run_function(struct runtime_function *fn, int argc, double *argv) {
  if (argc > fn->num_args) {
    error(RUNTIME_ERROR, "function does not accept this many arguments", NULL, 0, 0);
    return 0;
  }

  return fn->function(argc, argv);
}
