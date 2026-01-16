#include <stdio.h>

/*
 * This showcase is an example of how to use lex.h features.
 *
 * Note that since LEX_STRIP_PREFIX is NOT defined the 'lex_' prefix is needed for use all lex.h functions, structures, or macros.
 * You can see this same example without prefixes at ./c-main-prefix.c
 */

#define LEX_TOKEN_ID_OFFSET 2
#define LEX_IMPLEMENTATION
#include "../lex.h"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

typedef enum {
  T_WS,
  T_KWORD,
  T_TERM,
  T_STRING,
  T_NUMBER,
  T_ID,
  T_COUNT,
} Tokens;

size_t rule_kword(LexCursor cursor);
size_t rule_term(LexCursor cursor);
size_t rule_string(LexCursor cursor);
size_t rule_number(LexCursor cursor);

int main() {
  LexTokenType tkdefs[T_COUNT] = {
    LEX_TOKENTYPE(T_WS, lex_builtin_rule_ws, .skip = true),
    LEX_TOKENTYPE(T_KWORD, rule_kword),
    LEX_TOKENTYPE(T_TERM, rule_term),
    LEX_TOKENTYPE(T_STRING, rule_string),
    LEX_TOKENTYPE(T_NUMBER, rule_number),
    LEX_TOKENTYPE(T_ID, lex_builtin_rule_id),
  };

  LexTokenMap tkmap = LEX_TOKENMAP(tkdefs);
  Lex l = lex_init(tkmap, "int main() {\n\tprint(\"Hello world\");\n\treturn 0;\n}");

  lex_print_hl(l, true);

  LexResult result;
  while (lex_current(&l, &result)) {
    printf("(%2zu:%2zu) %-10s '%s'\n", lex_curline(l.cursor), lex_curcol(l.cursor), lex_tkname(l, l.tk), lex_tkstr_tmp(l.tk));
    lex_move(&l);
  }

  if (result == LEX_INVALID_TOKEN) {
    fprintf(stderr, 
      "\e[31mErro: (%2zu:%2zu) '%s'\e[0m\n", 
      lex_curline(l.cursor), 
      lex_curcol(l.cursor), 
      lex_curstr(l.cursor)
    );

    return 1;
  }

  return 0;
}

// int | return
size_t rule_kword(LexCursor cursor) {
  const char *kwords[] = {"int", "return"};

  for (int i = 0; i < ARRAY_LEN(kwords); i++) {
    size_t len = lex_match_keyword(cursor, kwords[i]);

    if (len != LEX_NO_MATCH)
      return len;
  }

  return LEX_NO_MATCH;
}


// '(' | ')' | '{' | '}'
size_t rule_term(LexCursor cursor) {
  return lex_match_chars(cursor, "(){};") ? 1 : LEX_NO_MATCH;
}


size_t rule_string(LexCursor cursor) {
  return lex_match_wrapped(cursor, '"', true);
}

// [0-9]+
size_t rule_number(LexCursor cursor) {
  const char *start = lex_curstr(cursor);

  size_t len = LEX_NO_MATCH;
  for (; start[len] != '\0'; ++len) {
    if (!isdigit(start[len]))
      break;
  }

  return len;
}
