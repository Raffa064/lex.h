#include <stddef.h>

#define LEX_IMPLEMENTATION
#define LEX_USE_XMACRO
#include "../lex.h"

#define EXAMPLE(X) \
  X(EX_WS,     lex_builtin_rule_ws, .skip = true) \
  X(EX_ID,     lex_builtin_rule_id) \
  X(EX_CUSTOM, custom_rule) \

// This single macro pushes single source of truth to it's limits!
// It will create a enum of types, a LexType array, and forward declarations for each rule!
LEX_XMACRO_FRAMEWORK(EXAMPLE, ExampleTypes, example_types);

int main() {
  Lex l = lex_init(LEX_TYPEARRAY(example_types), LEX_SRC(NULL, "this is an example"));
  lex_print_hl(l, true);
}

size_t custom_rule(LexCursor cursor) {
  // ... Implementation of custom rule
  return LEX_NO_MATCH;
}
