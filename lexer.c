#include "lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct {
  char *current, *start;
  int line;
  char *mathstr;
} state;

static inline LexerToken token_single(LexerTokenType type) {
  return (LexerToken){type, -1.0};
}

void lexer_init(const char *str) {
  state.mathstr = malloc(strlen(str) + 1);
  strcpy(state.mathstr, str);
  state.line = 1;
  state.current = state.start = state.mathstr;
}

void lexer_quit() { free(state.mathstr); }

static inline char advance() { return *state.current++; }

static inline char deadvance() { return *state.current--; }

static inline char peek() { return *state.current; }

static inline char peek_back() { return *(state.current - 1); };

static LexerToken token_number(char current) {
  char c = current;
  char numstr[1024] = {0};
  int i = 0;
  char *eptr;

  while ((isdigit(peek_back()) != 0) || (peek_back() == '.')) {
    numstr[i] = c;
    c = advance();
    i++;
  }

  c = deadvance();

  numstr[i] = '\0';

  return (LexerToken){NUMBER, strtod(numstr, &eptr)};
};

static LexerToken token_identifier(char current) {
  char c = current;
  LexerToken tk;
  char identifier[1024] = {0};
  int i = 0;

  while (isalnum(peek_back())) {
    identifier[i] = c;
    c = advance();
    i++;
  }

  c = deadvance();

  identifier[i + 1] = '\0';

  tk.type = IDENTIFIER;
  strcpy(tk.literal.identifier, identifier);
  return tk;
};

LexerToken lexer_next() {
  char c;
  LexerToken tk;

  state.start = state.current;

  // Skip whitespace
  while (1) {
    c = advance();

    if (c == '\n') {
      state.line++;
    } else if (c == '\t' || c == ' ') {
      continue;
    } else {
      break;
    }
  }

  switch (c) {
  case '\0':
    tk = token_single(END);
    break;

  case '(':
    tk = token_single(OPEN_PAREN);
    break;
  case ')':
    tk = token_single(CLOSE_PAREN);
    break;
  case '[':
    tk = token_single(OPEN_BRACKET);
    break;
  case ']':
    tk = token_single(CLOSE_BRACKET);
    break;
  case '{':
    tk = token_single(OPEN_BRACE);
    break;
  case '}':
    tk = token_single(CLOSE_BRACE);
    break;
  case ',':
    tk = token_single(COMMA);
    break;

  case '*':
    tk = token_single(MULTIPLY);
    break;
  case '/':
    tk = token_single(DIVIDE);
    break;
  case '+':
    tk = token_single(ADD);
    break;
  case '-':
    tk = token_single(SUBTRACT);
    break;
  case '^':
    tk = token_single(EXPONENT);
    break;
  case '%':
    tk = token_single(MODULO);
    break;
  case '!':
    tk = token_single(FACTORIAL);
    break;
  case '=':
    tk = token_single(EQUAL);
    break;

  default:
    if (isdigit(c) != 0) {
      // It is a number
      tk = token_number(c);
      break;
    } else if (isalpha(c) != 0) {
      // It is an identifier
      tk = token_identifier(c);
      break;
    }
    tk = token_single(ERROR);
  }

  return tk;
};
