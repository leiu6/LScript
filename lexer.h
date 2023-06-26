#ifndef MATHSTR_LEXER_H_INCLUDED
#define MATHSTR_LEXER_H_INCLUDED

typedef enum {
  // Single character
  OPEN_PAREN,
  CLOSE_PAREN,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  OPEN_BRACE,
  CLOSE_BRACE,
  COMMA,

  MULTIPLY,
  DIVIDE,
  ADD,
  SUBTRACT,
  EXPONENT,
  MODULO,
  FACTORIAL,
  EQUAL,

  // Literals
  NUMBER,
  IDENTIFIER,

  // MISC
  END,
  ERROR
} LexerTokenType;

typedef struct {
  // The type of the token
  LexerTokenType type;

  // Literal value
  union {
    double number;
    char identifier[1024];
  } literal;
} LexerToken;

// Initialize the lexer with a new source string
void lexer_init(const char *str);

// Free the allocated memory for the lexer
void lexer_quit();

// Get the next token from the loaded source string
LexerToken lexer_next();

#endif // MATHSTR_LEXER_H_INCLUDED
