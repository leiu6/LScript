#ifndef MATHSTR_LIBRARY_H_INCLUDED
#define MATHSTR_LIBRARY_H_INCLUDED

#include "runtime.h"

#define MAX_LIBS 1024

extern struct runtime_library *libs[MAX_LIBS];
extern int num_libs;
extern bool stdlib_defined;
#define STDLIB_PATH "./modules/stdlib.so"

#define LIBRARY_NORMAL       0x0
#define LIBRARY_MAX_REACHED -0x1

// Loads all libraries
// Returns LIBRARY_NORMAL if all goes well
// Returns LIBRARY_MAX_REACHED if there are too many libraries to completely laod
// Will still load all that it can
int library_init();

// Unloads all libraries
void library_quit();

// Finds a function in all loaded libraries and returns it
// Returns NULL if the function was not found
struct runtime_function *library_find(const char *function_name);

#endif // MATHSTR_LIBRARY_H_INCLUDED
