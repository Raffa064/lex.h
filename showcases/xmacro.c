#include <stddef.h>
#define LEX_IMPLEMENTATION
#define LEX_USE_XMACRO
#include "../lex.h"

// Using X-macros you can make really cool stuff in lex.h, pushing single source of truth to it's limits:

// Any changes to this "X-table" will affect both ExampleTokens enum and types array.
#define EXAMPLE(X) \
  X(EX_WS,     lex_builtin_rule_ws, .skip = true) \
  X(EX_ID,     lex_builtin_rule_id) \
  X(EX_CUSTOM, custom_rule) \

typedef LEX_ENUMX(EXAMPLE) ExampleTokens;

// Automatic forward declarations for all rules
LEX_XFORWARD_DECL(EXAMPLE);

LexType types[EXAMPLE_COUNT] = LEX_TYPEX(EXAMPLE);

int main() {
  Lex l = lex_init(LEX_TYPEARRAY(types), "this is an example");
  lex_print_hl(l, true);
}

size_t custom_rule(LexCursor cursor) {
  // ... Implementation of custom rule
  return LEX_NO_MATCH;
}
