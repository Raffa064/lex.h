#include <stdio.h>

/*
 * This showcase is an example of how to use lex.h features.
 *
 * Note that since STRIP_PREFIX is defined the 'lex_' prefix is NOT needed for use all lex.h functions, structures, or macros.
 * You can see this same example with prefixes at ./c-main-prefix.c.
 *
 */

#define LEX_STRIP_PREFIX
#define LEX_TOKEN_ID_OFFSET 2
#define LEX_IMPLEMENTATION
#include "../lex.h"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

typedef enum {
  T_WS,
  T_COMMENT,
  T_ML_COMMENT,
  T_KWORD,
  T_TERM,
  T_STRING,
  T_NUMBER,
  T_ID,
  T_COUNT,
} Tokens;

size_t rule_comment(Cursor cursor);
size_t rule_ml_comment(Cursor cursor);
size_t rule_kword(Cursor cursor);
size_t rule_term(Cursor cursor);
size_t rule_number(Cursor cursor);

int main() {
  TokenType tkdefs[T_COUNT] = {
    TOKENTYPE(T_WS, builtin_rule_ws, .skip = true),
    TOKENTYPE(T_COMMENT, rule_comment, .skip = true),
    TOKENTYPE(T_ML_COMMENT, rule_ml_comment, .skip = true),
    TOKENTYPE(T_KWORD, rule_kword),
    TOKENTYPE(T_TERM, rule_term),
    TOKENTYPE(T_STRING, builtin_rule_dqstring),
    TOKENTYPE(T_NUMBER, rule_number),
    TOKENTYPE(T_ID, builtin_rule_id),
  };

  TokenMap tkmap = TOKENMAP(tkdefs);
  Lex l = init(tkmap, "// This is single line a comment\nint main() {\n\tprint(\"Hello world\");\n\treturn 0;\n} /* This comment \n can be multiline! */");

  print_hl(l, true);

  LexResult result;
  while (current(&l, &result)) {
    printf("(%2zu:%2zu) %-10s '%s'\n", curline(l.cursor), curcol(l.cursor), tkname(l, l.tk), tkstr_tmp(l.tk));
    move(&l);
  }

  if (result == LEX_INVALID_TOKEN) {
    fprintf(stderr, 
      "\e[31mErro: (%2zu:%2zu) '%s'\e[0m\n", 
      curline(l.cursor), 
      curcol(l.cursor), 
      curstr(l.cursor)
    );

    return 1;
  }

  return 0;
}


// '//' .* '\n'?
size_t rule_comment(Cursor cursor) {
  return match_region(cursor, "//", "\n", true);
}

// '/*' .* '*/'
size_t rule_ml_comment(Cursor cursor) {
  return match_region(cursor, "/*", "*/", false);
}

// int | return
size_t rule_kword(Cursor cursor) {
  const char *kwords[] = {"int", "return"};

  for (int i = 0; i < ARRAY_LEN(kwords); i++) {
    size_t len = match_keyword(cursor, kwords[i]);

    if (len != NO_MATCH)
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
