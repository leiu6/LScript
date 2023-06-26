#include "library.h"

#include <stdio.h>

struct runtime_library *libs[MAX_LIBS] = {0};
int num_libs = 0;
bool stdlib_defined = false;

static struct runtime_library *
library_load(const char *path) {
    return runtime_make_callable(path);
}

int
library_init(void) {
    struct runtime_library * stdlib = library_load(STDLIB_PATH);

    if (stdlib != NULL) {
        num_libs++;
        libs[0] = stdlib;
        stdlib_defined = true;
        return LIBRARY_NORMAL;
    }

    return LIBRARY_MAX_REACHED;
}

void
library_quit(void) {
    runtime_close(libs[0]);
}

struct runtime_function *library_find(const char *function_name) {
    struct runtime_function *fun = NULL;

    for (int i = 0; i < num_libs; i++) {
        struct runtime_library *lib = libs[i];

        fun = runtime_get_function(lib, function_name);
        if (fun != NULL) {
            return fun;
        }
    }

    return fun;
}
