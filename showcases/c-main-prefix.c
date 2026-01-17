#include <stdio.h>

/*
 * This showcase is an example of how to use lex.h features.
 *
 * Note that since LEX_STRIP_PREFIX is NOT defined, the 'lex_' prefix is needed for use all lex.h functions, structures, or macros.
 * You can see this same example without prefixes at ./c-main-prefix.c
 *
 * Run with: `make run SHOWCASE=c-main-prefix.c`
 */

#define LEX_TOKEN_NAME_OFFSET 2 // Remove "T_" from token names
#define LEX_IMPLEMENTATION
#include "../lex.h"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

typedef enum {
  T_KWORD,
  T_TERM,
  T_NUMBER,
  T_WS,
  T_COMMENT,
  T_ML_COMMENT,
  T_STRING,
  T_ID,
  T_COUNT,
} Tokens;

size_t rule_kword(LexCursor cursor);
size_t rule_term(LexCursor cursor);
size_t rule_number(LexCursor cursor);

int main() {
  LexType tkdefs[T_COUNT] = {
    LEX_TYPE(T_KWORD,      rule_kword),
    LEX_TYPE(T_TERM,       rule_term),
    LEX_TYPE(T_NUMBER,     rule_number),
    LEX_TYPE(T_WS,         lex_builtin_rule_ws,              .skip = true),
    LEX_TYPE(T_COMMENT,    lex_builtin_rule_clike_comment,   .skip = true),
    LEX_TYPE(T_ML_COMMENT, lex_builtin_rule_clike_mlcomment, .skip = true),
    LEX_TYPE(T_STRING,     lex_builtin_rule_dqstring),
    LEX_TYPE(T_ID,         lex_builtin_rule_id),
  };

  LexTypeArray tkmap = LEX_TYPEARRAY(tkdefs);
  Lex l = lex_init(tkmap, 
    "// This is single line a comment\n"
    "int main() {\n\t"
      "print(\"Hello world\");\n\t"
      "return 0;\n"
    "} /* This comment \n can be multiline! */"
  );  

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
