#ifndef MATHSTR_PARSER_H_INCLUDED
#define MATHSTR_PARSER_H_INCLUDED

#include "defs.h"
#include "lexer.h"
#include "ls-obj.h"

typedef enum {
  LITERAL_EXPRESSION,
  PREFIX_EXPRESSION,
  INFIX_EXPRESSION,
  FUNCTION_CALL
} ParserNodeType;

typedef struct ParserNode {
  // The type of node
  ParserNodeType type;

  union {
    LSObj *literal;

    struct {
      LexerTokenType operator;
      struct ParserNode *next;
    } prefix;

    struct {
      LexerTokenType operator;
      struct ParserNode *rhs;
      struct ParserNode *lhs;
    } infix;

    struct {
      LexerToken identifier;
      int num_args;
      struct ParserNode *args[FUNCTION_MAX_NUM_ARGS];
    } function_call;
  } value;

} ParserNode;

typedef struct {
  ParserNode *head;  // The first node in the abstract syntax tree
  ParserNode *start; // The deepest node in the abstract syntax tree
} ParserAST;

// Parses the given mathstring and converts it to an abstract syntax tree
ParserAST *parser_generate_ast(const char *mathstr);

// Free the memory allocated for the abstract syntax tree
void parser_free_ast(ParserAST *ast);

void parser_print_ast(ParserAST *ast);

#endif // MATHSTR_PARSER_H_INCLUDED
