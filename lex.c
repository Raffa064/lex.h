#include <stdio.h>
#define LEX_IMPLEMENTATION
#include "./lex.h"

int main() {
  LexCursor c = {};

  for (int  i = 0; i < 10; i++)
    lex_curmove(&c, 1);

  printf("%zu\n", lex_curstart(c));
}
