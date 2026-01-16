// ----------------- lex.h | made by Raffa064 --------------------
// Check out for new versions at http://github;com/Raffa064/lex.h

#ifndef lex_H
#define lex_H

#define LEX_VERSION 1

/*
 * ABOUT
 *
 * lex.h is a minimal sigle-header lexer library, designed to be fast, and readable.
 * The library could be entirely stack allocated without needing for heap allocation.
 *
 *
 * PRE-INCLUDE OPTIONS:
 *  - LEX_IMPLEMENTATION          Necessary to export lex.h implementations as it is a single-header library
 *  - LEX_STRIP_PREFIX            It will #define all library symbols without 'lex_' prefix 
 *  - LEX_TOKEN_NAME_OFFSET       Can be used to strip tokens name prefix (read more about on it's definition)
 *  - LEX_DISABLE_BUILTIN_RULES   Disables all builtin rules (use it if you wanna implement everything by yourself).
 */

#include <asm-generic/errno-base.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/// MACROS

/*
 * This macro is used to offset the TokenDef.name created by TOKENTYPE macro.
 * You can define this macro if your token's ID has some prefix.
 *
 * Ex:
 *
 * #define LEX_TOKEN_NAME_OFFSET 2
 * #include "lex.h"
 *
 * enum { T_KEYWORD } ExampleTokens;
 *
 * ...
 * TOKENTYPE(T_KEYWORD, lexer_rule_here) // The name for this token will be "KEYWORD" instead of "T_KEYWORD"
 * ...
 */
#ifndef LEX_TOKEN_NAME_OFFSET
#define LEX_TOKEN_NAME_OFFSET 0
#endif

