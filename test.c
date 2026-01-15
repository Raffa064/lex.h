#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define LEX_TOKEN_ID_OFFSET 2
#define LEX_STRIP_PREFIX
#define LEX_IMPLEMENTATION
#include "lex.h"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

typedef enum {
  T_WS,
  T_KWORD,
  T_ID,
  T_TERM,
  T_NUMBER,
  T_COUNT,
} Tokens;

size_t rule_kword(Cursor cursor);
size_t rule_id(Cursor cursor);
size_t rule_term(Cursor cursor);
size_t rule_number(Cursor cursor);

int main() {
  LexTokenDef tkdefs[T_COUNT] = {
    TOKENDEF(T_WS, builtin_rule_ws, .skip = true),
    TOKENDEF(T_KWORD, rule_kword),
    TOKENDEF(T_ID, rule_id),
    TOKENDEF(T_TERM, rule_term),
    TOKENDEF(T_NUMBER, rule_number),
  };

  TokenMap tkmap = TOKENMAP(tkdefs);
  Lex l = init(tkmap, "int main() {\n\treturn 0;\n}");

  LexResult result;
  while (current(&l, &result)) {
    printf("(%2zu:%2zu) %-10s '%s'\n", curline(l.cursor), curcol(l.cursor), tkname(l, l.tk), tkstr_tmp(l.tk));
    move(&l);
  }

  if (result == LEX_INVALID_TOKEN) {
    fprintf(stderr, "\e[31mErro\e[0m\n");
    return 1;
  }
}

// int | return
size_t rule_kword(LexCursor cursor) {
  const char *kwords[] = {"int", "return"};

  for (int i = 0; i < ARRAY_LEN(kwords); i++) {
    size_t len = strlen(kwords[i]);
    if (strncmp(kwords[i], curstr(cursor), len) == 0)
      return len;
  }

  return NO_MATCH;
}

// [a-zA-Z$_][a-zA-Z$_0-9]*
size_t rule_id(LexCursor cursor) {
  const char *start = curstr(cursor);
  if (isalpha(*start) || *start == '$' || *start == '_') {
    for (int len = 1; start[len] != '\0'; len++) {
      char ch = start[len];
      if (isalnum(ch) || *start == '$' || *start == '_')
        continue;

      return len;
    }
  }

  return NO_MATCH;
}

// '(' | ')' | '{' | '}'
size_t rule_term(LexCursor cursor) {
  return match_chars(cursor, "(){};") ? 1 : NO_MATCH;
}

// [0-9]+
size_t rule_number(LexCursor cursor) {
  const char *start = curstr(cursor);

  size_t len = NO_MATCH;
  for (; start[len] != '\0'; ++len) {
    if (!isdigit(start[len]))
      break;
  }

  return len;
}

