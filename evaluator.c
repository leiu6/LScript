#include "evaluator.h"

#include "error.h"
#include "runtime.h"
#include "library.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static double evaluate_recursive(ParserNode *n);

static double evaluate_prefix(ParserNode *n);
static double evaluate_infix(ParserNode *n);
static double evaluate_function_call(ParserNode *n);

static double calculate_factorial(double x);

double evaluate_ast(ParserAST *ast) { return evaluate_recursive(ast->head); }

double evaluate_recursive(ParserNode *n) {
  if (n == NULL) {
    error(RUNTIME_ERROR, "Missing operand", NULL, 0, 0);
  }

  switch (n->type) {
  case LITERAL_EXPRESSION:
    return n->value.literal;
  case PREFIX_EXPRESSION:
    return evaluate_prefix(n);
  case INFIX_EXPRESSION:
    return evaluate_infix(n);
  case FUNCTION_CALL:
    return evaluate_function_call(n);
  }
}

double evaluate_prefix(ParserNode *n) {
  double val = evaluate_recursive(n->value.prefix.next);

  switch (n->value.prefix.operator) {
  case FACTORIAL:
    return calculate_factorial(val);
  case SUBTRACT:
    return -val;
  case ADD:
    return val;
  default:
    error(SYNTAX_ERROR, "Invalid operator for prefix operation", NULL, 0, 0);
    return val;
  }
}

double evaluate_infix(ParserNode *n) {
  double lhs, rhs;

  lhs = evaluate_recursive(n->value.infix.lhs);
  rhs = evaluate_recursive(n->value.infix.rhs);

  switch (n->value.infix.operator) {
  case EXPONENT:
    return pow(lhs, rhs);
  case MODULO:
    return (int)lhs & (int)rhs;
  case MULTIPLY:
    return lhs * rhs;
  case DIVIDE:
    return lhs / rhs;
  case ADD:
    return lhs + rhs;
  case SUBTRACT:
    return lhs - rhs;
  default:
    error(SYNTAX_ERROR, "Invalid operator for infix operation", NULL, 0, 0);
    return 0;
  }
}

double evaluate_function_call(ParserNode *n) {
  struct runtime_function *fn = library_find(n->value.function_call.identifier.literal.identifier);

  double args[FUNCTION_MAX_NUM_ARGS] = {0};
  for (int i = 0; i < n->value.function_call.num_args; i++) {
    args[i] = n->value.function_call.args[i]->value.literal;
  }

  double result = runtime_run_function(fn, n->value.function_call.num_args, args);
  return result;
}

double calculate_factorial(double x) { return x; }
