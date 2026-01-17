#define LEX_IMPLEMENTATION
#include "../lex.h"

/*
 *  The lexer initialization actually checks for undefines types.
 */


enum {
  TYPE_A,     // = 0
  TYPE_B,     // = 1
  TYPE_C,     // = 2
  TYPE_COUNT  // = 3
};

// As the array length is 3, lex_init will look index by index, 
// cheking for unset tokens, and crash with a error message when
// it founds it.
LexType types[TYPE_COUNT] = {
  LEX_TYPE(TYPE_A, NULL), 
  LEX_TYPE(TYPE_B, NULL),    
  /* TYPE_C is unset */    
}; 

int main() {
  Lex l = lex_init(LEX_TYPEARRAY(types), NULL); // It will crash
  
  // Expected error message:
  // "Lex initialization failed due to an missing type definition. TypeIndex '2' is unset: Invalid argument"
}
