#include <stdio.h>

/*
 * This showcase is an example of how to use lex.h features.
 *
 * Note that since STRIP_PREFIX is defined the 'lex_' prefix is NOT needed for use all lex.h functions, structures, or macros.
 * You can see this same example with prefixes at ./c-main-prefix.c.
 *
 * Run with: `make run SHOWCASE=c-main.c`
 */

#define LEX_STRIP_PREFIX
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
} TokenTypes;

size_t rule_kword(Cursor cursor);
size_t rule_term(Cursor cursor);
size_t rule_number(Cursor cursor);

int main() {
  Type types[T_COUNT] = {
    TYPE(T_KWORD,      rule_kword),
    TYPE(T_TERM,       rule_term),
    TYPE(T_NUMBER,     rule_number),
    TYPE(T_COMMENT,    builtin_rule_clike_comment,   .skip = true),
    TYPE(T_ML_COMMENT, builtin_rule_clike_mlcomment, .skip = true),
    TYPE(T_WS,         builtin_rule_ws,              .skip = true),
    TYPE(T_STRING,     builtin_rule_dqstring),
    TYPE(T_ID,         builtin_rule_id),
  };

  Lex l = init(TYPEARRAY(types), 
    "// This is single line a comment\n"
    "int main() {\n\t"
      "print(\"Hello world\");\n\t"
      "return 0;\n"
    "} /* This comment \n can be multiline! */"
  );

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
