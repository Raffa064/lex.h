/*
 * Minimal JSON lexer.
 *
 * This showcases is an example of use for lex profiler and debug-highligher.
 * 
 * Run with: `make run SHOWCASE=json.c`
 */

#include <stdio.h>

#define LEX_PROFILER // Enabled profiler

#define LEX_TYPE_NAME_OFFSET 3 // Remove "JT_" prefix from token names
#define LEX_IMPLEMENTATION
#include "../lex.h"

typedef enum {
  JT_WS,
  JT_STRING,
  JT_TERM,
  JT_NUMBER,
  JT_BOOL,
  JT_COUNT,
} JsonTokens;

size_t json_lex_rule_bool(LexCursor cursor);
size_t json_lex_rule_number(LexCursor cursor);
size_t json_lex_rule_term(LexCursor cursor);

LexType json_types[JT_COUNT] = {
  LEX_TYPE(JT_WS,     lex_builtin_rule_ws,    .skip = true),
  LEX_TYPE(JT_STRING, lex_builtin_rule_string),  
  LEX_TYPE(JT_BOOL,   json_lex_rule_bool),  
  LEX_TYPE(JT_NUMBER, json_lex_rule_number),  
  LEX_TYPE(JT_TERM,   json_lex_rule_term),  
};

int main() {
  Lex lex = lex_init(LEX_TYPEARRAY(json_types), 
    "{\n"
    "  \"array\": [ \"str\", 64, .2, false, [] ],\n"
    "  \"some-number\": .1,\n"
    "  \"bool-option\": true,\n"
    "  \"text\": \"Hello world\"\n"
    "}\n"
  );

  lex_print_hl(lex, true);

  LexResult result;
  while (lex_current(&lex, &result)) {
    printf("Token: %s '%s'\n", lex_tkname(lex, lex.tk), lex_tkstr_tmp(lex.tk));
    lex_move(&lex);
  }

  if (result == LEX_INVALID_TOKEN) {
    size_t start = lex_curline_start(lex.cursor);
    size_t end   = lex_curline_end(lex.cursor);
   
    fprintf(stderr, 
      "\e[31mErro: at %zu:%zu near '%.*s'\e[0m\n", 
      lex_curline(lex.cursor), 
      lex_curcol(lex.cursor), 
      (int) (end - start), 
      lex_view(lex, start)
    );

    return 1;
  }

  
  lex_print_profiler(lex); 

  return 0;
}

size_t json_lex_rule_bool(LexCursor cursor) { 
  size_t len = lex_match_keyword(cursor, "true"); 
  if (len)
    return len;

  len = lex_match_keyword(cursor, "false"); 
  if (len) return len;

  return LEX_NO_MATCH;
}

// [+-]?[0-9]+(.[0-9]*)?
size_t json_lex_rule_number(LexCursor cursor) { 
  const char *start = lex_curstr(cursor);

  size_t len = LEX_NO_MATCH;

  if (lex_match_chars(cursor, "+-"))
    len++;

  bool dot = false;
  bool digit = false;
  while (isdigit(start[len]) || start[len] == '.') {
    if (start[len] == '.') {
      if (dot)
        return LEX_NO_MATCH; // doubled dotted number -> invalid

      dot = true;
    } else if (isdigit(start[len])) {
      digit = true;
    }

    len++;
  }

  if (dot && !digit) // '.' is not a valid number
    return LEX_NO_MATCH;

  return len; 
}

size_t json_lex_rule_term(LexCursor cursor) { 
  return lex_match_chars(cursor, "{}[]:,"); 
}
