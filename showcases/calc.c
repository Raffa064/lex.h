#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEX_IMPLEMENTATION
#include "../lex.h"

/*
 * This example shows how to create a simple mathematical expresion evaluator using lex.h as tokenizer.
 */

enum {
  CALC_WS,     // white spaces
  CALC_PARENS, // '(' and ')'
  CALC_NUM,    // Numbers such as '0.2', '1', '+2', '-3', '-3.', '.2'
  CALC_OP,     // '+', '-', '*', '/'
  CALC_COUNT
};

size_t calc_rule_parens(LexCursor cursor);
size_t calc_rule_num(LexCursor cursor);
size_t calc_rule_op(LexCursor cursor);

LexType calc_types[CALC_COUNT] = {
  LEX_TYPE(CALC_WS,     lex_builtin_rule_ws, .skip = true),
  LEX_TYPE(CALC_NUM,    calc_rule_num),
  LEX_TYPE(CALC_OP,     calc_rule_op),
  LEX_TYPE(CALC_PARENS, calc_rule_parens),
}; 

enum { 
  ERROR_EXPECTED_CPAREN,
  ERROR_INVALID_OPERAND,
  ERROR_EXTRANEOUS_TOKEN,
  ERROR_INVALID_TOKEN,
  ERROR_COUNT,
};

int calc_error;

bool eval_expr(Lex* lex, double *out);

int main() {
  printf("Lex Calculator v1.0\n\tUse 'q' to quit.\n");

  char *input = NULL;
  size_t input_len;

  for (;;) {
    printf(">> "), fflush(stdout);

    if (getline(&input, &input_len, stdin) >= 0) {
      input[input_len] = '\0';

      if (input[0] == 'q')
        break;
  
      Lex lex = lex_init(LEX_TYPEARRAY(calc_types), input);

      double result;
      if (eval_expr(&lex, &result)) {
        LexResult res;
        if (lex_current(&lex, &res)) {
          calc_error = ERROR_EXTRANEOUS_TOKEN;
        } else if (result == LEX_INVALID_TOKEN) {
          calc_error = ERROR_INVALID_TOKEN;
        } else {
          printf("= %g\n", result);
          continue;
        }
      }
      
      const char *err_msgs[ERROR_COUNT] = {
        [ERROR_EXPECTED_CPAREN]  = "Expected ')'.",
        [ERROR_INVALID_OPERAND]  = "Invalid operand.",
        [ERROR_EXTRANEOUS_TOKEN] = "Extraneous token.",
        [ERROR_INVALID_TOKEN]    = "Invalid token.",
      };
      
      printf("   \e[31m%*s ERROR: %s\e[0m\n", (int)lex_curcol(lex.cursor), "^", err_msgs[calc_error]);
    }
  }
}

/// CUSTOM LEXER RULES

size_t calc_rule_num(LexCursor cursor) {
  bool dot = false;
  
  size_t len = LEX_NO_MATCH;
  for (;;) {
    char ch = lex_curch(cursor);
    
    if (ch == '\0') break;

    if (isdigit(ch)) {
      lex_curmove(&cursor, 1);
      len++;
      continue;
    }

    if (ch == '.') {
      if (dot) return LEX_NO_MATCH; // Double dotted number is invalid -> '1.2.2'

      dot = true;
      lex_curmove(&cursor, 1);
      len++;
      continue;
    }

    break;
  }

  return len;
}

size_t calc_rule_op(LexCursor cursor) { return lex_match_chars(cursor, "+-*/"); }

size_t calc_rule_parens(LexCursor cursor) { return lex_match_chars(cursor, "()"); }


/// PARSER

// An atom is the smallest part of the expresion (aka numbers), which actually can be 
// also be a entire expresion wrapped by parens.
// atom = ([+-]?  CALC_NUM) | expr
bool eval_atom(Lex *lex, double *out) {
  LexToken tk;

  // Plus sign could be ignored Ex: '+3' == '3', '+(2 + 1)' == '(2 + 1)'
  lex_skip(lex, CALC_OP, "+");    
  
  bool is_negative = false;
  if (lex_skip(lex, CALC_OP, "-"))
    is_negative = true;

  if (lex_consume(lex, &tk, CALC_NUM)) {
    double result = atof(lex_tkstr_tmp(tk));

    if (is_negative)
      *out = -result;
    else *out = result;

    return true;
  }

  if (lex_skip(lex, CALC_PARENS, "(")) {
    bool result = eval_expr(lex, out);
    
    if (!lex_skip(lex, CALC_PARENS, ")")) {
      calc_error = ERROR_EXPECTED_CPAREN;
      return false;
    }

    return result;
  }

  calc_error = ERROR_INVALID_OPERAND;
  return false;
}

// A term is a stack of multiplication/division of "atoms"
// term = atom ( [*/] atom )*
bool eval_term(Lex *lex, double *out) {
  double lhs, rhs; // left/right hand side

  if (!eval_atom(lex, &lhs))
    return false;

  Lex b = LEX_BRANCH(lex);
  LexToken tk;
  while (lex_consume(&b, &tk, CALC_OP)) {
    if (!eval_atom(&b, &rhs))
      return false;
    
    if (strcmp(lex_tkstr_tmp(tk), "*") == 0) {
      lhs *= rhs;
    } else if (strcmp(lex_tkstr_tmp(tk), "/") == 0) {
      lhs /= rhs;
    } else break;

    LEX_MERGE_BRANCH(lex, b); // apply `lex_consume` to lex
  }

  *out = lhs;
  return true;
}

// An expression is a stack of addition/subtraction of "terms"
// expr = term ( [+-] term )*
bool eval_expr(Lex* lex, double *out) {
  double lhs, rhs; // left/right hand side

  if (!eval_term(lex, &lhs))
    return false;

  Lex b = LEX_BRANCH(lex);
  LexToken tk;
  while (lex_consume(&b, &tk, CALC_OP)) {
    if (!eval_term(&b, &rhs))
      return false;
    
    if (strcmp(lex_tkstr_tmp(tk), "+") == 0) {
      lhs += rhs;
    } else if (strcmp(lex_tkstr_tmp(tk), "-") == 0) {
      lhs -= rhs;
    } else {
      // It should never haven as we do not have any operator with
      // lower precedence than '+' and '-'.
      assert(0); 
    }

    LEX_MERGE_BRANCH(lex, b);
  }

  *out = lhs;
  return true;
}
