#include <stdio.h>

#define LEX_STRIP_PREFIX
#define LEX_TYPE_NAME_OFFSET 2 // Remove "T_" from token names
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

 
  lex_repl(LEX_TYPEARRAY(types));

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
