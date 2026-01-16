#include <stdio.h>

/*
 * This showcase is an example of how to use lex.h features.
 *
 * Note that since LEX_STRIP_PREFIX is defined, no 'lex_' prefix is needed for use lex.h functions, structures, or macros.
 * You can see this same example with prefixes at ./c-main-prefix.c
 */

#define LEX_TOKEN_ID_OFFSET 2
#define LEX_STRIP_PREFIX

#define LEX_IMPLEMENTATION
#include "../lex.h"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

typedef enum {
  T_WS,
  T_KWORD,
  T_TERM,
  T_NUMBER,
  T_ID,
  T_COUNT,
} Tokens;

size_t rule_kword(Cursor cursor);
size_t rule_term(Cursor cursor);
size_t rule_number(Cursor cursor);

int main() {
  TokenType tkdefs[T_COUNT] = {
    TOKENTYPE(T_WS, builtin_rule_ws, .skip = true),
    TOKENTYPE(T_KWORD, rule_kword),
    TOKENTYPE(T_TERM, rule_term),
    TOKENTYPE(T_NUMBER, rule_number),
    TOKENTYPE(T_ID, builtin_rule_id),
  };

  TokenMap tkmap = TOKENMAP(tkdefs);
  Lex l = init(tkmap, "int main() {\n\treturn 0;\n}");

  LexResult result;
  while (current(&l, &result)) {
    printf("(%2zu:%2zu) %-10s '%s'\n", curline(l.cursor), curcol(l.cursor), tkname(l, l.tk), tkstr_tmp(l.tk));
    move(&l);
  }

  if (result == LEX_INVALID_TOKEN) {
    fprintf(stderr, "\e[31mErro: (%2zu:%2zu) %-10s '%s'\e[0m\n", curline(l.cursor), curcol(l.cursor), tkname(l, l.tk), tkstr_tmp(l.tk));
    return 1;
  }
}

// int | return
size_t rule_kword(Cursor cursor) {
  const char *kwords[] = {"int", "return"};

  for (int i = 0; i < ARRAY_LEN(kwords); i++) {
    size_t len = sizeof(kwords[i]);
    if (match_keywordn(cursor, kwords[i], len))
      return len;
  }

  return NO_MATCH;
}


// '(' | ')' | '{' | '}'
size_t rule_term(Cursor cursor) {
  return match_chars(cursor, "(){};") ? 1 : NO_MATCH;
}

// [0-9]+
size_t rule_number(Cursor cursor) {
  const char *start = curstr(cursor);

  size_t len = NO_MATCH;
  for (; start[len] != '\0'; ++len) {
    if (!isdigit(start[len]))
      break;
  }

  return len;
}
