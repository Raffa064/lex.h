// Minimal JSON lexer 

#include <stdio.h>

#define LEX_TOKEN_NAME_OFFSET 3 // Remove "JT_" prefix from token names
#define LEX_IMPLEMENTATION
#include "../lex.h"

typedef enum {
  JT_WS,
  JT_STRING,
  JT_BOOL,
  JT_NUMBER,
  JT_TERM,
  JT_COUNT,
} JsonTokens;

size_t json_lex_rule_string(LexCursor cursor);
size_t json_lex_rule_bool(LexCursor cursor);
size_t json_lex_rule_number(LexCursor cursor);
size_t json_lex_rule_term(LexCursor cursor);

LexTokenType json_token_types[JT_COUNT] = {
  LEX_TOKENTYPE(JT_WS, lex_builtin_rule_ws, .skip = true),
  LEX_TOKENTYPE(JT_STRING, json_lex_rule_string),  
  LEX_TOKENTYPE(JT_BOOL, json_lex_rule_bool),  
  LEX_TOKENTYPE(JT_NUMBER, json_lex_rule_number),  
  LEX_TOKENTYPE(JT_TERM, json_lex_rule_term),  
};

LexTokenMap json_map = LEX_TOKENMAP(json_token_types);

int main() {
  Lex lex = lex_init(json_map, 
    "{\n"
    "  \"watch\": [\n"
    "    \".\"\n"
    "  ],\n"
    "  \"ignore\": [\n"
    "    \"**/build/**\"\n"
    "  ],\n"
    "  \"some-number\": .1"
    "  \"ext\": \"sh,txt,c,h,py,csv\",\n"
    "  \"exec\": \"./run.sh\"\n"
    "}\n"
  );

  LexResult result;
  while (lex_current(&lex, &result)) {
    printf("Token: %s '%s'\n", lex_tkname(lex, lex.tk), lex_tkstr_tmp(lex.tk));

    lex_move(&lex);
  }

  if (result == LEX_INVALID_TOKEN) {
    fprintf(stderr, 
      "\e[31mErro: (%2zu:%2zu) '%s'\e[0m\n", 
      lex_curline(lex.cursor), 
      lex_curcol(lex.cursor), 
      lex_curstr(lex.cursor)
    );

    return 1;
  }

  return 0;
}

// '"' .* '"'
size_t json_lex_rule_string(LexCursor cursor) {
  size_t len = LEX_NO_MATCH;

  len = lex_match_wrapped(cursor, '\"', true);
  
  if (len)
    return len;

 len = lex_match_wrapped(cursor, '\'', true);
 if (len)
   return len;

  return LEX_NO_MATCH;
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
