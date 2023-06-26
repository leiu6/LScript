#ifndef MATHSTR_RUNTIME_MODULE_H_INCLUDED
#define MATHSTR_RUNTIME_MODULE_H_INCLUDED

#include "../defs.h"

#include <stdbool.h>

typedef double (*rt_fun)(int argc, double *argv);

struct runtime_method_def {
  char method_name[FUNCTION_NAME_MAX_CHARS];
  rt_fun ptr;
  int num_args;
};

struct runtime_module_def {
  char module_name[128];
  char module_desc[528];
  int num_methods;
  struct runtime_method_def *methods;
};

#define runtime_module_autocount(modules) (sizeof(modules) / sizeof(modules[0]))

#endif // MATHSTR_RUNTIME_MODULE_H_INCLUDED