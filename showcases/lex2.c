#include <stdio.h>

#define LEX_IMPLEMENTATION
#include "../lex.h"

int main() {
  const char* str = "I'm testing something here";

  LexCursor cursor = {
    .source = str,
    .index = 4,
    .length = 7
  };

  LexStringView sv = lex_view_at_cursor(cursor);
  printf("My sv: '" LEX_SVFMT "'\n", lex_svarg(sv));

  LexStringView hw = lex_view("Hello world");
  bool eq = lex_view_eq_cstr(hw, "Hello world");
  printf("eq cstr test: %d\n", eq);

  printf("Is hw empty: %d\n", lex_view_empty(hw));
  printf("Is empty: %d\n", lex_view_empty(lex_view("")));

  LexStringView hw_chopl = lex_view_chop(hw, 2, 0);
  printf("Chop left: " LEX_SVFMT "\n", lex_svarg(hw_chopl));

  LexStringView hw_chopr = lex_view_chop(hw, 0, 2);
  printf("Chop right: " LEX_SVFMT "\n", lex_svarg(hw_chopr));

  LexStringView s = lex_view("\n\t this library is insane   ");
  LexStringView tl = lex_view_trim_left(s);
  LexStringView tr = lex_view_trim_right(s);
  LexStringView ta = lex_view_trim(tl);

  printf("TL: '" LEX_SVFMT "'\n", lex_svarg(tl));
  printf("TR: '" LEX_SVFMT "'\n", lex_svarg(tr));
  printf("TA: '" LEX_SVFMT "'\n", lex_svarg(ta));

  bool lex_view_empty(LexStringView sv);
}
