#define LEX_IMPLEMENTATION
#include "../lex.h"

int main() {
  const char* str = "I'm testing something here";

  LexCursor cursor = {
    .source = str,
    .index = 4,
    .length = 7
  };

  LexStringView sv = lex_view_after(cursor);
  printf("My sv: '" LEX_SVFMT "'", lex_svarg(sv));
}
