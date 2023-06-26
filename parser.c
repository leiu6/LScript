#include "parser.h"

#include "error.h"
#include "mat.h"
#include "dqueue.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBG_PRINT(str) printf("DBG: %s %i: %s\n", __FILE__, __LINE__, str)

/* Global Vars */
static LexerToken next;

/* Static Forward */
/* Allocate */
static ParserNode *alloc_literal(double literal_val);
static ParserNode *alloc_matrix(LSMat *m);
static ParserNode *alloc_prefix(LexerTokenType operator, ParserNode * next);
static ParserNode *alloc_infix(LexerTokenType operator, ParserNode * rhs,
                               ParserNode *lhs);
static ParserNode *
alloc_function_call(LexerToken identifier, int num_args,
                    ParserNode *(*args)[FUNCTION_MAX_NUM_ARGS]);
// Starts at a node and frees the memory recursively
static void free_from_node(ParserNode *node);

/* Parsing Levels */
static ParserNode *parse_expression(void); // Parse any expression
static ParserNode *parse_term(void);       // Parse a term (+, -)
static ParserNode *parse_factor(void);     // Parse a factor (*, /)
static ParserNode *
parse_exponent_or_modulo(void);       // Parse exponent or modulo (^, %)
static ParserNode *parse_unary(void); // Parse unary expression (!, -, +)
static ParserNode *
parse_primary(void); // Parse a primary expression (literal, function_call)
static ParserNode *parse_function_call(void); // Parse a function_call

/* Printing */
static void rprint(int padding, int add, const ParserNode *n);

/* Function Implementation */
ParserNode *alloc_literal(double literal_val) {
  ParserNode *n = malloc(sizeof(ParserNode));
  n->type = LITERAL_EXPRESSION;
  n->value.literal = lsobj_alloc_number(literal_val);
  return n;
}

ParserNode *alloc_matrix(LSMat *m) {
  ParserNode *n = malloc(sizeof(ParserNode));
  n->type = LITERAL_EXPRESSION;
  n->value.literal = lsobj_alloc_matrix(m);
  return n;
}

ParserNode *alloc_prefix(LexerTokenType operator, ParserNode * next) {
  ParserNode *n = malloc(sizeof(ParserNode));
  n->type = PREFIX_EXPRESSION;
  n->value.prefix.operator= operator;
  n->value.prefix.next = next;
  return n;
}

ParserNode *alloc_infix(LexerTokenType operator, ParserNode * rhs,
                        ParserNode *lhs) {
  ParserNode *n = malloc(sizeof(ParserNode));
  n->type = INFIX_EXPRESSION;
  n->value.infix.operator= operator;
  n->value.infix.rhs = rhs;
  n->value.infix.lhs = lhs;
  return n;
}

ParserNode *
alloc_function_call(LexerToken identifier, int num_args,
                    struct ParserNode *(*args)[FUNCTION_MAX_NUM_ARGS]) {
  ParserNode *n = malloc(sizeof(ParserNode));
  n->type = FUNCTION_CALL;
  n->value.function_call.identifier = identifier;
  n->value.function_call.num_args = num_args;
  memcpy(n->value.function_call.args, args, sizeof(ParserNode *) * num_args);
  return n;
}

void free_from_node(ParserNode *start) {
  if (start == NULL)
    return;

  switch (start->type) {
  case LITERAL_EXPRESSION:
    lsobj_free(start->value.literal);
    free(start);
    break;
  case PREFIX_EXPRESSION:
    free_from_node(start->value.prefix.next);
    free(start);
    break;
  case INFIX_EXPRESSION:
    free_from_node(start->value.infix.lhs);
    free_from_node(start->value.infix.rhs);
    free(start);
    break;
  case FUNCTION_CALL:
    for (int i = 0; i < start->value.function_call.num_args; i++)
      free_from_node(start->value.function_call.args[i]);
    free(start);
    break;
  }
}

