#include "error.h"

#include <stdio.h>
#include <stdlib.h>

#define E(e) #e
static const char *error_types[] = {ERROR_TYPES};
#undef E

#define eprint(str, ...) fprintf(stderr, str, __VA_ARGS__)

void error(ErrorType type, const char *msg, const char *code_snippet, int line,
           int col) {
  eprint("%s: %s\n", error_types[type], msg);

  if (code_snippet != NULL) {
    eprint("%i |%s\n", line, code_snippet);
  }
}

void error_s(ErrorType type, const char *msg) {
  error(type, msg, NULL, 0, 0);
}

void error_fatal(ErrorType type, const char *msg, const char *code_snippet,
                 int line, int col, int errno) {
  error(type, msg, code_snippet, line, col);
  exit(errno);
}
