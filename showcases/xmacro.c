#define LEX_IMPLEMENTATION
#define LEX_USE_XMACRO
#include "../lex.h"

// Using X-macros you can make really cool stuff in lex.h, pushing single source of truth to it's limits:

// Any changes to this "X-table" will affect both ExampleTokens enum and types array.
#define EXAMPLE(X) \
  X(EX_WS, lex_builtin_rule_ws, .skip = true) \
  X(EX_ID, lex_builtin_rule_id)

typedef LEX_ENUMX(EXAMPLE) ExampleTokens;

LexType types[EXAMPLE_COUNT] = LEX_TYPEX(EXAMPLE);

int main() {
  Lex l = lex_init(LEX_TYPEARRAY(types), "this is an example");
  lex_print_hl(l, true);
}