ParserNode *parse_function_call(void) {
  LexerToken id;
  ParserNode *args[FUNCTION_MAX_NUM_ARGS];
  int num_args = 0;

  if (next.type == IDENTIFIER) {
    id = next;
    next = lexer_next();

    if (next.type != OPEN_PAREN) {
      error(SYNTAX_ERROR, "Expected '(' after function identifier", NULL, 0, 0);
      return NULL;
    }

    next = lexer_next();

    while (true) {
      if (next.type == CLOSE_PAREN) {
        next = lexer_next();
        return alloc_function_call(id, num_args, &args);
      } else if (next.type == COMMA) {
        next = lexer_next();
      }

      if (num_args >= FUNCTION_MAX_NUM_ARGS) {
        error(SYNTAX_ERROR, "Function can not accept this many arguments", NULL,
              0, 0);

        for (int i = 0; i < num_args; i++)
          free_from_node(args[i]);

        return NULL;
      }

      args[num_args] = parse_expression();
      num_args++;
    }
  }
}

ParserNode *parse_matrix_literal(void) {
  size_t rows = 0;
  size_t cols = 0;
  LSDQueue *queue = lsdq_alloc();
  bool first_row = true;

  if (next.type != OPEN_BRACKET) {
    error_s(SYNTAX_ERROR, "expected [ at beginning of matrix literal");
    return NULL;
  }

  for (;;) {
    next = lexer_next();
    if (next.type != OPEN_BRACKET) {
      error_s(SYNTAX_ERROR, "expected [ at beginning of matrix row");
      return NULL;
    }

    size_t check_cols = 0;
    for (;;) {
      next = lexer_next();
      ParserNode *entry = parse_primary();
      if (entry->type != LITERAL_EXPRESSION) {
        error_s(SYNTAX_ERROR, "matrix must be constructed with constants");
        return NULL;
      }

      lsdq_enqueue(queue, entry->value.literal->value.number);

      if (first_row)
        cols++;
      else {
        check_cols++;

        if (check_cols > cols) {
          error_s(SYNTAX_ERROR, "matrix rows must have the same number of columns");
          return NULL;
        }
      }

      if (next.type == COMMA) {
        continue;
      } else if (next.type == CLOSE_BRACKET) {
        break;
      } else {
        error_s(SYNTAX_ERROR, "expected , or ] after matrix entry");
        return NULL;
      }
    }
    rows++;
    if (first_row)
      first_row = false;

    next = lexer_next();
    if (next.type == COMMA) {
      continue;
    } else if (next.type == CLOSE_BRACKET) {
      break;
    } else {
      error_s(SYNTAX_ERROR, "expected , or ] after matrix row");
    }
  }

  LSMat *m = mat_alloc(rows, cols);

  for (size_t row = 0; row < rows; row++) {
    for (size_t col = 0; col < cols; col++) {
      double value;
      lsdq_dequeue(queue, &value);
      mat_set(m, row, col, value);
    }
  }
  lsdq_free(queue);

  return alloc_matrix(m);
}

ParserNode *parse_primary(void) {
  if (next.type == NUMBER) {
    double value = next.literal.number;
    next = lexer_next();
    return alloc_literal(value);
  } else if (next.type == OPEN_PAREN) {
    // Parse another expression
    next = lexer_next();
    ParserNode *expr = parse_expression();
    if (next.type != CLOSE_PAREN) {
      error(SYNTAX_ERROR, "Expected ')' after expression", NULL, 0, 0);
      return NULL;
    }
    next = lexer_next();
    return expr;
  } else if (next.type == IDENTIFIER) {
    // Parse a function call
    return parse_function_call();
  } else if (next.type == OPEN_BRACKET) {
    // Parse a matrix
    return parse_matrix_literal();
  } else {
    error(SYNTAX_ERROR, "Unable to parse primary statement", NULL, 0, 0);
  }

  return NULL;
}