/*
 * This macros is indent to be called inside a static 'TokenDef' array.
 * It's expected the parameter 'id' to be an global or local variable, 
 * where the value will be the index* of the defined token, and the it's 
 * name will be stored literally as it is, as the name** for this token.
 *
 * (*): The index determines the order of execution for the matching, so 
 *      it's recommend to take care about it.
 *
 * (**): The name will be stored as string, and if you are prefixing your 
 *       variables names with some sort of namespace, you can strip it out by 
 *       defining LEX_TOKEN_NAME_OFFSET to the size of your prefix. 
 *       Ex: "T_KEYWORD" -> "KEYWORD"
 *
 * Example Usage:
 *
 * int WS = 0;
 * LexTokenDef token_defs[TOKENS_COUNT] = {
 *  LEX_TOKENTYPE(WS, matching_rule, .skip = true) // defined token "WS" in the index 0
 * };
*/
#define LEX_TOKENTYPE(id, rulefn, ...) \
  [id] = (LexTokenType){ .name = ((const char*)#id) + LEX_TOKEN_NAME_OFFSET, .rule = rulefn, .opt = { __VA_ARGS__ } }

/*
 * This macro expects a 'TokenDef' static array, that could be either 
 * definined globally, or in local stack. 
 */
#define LEX_TOKENMAP(tkdefs) \
  (LexTokenMap) { \
    .token_defs = tkdefs, \
    .count = sizeof(tkdefs) / sizeof(tkdefs[0]) \
  }

/*
 * These two macros doesn't do a lot things, it's intent is all about legibility.
 * Use it to create and merge branches (which are just independent copies of the lexer) 
 * in order to creating complex parsers.
 */
#define LEX_BRANCH(l_ptr) (*l)                  // Ex: Lex b = LEX_BRANCH(l);   // Create a branch of l (which is a pointer)
#define LEX_MERGE_BRANCH(l_ptr, b) *l_ptr = b;  // Ex: LEX_MERGE_BRANCH(l, b);  // Apply changes from b into l

/*
 * Returns a pointer to the start of the token on the source code.
 * NOTE: It's not null-terminate as the source is ideally imutable.
 */
#define lex_tkstr(token) ((const char*)(token.cursor.source + token.cursor.index))

/*
 * Get the length of a token.
 */
#define lex_tklen(token) ((size_t)token.cursor.length)

/*
 * Get token name, defined by it's type on the TokenDef initialization.
 */
#define lex_tkname(lexer, token) ((const char*)lexer.map.token_defs[token.id].name)

/*
 * Get a pointer for the source code at cursor position
 */
#define lex_curstr(cursor) ((const char*)(cursor.source + cursor.index))

/*
 * Get a single char from the source code at the cursor position
 */
#define lex_curch(cursor) ((const char)(cursor.source[cursor.index]))

/*
 * Get cursor start index
 */
#define lex_curstart(cursor) ((size_t)(cursor.index))

/*
 * Get cursor end index
 */
#define lex_curend(cursor) ((size_t)(cursor.index + cursor.length))

/*
 * You can use this while creating 'rule functions' to indicate non matching cases.
 */
#define LEX_NO_MATCH 0

#ifdef LEX_IMPLEMENTATION
#define LEX_INLINE inline
#else
#define LEX_INLINE
#endif

/// STRUCTURES

typedef struct {
  size_t lineno, column;
} LexCursorPosition; 

typedef struct {
  const char* source;
  size_t index, length;
} LexCursor;

typedef size_t (*LexerRule)(LexCursor cursor);

typedef struct {
  /* When it's set, all the matched tokens with this option will not be
   * emitted by lex_current(), unless the Lex.no_skip flag is set.
   */
  bool skip;  
} LexTokenOptions;

typedef struct {
  const char* name;
  LexerRule rule;
  LexTokenOptions opt;
} LexTokenType;

typedef struct {
  LexTokenType *token_defs;
  size_t count;
} LexTokenMap;

typedef int LexTokenId;

typedef struct {
  LexCursor cursor;
  LexTokenId id;
} LexToken;

typedef struct {
  LexTokenMap map;
  LexCursor cursor;
  LexToken tk;
  bool has_token;  
  bool no_skip;     // Used to ignore TokenOptions.skip flag
} Lex;

/* 
 * LEX_INVALID_TOKEN:
 * Error code that's returned by lex_current when some 
 * input from current source couldn't be matched to any 
 * token definition (LexTokenDef).
 *
 * LEX_EOF:
 * It means that the lexer already reached the end of file.
 *
 * LEX_SUCESS:
 * The lexer sucessfully obtained a token from the current 
 * cursor position.
 */
typedef enum { 
  LEX_INVALID_TOKEN = -1, 
  LEX_EOF = 0,
  LEX_SUCESS = 1,
} LexResult;

/// FOWARD DECLARATIONS

/* 
 * Initializes a new lexer object on the stack.
 * It expects a LexTokenMap as fiest argument, which can be
 * created with the macro 'LEX_TOKENTYPE'
 */
Lex lex_init(LexTokenMap map, const char* source);

/*
 * Get a token from the current 'l->cursor' position.
 * The function returns true  whenever it sucessfully obtain 
 * a token, otherwise if it failed or reached the end of file, 
 * the return will be false. As the 'result' paramenter is optional
 * it could be passed as NULL, but if it's not, it will be set to the 
 * corresponding 'LexResult' value. 
 *
 * Also, 'l->has_token' is set to true if it succeed.
 */
bool lex_current(Lex* l, LexResult* result);

/*
 * Matches current token with the given 'id', if it matched, returns true 
 * and set optional parameter 'tk' to consumed token. It will automatically call 
 * for 'lex_move' when it sucessfully matches the desired id, and just return false 
 * otherwise.
 */
bool lex_consume(Lex* l, LexToken* tk, LexTokenId id);

/*
 * It will conditionally consume the current token if it's id and string value
 * matches to the given 'id' and 'match' parameters respectively. 
 * The 'match_len' parameter is useful for string without nullbyte terminator.
 * If it is not the case, you could use the simplified version 'lex_skip'.
 */
bool lex_skipn(Lex* l, LexTokenId id, const char* match, size_t match_len);

/*
 * It will conditionally consume the current token if it's id and string value
 * matches to the given 'id' and 'match' parameters respectively. 
 */
bool lex_skip(Lex* l, LexTokenId id, const char* match);

/*
 * Move "l->cursor" to a custom location.
 */
void lex_move_to(Lex* l, LexCursor to);

/*
 * Move cursor to the end of current token.
 * If 'l->has_token' is false, it will do nothing.
 */
void lex_move(Lex* l);

/*
 * Utilitary function, that can be used to match the char at the current cursor
 * position with a given list of chars. 'chars' could be a string or an array.
 *
 * If matched, returns the length of the match, that in the case of char can only be 1,
 * otherwise it returns 0 (LEX_NO_MATCH).
 */
size_t lex_match_charsn(LexCursor cursor, const char* chars, int count);

/*
 * Utilitary function, that can be used to match the char at the current cursor
 * position with a given list of chars.
 *
 * If matched, returns the length of the match, otherwise LEX_NO_MATCH.
 *
 * NOTE: This function expects a null terminated sequence of chars, that
 * could be either an string literal, or a manually configured array of chars 
 * ending with '\0'. If it doesn't fit to your needs, you can also use 'lex_match_charsn'.
 */
size_t lex_match_chars(LexCursor cursor, const char* chars);

/*
 * Utilitary function for matching "keywords", with builtin boundary check.
 *
 * If matched, returns the length of the match, otherwise LEX_NO_MATCH.
 */
size_t lex_match_keywordn(LexCursor cursor, const char* keyword, size_t len);

/*
 * Utilitary function (specifically made) for matching "keywords", with builtin boundary check.
 *
 * If matched, returns the length of the match, otherwise LEX_NO_MATCH.
 */
size_t lex_match_keyword(LexCursor cursor, const char* keyword);

/*
 * Utilitary function for matching string-like structures.
 *
 * If 'can_be_scaped' is set, it will allows the occurrency of 'delimiter' inside the match 
 * if it's preceeded by '\'.
 *
 * If matched, returns the length of the match, otherwise LEX_NO_MATCH.
 */

size_t lex_match_wrapped(LexCursor cursor, const char delimiter, bool can_be_scaped);

/*
 * Utilitary function for exact matching.
 *
 * If matched, returns the length of the match, otherwise LEX_NO_MATCH.
 */
size_t lex_match_exactn(LexCursor cursor, const char* match, size_t len);

/*
 * Utilitary function for exact matching.
 * This function expects a null-terminated string for 'match'.
 *
 * If matched, returns the length of the match, otherwise LEX_NO_MATCH.
 */
LEX_INLINE size_t lex_match_exact(LexCursor cursor, const char* match);

/*
 * Utilitary function for matching a region that starts with some prefix, and ends with some suffix.
 *
 * NOTE: If 'optional_suffix' is set, it will match to the end of file when sufix is not found.
 */
size_t lex_match_region(LexCursor cursor, const char* prefix, const char* suffix, bool optional_suffix);

/*
  * Copy token string value to a internal static buffer.
  * The buffer size is fixed at 1024 bytes, if the token exceeds this limit
  * the string value will be clamped to this maximum value.
  * NOTE: As a static buffer it will be overriden by every call, so don't use it
  * for anything other than debug.
  */
const char* lex_tkstr_tmp(LexToken tk);

/*
 * Copy token string value to a heap allocated buffer.
 * The library is not responsible for deallocate it.
 */
char* lex_tkstr_dup(LexToken tk);

/*
 * Get cursor column position by searching for the first occurrency of 
 * '\n' before the 'cursor->index'. 
 */
size_t lex_curcol(LexCursor cursor);

/*
 * Get cursor line position by scanning all occurrencies of '\n' before 'cursor->index'
 */
size_t lex_curline(LexCursor cursor);

/*
 * Reset cursor length and position to zero
 */
LEX_INLINE void lex_curreset(LexCursor *cursor);

/*
 * Get cursor line and column positions as a LexCursorPosition object. 
 * That's an alias for both  'lex_curcol' and 'lex_curline'.
 */
LEX_INLINE LexCursorPosition lex_curpos(LexCursor cursor);

/*
 * Move cursor by N chars. N could be a negative value, meaning that the cursor will mobe backward.
 */
LEX_INLINE void lex_curmove(LexCursor *cursor, ssize_t N);

/*
 * Print source code to the console, colorizing diferent tokens.
 *
 * Use 'print_labels' to show a text with the token names for each color after the code. 
 */
void lex_print_hl(Lex l, bool print_caption);

#ifndef LEX_DISABLE_BUILTIN_RULES
/*
 * Built-in rule for White-Space tokens.
 * It uses isscape() from 'ctype.h' as a matching rule.
 * NOTE: It's recommend to set 'skip' flag on token type definition (TokenOptions)
 */
size_t lex_builtin_rule_ws(LexCursor cursor);

/*
 * Built-in rule for Identifiers following the most common pattern found on modern languages,
 * useful for user defined names (such as variables and class names).
 * The same as [a-zA-Z$_][a-zA-Z$_0-9]*
 * NOTE: It's usually a good practice to place IDs at the end of your LexTokenMap to 
 * prevent it to override other token types. 
 */
size_t lex_builtin_rule_id(LexCursor cursor);

/*
 * Built-in rule for double quoted strings. (Ex: "Hello world" ) 
 * NOTE: It already handles scaped delimiters < \" >, but scaping the final string is not handled 
 * by the library since the input source code is indent to be imutable.
 */
LEX_INLINE size_t lex_builtin_rule_dqstring(LexCursor cursor);

/*
 * Built-in rule for single quoted strings (Ex: 'Hello world' ). 
 * NOTE: It already handles scaped delimiters < \' >, but scaping the final string is not handled 
 * by the library since the input source code is indent to be imutable.
 */
LEX_INLINE size_t lex_builtin_rule_sqstring(LexCursor cursor);

/*
 * Built-in rule for JavaScript/Python-like string, which can be both single/double 
 * quoted (Ex: "Hello world" or 'Hello world' ). 
 * NOTE: It already handles scaped delimiters < \" | \' >, but scaping the final string is not handled 
 * by the library since the input source code is indent to be imutable.
 */
size_t lex_builtin_rule_string(LexCursor cursor);

/*
 * Built-in rule for  Python-like sharp comments.
 * Ex: # This is a comment
 */
LEX_INLINE size_t lex_builtin_rule_pylike_comment(LexCursor cursor);

/*
 * Built-in rule for  asm-like semicolon comments.
 * Ex: ; This is a comment
 */
LEX_INLINE size_t lex_builtin_rule_asmlike_comment(LexCursor cursor);

/*
 * Built-in rule for  C-like double-dash comments.
 * Ex: // This is a comment
 */
LEX_INLINE size_t lex_builtin_rule_clike_comment(LexCursor cursor);

// Built-in rule for  C-like multiline comments.
// Ex:  /* This is c-like ml-comment */ 
LEX_INLINE size_t lex_builtin_rule_clike_mlcomment(LexCursor cursor);

#endif

#ifdef LEX_IMPLEMENTATION

#include <ctype.h>
#include <stddef.h>
#include <string.h>

Lex lex_init(LexTokenMap map, const char* source) {
  LexTokenType unset = {0};

  for (int i = 0; i < map.count; i++) {
    if (memcmp(&map.token_defs[i], &unset, sizeof(LexTokenType)) == 0) {
      char msg[512];
      sprintf(msg, "Lex initialization failed due to an missing token definition. TokenId '%d' is unset", i);
      errno = EINVAL;
      perror(msg);
      exit(1);
    }
  }

  return (Lex){
    .map = map,
    .cursor = { .source = source,}
  };
}

bool lex_current(Lex* l, LexResult* result) {
  if (l->has_token) {
    if (result)
      *result = LEX_SUCESS;

    return true;
  }

  LexCursor cursor = l->cursor;

  if (cursor.source[cursor.index] == '\0') {
    if (result)
      *result = LEX_EOF;

    return false;
  }

  for (LexTokenId id = 0; id < l->map.count; id++) {
    LexTokenType tkdef = l->map.token_defs[id];
    size_t len = tkdef.rule(cursor);

    if (len != LEX_NO_MATCH) {
      cursor.length = len;

      l->cursor = cursor;
      l->tk = (LexToken){
        .cursor = cursor,
        .id = id,
      };
      l->has_token = true;

      if (!l->no_skip && l->map.token_defs[id].opt.skip) {
        lex_move(l);
        return lex_current(l, result);
      }

      if (result)
        *result = LEX_SUCESS;

      return true;      
    }
  }

  if (result)
    *result = LEX_INVALID_TOKEN;

  return false;
}
 

bool lex_consume(Lex* l, LexToken* tk, LexTokenId id) { 
  if (lex_current(l, NULL)) {
    if (l->tk.id == id) {
      if (tk)
        *tk = l->tk;

      lex_move(l);
      return true;
    }
  }

  return false;
}

bool lex_skipn(Lex* l, LexTokenId id, const char* match, size_t match_len) {
  Lex b = LEX_BRANCH(l);

  LexToken tk;
  if (lex_consume(&b, &tk, id)) {
    if (lex_tklen(b.tk) == match_len && strncmp(lex_tkstr(tk), match, match_len) == 0) {
      LEX_MERGE_BRANCH(l, b);
      return true;
    }
  }

  return false;
}

bool lex_skip(Lex* l, LexTokenId id, const char* match) {
  return lex_skipn(l, id, match, strlen(match));
}

void lex_move_to(Lex* l, LexCursor to) {
  l->cursor = to;
  l->has_token = false;
}

void lex_move(Lex* l) {
  if (l->has_token) {
    l->cursor.index += l->cursor.length;
    l->cursor.length = 0;
    l->has_token = false;
  }
}

size_t lex_match_charsn(LexCursor cursor, const char* chars, int count) {
  for (int i = 0; i < count; i++) {
    if (cursor.source[cursor.index] == chars[i])
      return 1; // sizeof char 
  }

  return LEX_NO_MATCH;
}

size_t lex_match_chars(LexCursor cursor, const char* chars) {
  return lex_match_charsn(cursor, chars, strlen(chars));
}

size_t lex_match_keywordn(LexCursor cursor, const char* keyword, size_t len) {
  const char *str = lex_curstr(cursor);

  if (strncmp(str, keyword, len) == 0) {
    char lookahead = str[len];

    if (isalnum(lookahead)) // boundary check: prevent cases like 'interrupt' to be split into 'int' 'errupt' (keyword/id)
      return LEX_NO_MATCH;

    return len;
  }

  return LEX_NO_MATCH;
}

size_t lex_match_keyword(LexCursor cursor, const char* keyword) {
  return lex_match_keywordn(cursor, keyword, strlen(keyword));
}

size_t lex_match_wrapped(LexCursor cursor, const char delimiter, bool can_be_scaped) {
  const char *str = lex_curstr(cursor);
  
  if (str[0] == delimiter) {
    for (size_t len = 1; str[len] != '\0'; len++) {
      if (str[len] == delimiter) {
        if (can_be_scaped) {
          if (str[len - 1] == '\\')  // ignore scaped delimiter
            continue;
        }

        return len + 1; // following inclusive-exclusive convention
      }
    }
  }

  return LEX_NO_MATCH;
}

size_t lex_match_exactn(LexCursor cursor, const char* match, size_t len) {
  const char *str = lex_curstr(cursor);

  if (strncmp(str, match, len) == 0)
    return len;

  return LEX_NO_MATCH;
}

size_t lex_match_exact(LexCursor cursor, const char* match) {
  return lex_match_exactn(cursor, match, strlen(match));
}

size_t lex_match_region(LexCursor cursor, const char* prefix, const char* suffix, bool optional_suffix) {
  size_t prefix_len = strlen(prefix);
  size_t suffix_len = strlen(suffix);

  if (lex_match_exact(cursor, prefix)) {
    LexCursor cur = cursor;
    lex_curmove(&cur, prefix_len);

    bool found_suffix = false;
    while (lex_curch(cur) != '\0') {
      if (lex_match_exact(cur, suffix)) {
        lex_curmove(&cur, suffix_len);
        found_suffix = true;
        break;
      }
      
      lex_curmove(&cur, 1);
    }

    if (optional_suffix || found_suffix)
      return lex_curstart(cur) - lex_curstart(cursor);
      
    return LEX_NO_MATCH; // Non-optional suffix
  }

  return LEX_NO_MATCH;  
}

const char* lex_tkstr_tmp(LexToken tk) {
  static char buf[1024];

  size_t len = lex_tklen(tk);
  if (len > sizeof(buf))
    len = sizeof(buf);
  
  strncpy(buf, lex_tkstr(tk), len);
  buf[len] = '\0';
  return buf;
}

char* lex_tkstr_dup(LexToken tk) {
  size_t len = lex_tklen(tk);
  char* buf = malloc(len);
  strncpy(buf, lex_tkstr(tk), len);
  return buf;
}

size_t lex_curcol(LexCursor cursor) {
  size_t col = 0;
  for (int i = lex_curstart(cursor) - 1; i >= 0; i--, col++) {
    if (cursor.source[cursor.index + i] == '\n')
      return col;
  }

  return col;
}

size_t lex_curline(LexCursor cursor) {
  size_t lines = 1;
  for (int i = 0; i < lex_curstart(cursor); i++) {
    if (cursor.source[i] == '\n')
      lines++;
  }

  return lines;
}

void lex_curreset(LexCursor *cursor) {
  cursor->index = cursor->length = 0;
}

LexCursorPosition lex_curpos(LexCursor cursor) {
  return (LexCursorPosition) { 
    .lineno = lex_curline(cursor), 
    .column = lex_curcol(cursor),
  };
}

void lex_curmove(LexCursor *cursor, ssize_t N) {
  cursor->index += N;
}

void lex_print_hl(Lex l, bool print_labels) {
  // It has 42 different highlights
  static const int colors[] = { 31, 32, 33, 34, 35, 36, 37 };
  static const int styles[] = { 0, 1, 3, 4, 7, 9 };

  lex_curreset(&l.cursor); // return to begining of file
  l.no_skip = true;

  while (lex_current(&l, NULL)) {
    int s = styles[l.tk.id / 6];
    int c = colors[l.tk.id % 7];
    
    printf("\e[%d;%dm%.*s", s, c, (int)lex_tklen(l.tk), lex_tkstr(l.tk));
    lex_move(&l);
  }

  printf("\e[0m\n");

  if (print_labels) {
    for (int  i = 0; i < l.map.count; i++) {
      int s = styles[i / 6];
      int c = colors[i % 7];
      printf("\e[%d;%dm%s ", s, c, l.map.token_defs[i].name);
    }
    
    printf("\e[0m\n");
  }
}

#ifndef LEX_DISABLE_BUILTIN_RULES

size_t lex_builtin_rule_ws(LexCursor cursor) { 
  size_t len = LEX_NO_MATCH;

  for (;; len++) {
    char ch = cursor.source[cursor.index + len];

    if (ch == '\0' || !isspace(ch))
      break;
  }

  return len; 
}

size_t lex_builtin_rule_id(LexCursor cursor) {
  const char *start = lex_curstr(cursor);
  if (isalpha(*start) || *start == '$' || *start == '_') {
    for (int len = 1; start[len] != '\0'; len++) {
      char ch = start[len];
      if (isalnum(ch) || ch == '$' || ch == '_')
        continue;

      return len;
    }
  }

  return LEX_NO_MATCH;
}

size_t lex_builtin_rule_dqstring(LexCursor cursor) {
  return lex_match_wrapped(cursor, '"', true);
}

size_t lex_builtin_rule_sqstring(LexCursor cursor) {
  return lex_match_wrapped(cursor, '\'', true);
}

size_t lex_builtin_rule_string(LexCursor cursor) {
  size_t len = lex_match_wrapped(cursor, '"', true);
  if (len != LEX_NO_MATCH)
    return len;

  return lex_match_wrapped(cursor, '\'', true);
}

size_t lex_builtin_rule_pylike_comment(LexCursor cursor) {
  return lex_match_region(cursor, "#", "\n", true);
}

size_t lex_builtin_rule_asmlike_comment(LexCursor cursor) {
  return lex_match_region(cursor, ";", "\n", true);
}

size_t lex_builtin_rule_clike_comment(LexCursor cursor) {
  return lex_match_region(cursor, "//", "\n", true);
}

size_t lex_builtin_rule_clike_mlcomment(LexCursor cursor) {
  return lex_match_region(cursor, "/*", "*/", false);
}

#endif

#endif


#ifdef LEX_STRIP_PREFIX

/* 
 * By defining this macro before including lex.h, you'll be able to 
 * access any libray symbol without prefixing with 'lex'.
 *
 * NOTE: If there's some naming conflics with your project libraries,
 * you could use "#undef SYMBOL" to remove it.
 *
 * Ex:
 * #define LEX_STRIP_PREFIX
 * #include <lex.h>
 * #undef SUCESS
 * #define SUCESS "overriden symbol"
 */

/// NO PREFIX MACROS
#define NO_MATCH LEX_NO_MATCH
#define TOKENMAP LEX_TOKENMAP
#define TOKENTYPE LEX_TOKENTYPE
#define BRANCH LEX_BRANCH
#define MERGE_BRANCH LEX_MERGE_BRANCH
#define tkstr lex_tkstr
#define tklen lex_tklen
#define tkname lex_tkname
#define curstr lex_curstr
#define curch lex_curch
#define curstart lex_curstart
#define curend lex_curend

/// NO PREFIX STRUCTURES
#define CursorPosition LexCursorPosition
#define Cursor LexCursor
#define TokenOptions LexTokenOptions
#define TokenType LexTokenType
#define TokenMap LexTokenMap
#define TokenId LexTokenId
#define Token LexToken

/// NO PREFIX FUNCTIONS
#define init lex_init
#define current lex_current
#define consume lex_consume
#define skipn lex_skipn
#define skip(l, id, match) lex_skip // This skip macro conflicts with skip flag from LexTokenOptions
#define move_to lex_move_to
#define move lex_move
#define match_charsn lex_match_charsn
#define match_chars lex_match_chars
#define match_keywordn lex_match_keywordn
#define match_keyword lex_match_keyword
#define match_wrapped lex_match_wrapped
#define match_exactn lex_match_exactn
#define match_exact lex_match_exact
#define match_region lex_match_region
#define tkstr_tmp lex_tkstr_tmp
#define tkstr_dup lex_tkstr_dup
#define curcol lex_curcol
#define curline lex_curline
#define curpos lex_curline
#define curmove lex_curmove
#define print_hl lex_print_hl

#ifndef LEX_DISABLE_BUILTIN_RULES
#define builtin_rule_ws lex_builtin_rule_ws
#define builtin_rule_id lex_builtin_rule_id
#define builtin_rule_dqstring lex_builtin_rule_dqstring
#define builtin_rule_sqstring lex_builtin_rule_sqstring
#define builtin_rule_string lex_builtin_rule_string
#define builtin_rule_pylike_comment lex_builtin_rule_pylike_comment
#define builtin_rule_asmlike_comment lex_builtin_rule_asmlike_comment
#define builtin_rule_clike_comment lex_builtin_rule_clike_comment
#define builtin_rule_clike_mlcomment lex_builtin_rule_clike_mlcomment
#endif

#endif

#endif
