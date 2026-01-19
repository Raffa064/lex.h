#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LEX_IMPLEMENTATION
#include "../lex.h"

#define TEST   32
#define TARGET 64

enum {
  TYPE_A,     // = 0
  TYPE_B,     // = 1
  TYPE_C,     // = 2
  TYPE_COUNT  // = 3
};

LexType types[TYPE_COUNT] = {
  LEX_TYPE(TYPE_A, NULL), 
  LEX_TYPE(TYPE_B, NULL),    
  LEX_TYPE(TYPE_C, NULL),    
}; 

void test(Lex *l) {
  Lex b = LEX_BRANCH(l);
  printf("\e[33mCreated a TEST branch!\e[0m\n");
  b.cursor.index = TEST;

  if (rand() < RAND_MAX/2) {
    printf("\e[32mMerged a TEST branch!\e[0m\n");
    LEX_MERGE_BRANCH(l, b);
    return;
  }

  printf("\e[37mDiscarted TEST branch!\e[0m\n");
}

int main() {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC,  &time);
  srand(time.tv_nsec);

  Lex l = lex_init(LEX_TYPEARRAY(types), NULL); // It will crash

  Lex target = LEX_BRANCH(&l);
  printf("\e[35mCreated the TARGET branch!\e[0m\n");
  target.cursor.index = TARGET;
  
  test(&l);
  test(&l);
  test(&l);

  LEX_MERGE_BRANCH(&l, target);
  printf("\e[36mMerged the TARGET branch!\e[0m\n");

  // It should be TARGET, as it is the last merged branch
  assert(l.cursor.index == TARGET);

  // Expeced to have 4 branchs,  and up to 4 merges (at least one...)
  lex_print_profiler(l);
}
