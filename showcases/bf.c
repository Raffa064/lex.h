/*
 * Small brain Fu** Interpreter.
 * 
 * Run with: `make run SHOWCASE=bf.c`
 */

#define LEX_IMPLEMENTATION
#include "../lex.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

enum {
  BF_WS,             // 
  BF_COMMENT,       // ;  
  BF_MOVE,         // <>
  BF_OPERATOR,    // +-
  BF_IO,         // .,
  BF_LOOP,      // []
  BF_COUNT,    //
};

size_t bf_rule_move(LexCursor cursor) { return lex_match_chars(cursor, "<>"); }
size_t bf_rule_operator(LexCursor cursor) { return lex_match_chars(cursor, "+-"); }
size_t bf_rule_io(LexCursor cursor) { return lex_match_chars(cursor, ".,"); }
size_t bf_rule_loop(LexCursor cursor) { return lex_match_chars(cursor, "[]"); }

LexType bf_tokens[BF_COUNT] = {
  LEX_TYPE(BF_WS,       lex_builtin_rule_ws,              .skip = true),
  LEX_TYPE(BF_COMMENT,  lex_builtin_rule_asmlike_comment, .skip = true),
  LEX_TYPE(BF_MOVE,     bf_rule_move),
  LEX_TYPE(BF_OPERATOR, bf_rule_operator),
  LEX_TYPE(BF_IO,       bf_rule_io),
  LEX_TYPE(BF_LOOP,     bf_rule_loop),
};

char* read_file(const char* path) {
  FILE* file = fopen(path, "r");
  char *content = NULL;

  if (file) {
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    content = malloc(size);

    fread(content, 1, size, file);
  } else {
    printf("Can't open %s\n", path);
  }

  fclose(file);

  return content;
}

int main() {
  char* input = read_file("./showcases/input.bf");

  Lex l = lex_init(LEX_TYPEARRAY(bf_tokens), input);
  lex_print_hl(l, true);

  char      mem[128]  = {0};
  LexCursor stack[64] = {0};
  unsigned  mp        = 0;
  unsigned  sp        = 0;

  printf("OUTPUT: ");
  while (lex_current(&l, NULL)) {
    char ch = lex_tkstr(l.tk)[0];

    bool jump = false;
    switch(l.tk.id) {
      case BF_MOVE: {
        if (ch == '<') mp -= 1;
        if (ch == '>') mp += 1;
      } break;
      case BF_OPERATOR: {
        if (ch == '-') mem[mp]--;
        if (ch == '+') mem[mp]++;
      } break;
      case BF_IO: {
        if (ch == '.') printf("\e[7;37m%c\e[0m", mem[mp]);
        if (ch == ',') assert(0 && "Input is not implemented");
      } break;
      case BF_LOOP: {
        if (ch == '[')                 stack[sp++] = l.cursor; 
        if (ch == ']' && mem[mp] != 0) jump = true;
      } break;
    }
    
    if (jump)
      lex_move_to(&l, stack[--sp]);
    else
      lex_move(&l);
  }

  printf("\n");

  free(input);

  return 0;
}
