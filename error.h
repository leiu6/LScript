#ifndef MATHSTR_ERROR_H_INCLUDED
#define MATHSTR_ERROR_H_INCLUDED

#include <stdbool.h>

#define ERROR_TYPES E(SYNTAX_ERROR), E(RUNTIME_ERROR)

typedef enum {
#define E(e) e
  ERROR_TYPES
#undef E
} ErrorType;

void error(ErrorType type, const char *msg, const char *code_snippet, int line,
           int col);

void error_fatal(ErrorType type, const char *msg, const char *code_snippet,
                 int line, int col, int errno);

#endif // MATHSTR_ERROR_H_INCLUDED
