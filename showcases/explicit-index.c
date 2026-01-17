#define LEX_IMPLEMENTATION
#include "../lex.h"

enum {
  TYPE_C,
  TYPE_B,
  TYPE_A,
  TYPE_COUNT
};

// Using LEX_ETYPE, the order of the types are set by 'types' array initialization: 
LexType types[TYPE_COUNT] = {
  LEX_ETYPE(TYPE_A, NULL),    // Same as (LexType) { .name = "TYPE_A", .... }
  LEX_ETYPE(TYPE_B, NULL),
  LEX_ETYPE(TYPE_C, NULL),

  // It has a little drawback:
  // With explicit index option, the compiler will not warn about 
  // duplicates, unless you EXPLICITELY define your indexes with 
  // index designators:
  //
  // [TYPE_C] = LEX_TYPE(TYPE_C, NULL)
  //
  // *This is what the library actually does by default*
}; 

// It can be used as a normal statement:
LexType A = LEX_ETYPE(TYPE_A, NULL);

int main() {
  Lex l = lex_init(LEX_TYPEARRAY(types), NULL);
 
  lex_print_types(l); // TYPE_A TYPE_B TYPE_C 
}
