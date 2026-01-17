#define LEX_IMPLEMENTATION
#include "../lex.h"

// Using LEX_TYPE the order of the types are set by the value of its "id"s: 
enum {
  TYPE_C,     // = 0
  TYPE_B,     // = 1
  TYPE_A,     // = 2
  TYPE_COUNT  // = 3
};

LexType types[TYPE_COUNT] = {
  LEX_TYPE(TYPE_A, NULL),    // Same as [TYPE_A] = (LexType) { .name = "TYPE_A", .... }
  LEX_TYPE(TYPE_B, NULL),    //         [TYPE_B] = ...
  LEX_TYPE(TYPE_C, NULL),    //         [TYPE_C] = ...


  // As the type indexes are set by the corresponding `type id`,
  // This approach gives automatic duplication warns from the compiler.
  // Uncomment this line to test it out:
  // LEX_TYPE(TYPE_C, NULL),  
}; 

// You can't use it outside static arrays:
// LexType A = LEX_TYPE(TYPE_A, NULL);

int main() {
  Lex l = lex_init(LEX_TYPEARRAY(types), NULL);

  lex_print_types(l);  // TYPE_C TYPE_B TYPE_A 
}
