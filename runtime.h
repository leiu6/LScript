#ifndef MATHSTR_RUNTIME_H_INCLUDED
#define MATHSTR_RUNTIME_H_INCLUDED

#include "defs.h"
#include "ht.h"
#include "lib/runtime-module.h"

#ifndef __linux__
#error "platform does not support dynamic linking of library"
#endif
#include <dlfcn.h>

#include <stdbool.h>

struct runtime_function {
  rt_fun function; // A pointer to the callable runtime function
  int num_args;    // The number of arguments that can be passed to the function
};

struct runtime_library {
  void *handle;      // Handle to the library
  int num_functions; // The number of functions in the library
  ht *fn_ptrs;       // A hash table of the functions with the relevant pointers
};

// Create a callable library object from a .so library
//
// Returns NULL if the library could not be loaded
struct runtime_library *runtime_make_callable(const char *path);

// Close the runtime library when it is no longer needed
void runtime_close(struct runtime_library *lib);

#define RUNTIME_FUNCTION_FOUND      0
#define RUNTIME_FUNCTION_NOT_FOUND  -1
struct runtime_function *runtime_get_function(struct runtime_library *lib, const char *name);

double runtime_run_function(struct runtime_function *fn, int argc, double *argv);

#endif // MATHSTR_RUNTIME_H_INCLUDED