ParserNode *parse_unary(void) {
  LexerTokenType op;

  if (next.type == FACTORIAL || next.type == SUBTRACT || next.type == ADD) {
    op = next.type;
    next = lexer_next();

    ParserNode *rhs = parse_unary();

    return alloc_prefix(op, rhs);
  }

  return parse_primary();
}

ParserNode *parse_exponent_or_modulo(void) {
  LexerTokenType op;
  ParserNode *rhs = NULL;
  ParserNode *lhs = parse_unary();

  while (true) {
    if (next.type == EXPONENT || next.type == MODULO) {
      op = next.type;
      next = lexer_next();
    } else {
      break;
    }

    ParserNode *rhs = parse_unary();
    lhs = alloc_infix(op, rhs, lhs);
  }

  return lhs;
}

ParserNode *parse_factor(void) {
  LexerTokenType op;
  ParserNode *rhs = NULL;
  ParserNode *lhs = parse_exponent_or_modulo();

  while (true) {
    if (next.type == MULTIPLY || next.type == DIVIDE) {
      op = next.type;
      next = lexer_next();
    } else {
      break;
    }

    ParserNode *rhs = parse_exponent_or_modulo();
    lhs = alloc_infix(op, rhs, lhs);
  }

  return lhs;
}

ParserNode *parse_term(void) {
  LexerTokenType op;
  ParserNode *rhs = NULL;
  ParserNode *lhs = parse_factor();

  while (true) {
    if (next.type == ADD || next.type == SUBTRACT) {
      op = next.type;
      next = lexer_next();
    } else {
      break;
    }

    ParserNode *rhs = parse_factor();
    lhs = alloc_infix(op, rhs, lhs);
  }

  return lhs;
}

ParserNode *parse_expression(void) { return parse_term(); }

ParserAST *parser_generate_ast(const char *mathstr) {
  ParserAST *ast = malloc(sizeof(ParserAST));
  ast->head = ast->start = NULL;

  lexer_init(mathstr);
  next = lexer_next();
  ast->head = parse_expression();
  lexer_quit();

  return ast;
}

#define PAD printf("%s", pad)

void rprint(int padding, int add, const ParserNode *n) {
  if (n == NULL)
    return;

  char pad[1024] = {0};
  for (int i = 0; i < padding; i++) {
    pad[i] = ' ';
  }
  pad[padding] = '\0';

  if (n->type == LITERAL_EXPRESSION) {
    if (n->value.literal->type == LSOBJ_MATRIX) {
      PAD;
      printf("Literal Matrix:\n");

      PAD;
      lsobj_print(n->value.literal);
    }
    else {
      PAD;
      printf("Literal Expression:\n");

      PAD;
      printf("Value: %f\n", n->value.literal->value.number);
    }
  } else if (n->type == PREFIX_EXPRESSION) {
    PAD;
    printf("Prefix Expression:\n");

    PAD;
    printf("Operator: %i\n", n->value.prefix.operator);

    PAD;
    printf("Operand:\n");
    rprint(padding + add, add, n->value.prefix.next);
  } else if (n->type == INFIX_EXPRESSION) {
    PAD;
    printf("Infix Expression:\n");

    PAD;
    printf("Operator: %i\n", n->value.infix.operator);

    PAD;
    printf("LHS:\n");
    rprint(padding + add, add, n->value.infix.lhs);

    PAD;
    printf("RHS:\n");
    rprint(padding + add, add, n->value.infix.rhs);
  } else if (n->type == FUNCTION_CALL) {
    PAD;
    printf("Function Call:\n");

    PAD;
    printf("Name: %s\n", n->value.function_call.identifier.literal.identifier);

    PAD;
    printf("Num Args: %i\n", n->value.function_call.num_args);
    for (int i = 0; i < n->value.function_call.num_args; i++) {
      rprint(padding + add, add, n->value.function_call.args[i]);
    }
  }
}

void parser_free_ast(ParserAST *ast) { free_from_node(ast->head); }

void parser_print_ast(ParserAST *ast) {
  printf("Abstract Syntax Tree:\n");
  rprint(0, 5, ast->head);
}
