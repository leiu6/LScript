#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include "runtime.h"
#include "library.h"

#include <stdio.h>

double interpret(const char *code);
void repl(void);
void read_file(const char *file_path);

int main(int argc, char **argv) {
  switch (argc) {
  case 1:
    repl();
    break;
  case 2:
    read_file(argv[1]);
    break;
  default:
    fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
  }

  return 0;
}

void repl(void) {
  library_init();

  char line[1024];

  while (true) {
    printf(">>> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    printf("%f\n", interpret(line));
  }

  library_quit();
}

double interpret(const char *code) {
  ParserAST *ast = parser_generate_ast(code);
  double result = evaluate_ast(ast);
  parser_free_ast(ast);
  return result;
}

void read_file(const char *path) {
  printf("Reading %s\n", path);
}
