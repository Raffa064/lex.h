// ----------------- lex.h | made by Raffa064 --------------------
// Check out for new versions at https://github.com/Raffa064/lex.h

#ifndef LEX_H
#define LEX_H

#define LEX_VERSION 4

/*
 * ABOUT
 *
 * lex.h is a minimal sigle-header lexer library, designed to be fast, and readable.
 * The library could be entirely stack allocated without needing for heap allocation.
 *
 *
 * PRE-INCLUDE OPTIONS:
 * - LEX_IMPLEMENTATION             Required to emit the implementation when using 'lex.h' as it is a single-header library.
 * - LEX_STRIP_PREFIX               It will '#define' all library symbols without 'lex_' prefix.
 * - LEX_TYPE_NAME_OFFSET           Can be used to strip type name prefix (read more about on it's definition).
 * - LEX_DISABLE_BUILTIN_RULES      Disables all builtin rules (use it if you wanna implement everything by yourself).
 * - LEX_PROFILER                   Enable profiling will expose implemenmtaion for 'lex_print_profiler', which can be used to print information about lex execution.
 * - LEX_DISABLE_COLORS             Disable colorful outputs.
 * - LEX_USE_XMACRO                 Enables useful macros for using X-macro pattern.
 */

/// INCLUDES

#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <stdint.h>

#ifdef LEX_PROFILER
#include <time.h>
#endif

#ifdef LEX_IMPLEMENTATION
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#endif // LEX_IMPLEMENTATION

/// MACROS

// This tag is used for decorate optional parameters (aka. nullable)
#define LEX_OPTIONAL

/*
 * This macro is used to offset the LexType.name created by TYPE macro.
 * You can define this macro if your token's ID has some prefix.
 *
 * Ex:
 *
 * #define LEX_TYPE_NAME_OFFSET 2 // length of "T_"
 * #include "lex.h"
 *
 * enum { T_KEYWORD } ExampleTokens;
 *
 * ...
 * TYPE(T_KEYWORD, lexer_rule_here) // The name for this token will be "KEYWORD" instead of "T_KEYWORD"
 * ...
 */
#ifndef LEX_TYPE_NAME_OFFSET
  #define LEX_TYPE_NAME_OFFSET 0
#endif

/*
 * This macros is indent to be called ONLY inside static 'TokenType' arrays.
 * If you need to use it in another context, use LEX_ETYPE instead.
 *
 * 'id' is expected to be global or local int type variable, where it's value 
 * will be the index* of the defined type inside the array, and the it's 
 * name will be stored literally as it is, as the name** for this token.
 *
 * (*): The index determines the order of execution for the matching, so 
 *      it's recommend to take care about it. Also, the enclosing array capacity 
 *      should be greater than the 'id'.
 *
 * (**): The name will be stored as string, and if you are prefixing your 
 *       variables names with some sort of namespace, you can strip it out by 
 *       defining LEX_TYPE_NAME_OFFSET to the size of your prefix. 
 *       Ex: "T_KEYWORD" -> "KEYWORD"
 *
 * Example Usage:
 *
 * int WS = 0; // enums are the best way to defining ids.
 * 
 * LexType items[TOKENS_COUNT] = {
 *  LEX_TYPE(WS, matching_rule, .skip = true) // defined token "WS" in the index 0
 * };
*/
#define LEX_TYPE(id, rulefn, ...) \
  [id] = (LexType){ .name = ((const char*)#id) + LEX_TYPE_NAME_OFFSET, .rule = rulefn, .opt = { __VA_ARGS__ } }

/*
 * The same as LEX_TYPE, but it doesn't use implicit index designator, so it can be used 
 * outside static arrays as a normal expression.
 */
#define LEX_ETYPE(id, rulefn, ...) \
    (LexType){ .name = ((const char*)#id) + LEX_TYPE_NAME_OFFSET, .rule = rulefn, .opt = { __VA_ARGS__ } }

/*
 * This macro is an utility to create a LexTypeArray from a static LexType array.
 */
#define LEX_TYPEARRAY(_items) \
  (LexTypeArray) { \
    .items = _items, \
    .count = sizeof(_items) / sizeof(_items[0]) \
  }

/*
 * When LEX_PROFILER is set, this macros can be used to track branching operations.
 * Otherwise, when LEX_PROFILER is not defined, these two macros doesn't do a lot things, but it still being recommend to improve legibility.
 * Use it to create and merge branches (which are just independent copies of the lexer) 
 * in order to creating complex parsers.
 */
#define LEX_BRANCH(l_ptr) *(l_ptr)                  // Ex: Lex b = LEX_BRANCH(l);   // Create a branch of l (which is a pointer)
#define LEX_MERGE_BRANCH(l_ptr, b) *(l_ptr) = (b);  // Ex: LEX_MERGE_BRANCH(l, b);  // Apply changes from b into l

#define LEX_SRC(_file_name, _contents) ((LexSourceInfo) { .content = (_contents), .file_name = (_file_name), })

/*
 * Returns a pointer to the start of the token on the source code.
 * NOTE: It's not null-terminate as the source is ideally imutable.
 */
#define lex_tkstr(token) ((const char*)(token.sv.begin))

/*
 * Get the length of a token.
 */
#define lex_tklen(token) (lex_view_count(token.sv))

/*
 * Get token type name.
 */
#define lex_tkname(lexer, token) ((const char*)lexer.types.items[token.id].name)

/*
 * Get a pointer for the source code at cursor position.
 */
#define lex_cursor_str(cursor) ((const char*)((cursor).source + (cursor).index))

/*
 * Get a single char from the source code at the cursor position.
 */
#define lex_cursor_ch(cursor) ((const char)((cursor).source[(cursor).index]))

/*
 * Get cursor start index.
 */
#define lex_cursor_start(cursor) ((size_t)(cursor.index))

/*
 * Get cursor end index.
 */
#define lex_cursor_end(cursor) ((size_t)(cursor.index + cursor.length))

/*
 * Get a pointer for the source code at specified index
 */
#define lex_source(lex, index) ((const char*) lex.cursor.source + index)

/*
 * Format string for LexStringView
 */
#define LEX_SVFMT "%.*s"

/*
 * Shorthand for using string views inside format strings args.
 * Ex: printf("sv: " LEX_SVFMT, lex_svarg(my_sv));
 */
#define lex_svarg(sv) ((int)lex_view_count(sv)), ((sv).begin)

/*
 * Format string for LexLocation
 */
#define LEX_LOCFMT "%s:%zu:%zu"

/*
 * Shorthand for using LexLocation inside format strings. 
 * Ex: printf("sv: " LEX_LOCFMT, lex_locarg(my_loc));
 */
#define lex_locarg(loc) (loc).file_name, (loc).row, (loc).col

/*
 * You can use this while creating 'rule functions' to indicate non matching cases.
 */
#define LEX_NO_MATCH 0

#ifdef LEX_IMPLEMENTATION
  // Use 'inline' only for implementation, it prevents warnings
  #define LEX_INLINE inline 
#else
  #define LEX_INLINE
#endif // LEX_IMPLEMENTATION

#ifdef LEX_DISABLE_COLORS
  #define LEX_COLOR_RESET
#else
  #define LEX_COLOR_RESET "\e[0m"
#endif


#ifdef LEX_USE_XMACRO

// Automatic enum definition for XTABLE
#define __LEX_ENUMX_FILTER(id, ...)id,
#define LEX_ENUMX(XTABLE) enum { XTABLE(__LEX_ENUMX_FILTER) XTABLE##_COUNT }

// Create a static LexType array using XTABLE
#define __LEX_TYPEX_FILTER(...) LEX_TYPE(__VA_ARGS__),
#define LEX_TYPEX(XTABLE) { XTABLE(__LEX_TYPEX_FILTER) }

// Automatically forward declare all rules on a XTABLE
#define __LEX_XFORWARD_DECL_FILTER(_, rule_fn, ...) size_t rule_fn(LexCursor cursor); 
#define LEX_XFORWARD_DECL(XTABLE) XTABLE(__LEX_XFORWARD_DECL_FILTER)

// This macro pushes single source of truth to it's limits!
// It will create a enum of types, a LexType array, and forward declarations for each rule!
#define LEX_XMACRO_FRAMEWORK(XTABLE, XEnum, x_types) \
  LEX_XFORWARD_DECL(XTABLE); \
  typedef LEX_ENUMX(XTABLE) XEnum; \
  [[maybe_unused]] static LexType x_types[XTABLE##_COUNT] = LEX_TYPEX(XTABLE) 

/*
 * Using X-macros you can make really cool stuff in lex.h, pushing single source of truth to it's limits:
 *
 * // Any changes to this "X-table" will affect both ExampleTokens enum and types array.
 * #define EXAMPLE(X) \
 *   X(EX_WS, lex_builtin_rule_ws, .skip = true) \
 *   X(EX_ID, lex_builtin_rule_id)
 *
 * typedef LEX_ENUMX(EXAMPLE) ExampleTokens;
 *
 * LexType types[EXAMPLE_COUNT] = LEX_TYPEX(EXAMPLE);
 *
 * int main() {
 *   Lex l = lex_init(LEX_TYPEARRAY(types), "this is an example");
 *   lex_print_hl(l, true);
 * }
 */

#endif

/// STRUCTURES

typedef struct Lex Lex;

#ifdef LEX_PROFILER
typedef struct {
  uint64_t n_time;
  int call_count;
} LexType_ProfileData;

typedef struct {
  int call_count;     // Total call count
  int dummy_calls;    // Immediate returned calls due to cached results
  int skipped_calls;  // Number of calls automatically skipped by 'lex_current' (for tokens with .skip flag, like WS and commments)
  int success_count;  // Number of calls which successfully returns some useful stuff 
} Lex_ProfilerStats;

typedef struct {
  Lex* root;

  int branch_count;
  int merge_count;
  
  // Main function stats
  Lex_ProfilerStats lex_current;
  Lex_ProfilerStats lex_consume;
  Lex_ProfilerStats lex_skip;
} Lex_ProfileData;

#undef LEX_BRANCH
#define LEX_BRANCH(l_ptr) __lex_profiler_branch_wrapper(l_ptr);

#undef LEX_MERGE_BRANCH
#define LEX_MERGE_BRANCH(l_ptr, b) __lex_profiler_merge_branch_wrapper(l_ptr, b);

Lex __lex_profiler_branch_wrapper(Lex* l_ptr);
void __lex_profiler_merge_branch_wrapper(Lex* l_ptr, Lex b);

#endif // LEX_PROFILER

typedef struct {
  const char *source;
  size_t index, length;
} LexCursor;

typedef size_t (*LexerRule)(LexCursor cursor);

typedef struct {
  /* When it's set, all the matched tokens with this option will not be
   * emitted by lex_current(), unless the Lex.no_skip flag is set.
   */
  bool skip;  
} LexTypeOptions;

typedef struct {
  const char* name;
  LexerRule rule;
  LexTypeOptions opt;

#ifdef LEX_PROFILER
  LexType_ProfileData profile_data;
#endif
} LexType;

typedef size_t LexTypeIndex;

typedef struct {
  LexType *items;
  LexTypeIndex count;
} LexTypeArray;

typedef struct {
  const char *begin, *end;// inclusive/exclusive
} LexStringView;

typedef struct {
  // This structure represents that file tags, like "main.c:17:1"
  // You can use LEX_LOCFMT to print it out
  const char *file_name;
  LexStringView line;
  size_t row, col;
} LexLocation; 

typedef struct {
  LexStringView sv;
  LexTypeIndex id;
} LexToken;

typedef struct {
  const char *content;
  const char *file_name;
} LexSourceInfo;

struct Lex {
  LexSourceInfo src;
  LexTypeArray types;
  LexCursor cursor;
  LexToken tk;
  bool has_token;  
  bool no_skip;     // Used to ignore TypeOptions.skip flag

#ifdef LEX_PROFILER
  Lex_ProfileData profiler_data;
#endif
};

/* 
 * LEX_INVALID_TOKEN:
 * Error code that's returned by lex_current when some 
 * input from current source couldn't be matched to any type.
 *
 * LEX_EOF:
 * It means that the lexer already reached the end of file.
 *
 * LEX_SUCCESS:
 * The lexer successfully obtained a token from the current 
 * cursor position.
 */
typedef enum { 
  LEX_INVALID_TOKEN = -1, 
  LEX_EOF = 0,
  LEX_SUCCESS = 1,
} LexResult;


#include <stdint.h>

#if UINTPTR_MAX == 0xffffffff
typedef int32_t lex_ssize_t;
#elif UINTPTR_MAX == 0xffffffffffffffff
typedef int64_t lex_ssize_t;
#else
#error Unsupported architecture
#endif

/// FOWARD DECLARATIONS

/* 
 * Initializes a new lexer object on the stack.
 * It expects a LexTypeArray as first argument, which can be
 * created with the macro 'LEX_TYPEARRAY'
 */
Lex lex_init(LexTypeArray types, LexSourceInfo src);

/*
 * Get a token from the current 'l->cursor' position.
 * The function returns true  whenever it successfully obtain 
 * a token, otherwise if it failed or reached the end of file, 
 * the return will be false. As the 'result' paramenter is optional
 * it could be passed as NULL, but if it's not, it will be set to the 
 * corresponding 'LexResult' value. 
 *
 * Also, 'l->has_token' is set to true if it succeed.
 */
bool lex_current(Lex* l, LEX_OPTIONAL LexResult* result);

/*
 * Matches current token with the given 'id', if it matched, returns true 
 * and set optional parameter 'tk' to consumed token. It will automatically call 
 * for 'lex_move' when it successfully matches the desired id, and just return false 
 * otherwise.
 */
bool lex_consume(Lex* l, LEX_OPTIONAL LexToken* tk, LexTypeIndex id);

/*
 * Consume a single token reguardless of it's type.
 */
bool lex_consume_any(Lex *l, LEX_OPTIONAL LexToken *tk); 

/*
 * It will conditionally consume the current token if it's id and string value
 * matches to the given 'id' and 'match' parameters respectively. 
 * The 'match_len' parameter is useful for string without nullbyte terminator.
 * If it is not the case, you could use the simplified version 'lex_skip'.
 */
bool lex_skipn(Lex* l, LexTypeIndex id, const char* match, size_t match_len);

/*
 * It will conditionally consume the current token if it's id and string value
 * matches to the given 'id' and 'match' parameters respectively. 
 */
bool lex_skip(Lex* l, LexTypeIndex id, const char* match);

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
 * This function returns true when there is a token to be processed, even when it's not a valid token.
 */
bool lex_has_next(Lex l);

/*
 * Utilitary function, that can be used to match the char at the current cursor
 * position with a given list of chars. 'chars' could be a string or an array.
 *
 * If matched, returns the length of the match, that in the case of char can only be 1,
 * otherwise it returns 0 (LEX_NO_MATCH).
 */
size_t lex_match_charsn(LexCursor cursor, const char* chars, size_t count);

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
 * If 'can_be_escaped' is set, it will allows the occurrency of 'delimiter' inside the match 
 * if it's preceeded by '\'.
 *
 * For expanding for multiple lines, use 'multiline' option.
 *
 * If matched, returns the length of the match, otherwise LEX_NO_MATCH.
 */
size_t lex_match_wrapped(LexCursor cursor, const char delimiter, bool can_be_escaped, bool multiline);

/*
 * Utilitary function for matching a region that starts with some prefix, and ends with some suffix.
 *
 * For expanding for multiple lines, use 'multiline' option.
 *
 * NOTE: If 'optional_suffix' is set, it will match to the end of file when sufix is not found.
 */
size_t lex_match_region(LexCursor cursor, const char* prefix, const char* suffix, bool optional_suffix, bool multiline);

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

typedef bool (*lex_predicate_fn)(char);

/*
 * Utilitary function for matching charactes while a condition is satisfied.
 * It's useful for matching groups of characters like numbers or letters.
 *
 * 'pred' is a function that receives a char and returns true if the char
 * is part of the match.
 *
 * If matched, returns the length of the match, otherwise LEX_NO_MATCH.
 */
size_t lex_match_while(LexCursor cursor, lex_predicate_fn pred);

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
 * Reset cursor length and position to zero
 */
LEX_INLINE void lex_cursor_reset(LexCursor *cursor);

/*
 * Get cursor location, with file name, line view, number and offset as a LexLocation object.
 */
LexLocation lex_loc(Lex l);

/*
 * Returns the index for both ends of the current line where the cursor is.
 */
void lex_cursor_line_bounds(LexCursor cursor, LEX_OPTIONAL size_t *start, LEX_OPTIONAL size_t *end); 

/*
 * Move cursor by N chars. N could be a negative value, meaning that the cursor will mobe backward.
 */
LEX_INLINE void lex_cursor_move(LexCursor *cursor, lex_ssize_t N);

/*
 * LexStringView contructor:
 * Create a string view from source using specified indexes
 */
LexStringView lex_view_bounds(const char *src, size_t begin, size_t end);

/*
 * LexStringView contructor:
 * Create a string view from a length-based c-string
 */
LEX_INLINE LexStringView lex_viewn(const char* cstr, size_t count);

/*
 * LexStringView contructor:
 * Create a string view from a null-terminated c-string
 */
LEX_INLINE LexStringView lex_view(const char* cstr);

/*
 * Returns a string view of the current token selection
 */
LEX_INLINE LexStringView lex_view_at_cursor(LexCursor cursor);

/*
 * Returns a heap allocated null-terminated c-string copy of a string view.
 */
char* lex_view_dupstr(LexStringView sv);

/*
 * Returns the length of the string view
 */
LEX_INLINE size_t lex_view_count(LexStringView sv);

/*
 * Compare two string views, and return true if they're equals to each other
 */
bool lex_view_eq(LexStringView sv1, LexStringView sv2);

/*
 * Compare string views and null-terminated cstr, and return true if they're equals to each other
 */
LEX_INLINE bool lex_view_eq_cstr(LexStringView sv, const char *cstr);

/*
 * Chops out caracteres from the left and right of a string view
 */
LexStringView lex_view_chop(LexStringView sv, size_t left, size_t right);

/*
 * Trim spaces from the left of a string view
 */
LexStringView lex_view_trim_left(LexStringView sv);

/*
 * Trim spaces from the right of a string view
 */
LexStringView lex_view_trim_right(LexStringView sv);

/*
 * Trim both ends os string view
 */
LEX_INLINE LexStringView lex_view_trim(LexStringView sv);

/*
 * Retunrs true if a string is empty
 */
LEX_INLINE bool lex_view_empty(LexStringView sv);

/*
 * Remove prefix/suffix (like quotes) from a string view
 */
LEX_INLINE LexStringView lex_view_unwrap(LexStringView sv, size_t offset);

/*
 * Create a string view of the current line using lex_cursor_line_bounds 
 */
LexStringView lex_view_current_line(Lex l);

/*
 * This function returns the color code for the given type.
 *
 * Actually, it has a limited amout of colors and styles which rotates when it overflows.
 *
 * The returned string is a internal static memory, that should not be stored. 
 */
const char * lex_print_style(LexTypeIndex type);

/*
 * Print source code to the console, colorizing diferent tokens.
 *
 * Use 'print_labels' to show a small legend containing the meaning of each color.
 */
void lex_print_hl(Lex l, bool print_caption);

/*
 * It will print all type names in a sianlge line
 */
void lex_print_types(Lex l);

/*
 * It works as an iteractive mode for 'lex_print_hl'.
 * It will start a shell-like interface where you can input text to be processed by the lexer,
 * which will print a colorized output, where each color/style applies to a specific type of token.
 */
void lex_repl(LexTypeArray types);

#ifdef LEX_PROFILER
/*
 * Prints information about lex execution.
 * It is intent to be called after tokenization, but you can also print during tokenization if you want to.
 */
void lex_print_profiler(Lex l);
#else 
/*
 * When LEX_PROFILER is not defined the implementation is gone, so it does nothing
 */
#define lex_print_profiler(l) ;
#endif // LEX_PROFILER

#ifndef LEX_DISABLE_BUILTIN_RULES
/*
 * Built-in rule for White-Space tokens.
 * It uses isscape() from 'ctype.h' as a matching rule.
 * NOTE: It's recommend to set 'skip' flag on token type definition (TypeOptions)
 */
size_t lex_builtin_rule_ws(LexCursor cursor);

/*
 * Matches input chas with [a-zA-Z$_] or [a-zA-Z$_0-9] depending on 'allow_numbers'.
 * This functions is used internally by 'lex_builtin_rule_id' and 'lex_match_keyword'. 
 */
LEX_INLINE bool lex_idchar(char ch, bool allow_numbers); 

/*
 * Matches input char with [a-Z_$\-] (could optionally match with numbers too)
 */
LEX_INLINE bool lex_idchar_kebab(char ch, bool allow_numbers);

/*
 * Built-in rule for Identifiers following the most common pattern found on modern languages,
 * useful for user defined names (such as variables and class names).
 * The same as [a-zA-Z$_][a-zA-Z$_0-9]*
 * NOTE: It's usually a good practice to place IDs at the end of your LexTypeArray to 
 * prevent it to override other token types. 
 */
size_t lex_builtin_rule_id(LexCursor cursor);

/*
 * Same as lex_builtin_rule_id, but allwoing kabab-case. Ex: "this-is-an-kebab-id"
 */
size_t lex_builtin_rule_id_kebab(LexCursor cursor);

/*
 * Built-in rule for double quoted strings. (Ex: "Hello world" ) 
 * NOTE: It already handles escaped delimiters < \" >, but scaping the final string is not handled 
 * by the library since the input source code is indent to be imutable.
 */
LEX_INLINE size_t lex_builtin_rule_dqstring(LexCursor cursor);

/*
 * Built-in rule for single quoted strings (Ex: 'Hello world' ). 
 * NOTE: It already handles escaped delimiters < \' >, but scaping the final string is not handled 
 * by the library since the input source code is indent to be imutable.
 */
LEX_INLINE size_t lex_builtin_rule_sqstring(LexCursor cursor);

/*
 * Built-in rule for JavaScript/Python-like string, which can be both single/double 
 * quoted (Ex: "Hello world" or 'Hello world' ). 
 * NOTE: It already handles escaped delimiters < \" | \' >, but scaping the final string is not handled 
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

#endif // LEX_DISABLE_BUILTIN_RULES

/*
 * Exit with status 1, and log message at stderr 
 */
[[noreturn]]
void lex_fatal(const char *message);

/*
 * Reads contents from file.
 */
char *lex_read_file(const char *path, LEX_OPTIONAL size_t *out_file_size);


#ifdef LEX_IMPLEMENTATION

Lex lex_init(LexTypeArray types, LexSourceInfo src) {
  LexType unset = {0};

  for (size_t i = 0; i < types.count; i++) {
    if (memcmp(&types.items[i], &unset, sizeof(LexType)) == 0) {
      char msg[512];
      sprintf(msg, "Lex initialization failed due to an missing type definition. TypeIndex '%zu' is unset", i);
      errno = EINVAL;
      perror(msg);
      exit(1);
    }
  }

  return (Lex) {
    .src = src,
    .types = types,
    .cursor = { .source = src.content,}
  };
}

bool lex_current(Lex* l, LEX_OPTIONAL LexResult* result) {
#ifdef LEX_PROFILER
    l->profiler_data.lex_current.call_count++;
#endif

  if (l->has_token) {
    if (result)
      *result = LEX_SUCCESS;

#ifdef LEX_PROFILER
    l->profiler_data.lex_current.dummy_calls++;
#endif
    return true;
  }

  LexCursor cursor = l->cursor;

  if (cursor.source[cursor.index] == '\0') {
    if (result)
      *result = LEX_EOF;

    return false;
  }

  for (LexTypeIndex id = 0; id < l->types.count; id++) {
    LexType tkdef = l->types.items[id];
    
#ifdef LEX_PROFILER
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    size_t len = tkdef.rule(cursor);
    
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);

    uint64_t time = (end.tv_sec - start.tv_sec) * 1000000000L
                  + end.tv_nsec - start.tv_nsec;
    
    l->types.items[id].profile_data.n_time += time;
    l->types.items[id].profile_data.call_count++;
#else
    size_t len = tkdef.rule(cursor);
#endif // LEX_PROFILER

    if (len != LEX_NO_MATCH) {
      cursor.length = len;

      l->cursor = cursor;
      l->tk = (LexToken){
        .sv = lex_view_at_cursor(cursor),
        .id = id,
      };
      l->has_token = true;

      if (!l->no_skip && l->types.items[id].opt.skip) {
#ifdef LEX_PROFILER
        l->profiler_data.lex_current.skipped_calls++;
#endif
        lex_move(l);
        return lex_current(l, result);
      }

      if (result)
        *result = LEX_SUCCESS;

      return true;      
    }
  }

  if (result)
    *result = LEX_INVALID_TOKEN;

  return false;
}
 

bool lex_consume(Lex* l, LEX_OPTIONAL LexToken* tk, LexTypeIndex id) { 
#ifdef LEX_PROFILER
  l->profiler_data.lex_consume.call_count++;
#endif

  if (lex_current(l, NULL)) {
    if (l->tk.id == id) {
      if (tk)
        *tk = l->tk;

      lex_move(l);

#ifdef LEX_PROFILER
      l->profiler_data.lex_consume.success_count++;
#endif
      return true;
    }
  }

  return false;
}

bool lex_consume_any(Lex *l, LEX_OPTIONAL LexToken *tk) {
  if (lex_current(l, NULL)) {
    if (tk)
      *tk = l->tk;

    lex_move(l);

    return true;
  }

  return false; // no valid tokens
}

bool lex_skipn(Lex* l, LexTypeIndex id, const char* match, size_t match_len) {
#ifdef LEX_PROFILER
  l->profiler_data.lex_skip.call_count++;
#endif
  Lex b = LEX_BRANCH(l);

  LexToken tk;
  if (lex_consume(&b, &tk, id)) {
    if (lex_tklen(b.tk) == match_len && strncmp(lex_tkstr(tk), match, match_len) == 0) {
      LEX_MERGE_BRANCH(l, b);
      
#ifdef LEX_PROFILER
      l->profiler_data.lex_skip.success_count++;
#endif
      return true;
    }
  }

  return false;
}

bool lex_skip(Lex* l, LexTypeIndex id, const char* match) {
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

bool lex_has_next(Lex l) {
  lex_move(&l);
  LexResult result;
  return lex_current(&l, &result) || result == LEX_INVALID_TOKEN;
}

size_t lex_match_charsn(LexCursor cursor, const char* chars, size_t count) {
  for (size_t i = 0; i < count; i++) {
    if (cursor.source[cursor.index] == chars[i])
      return 1; // sizeof char 
  }

  return LEX_NO_MATCH;
}

size_t lex_match_chars(LexCursor cursor, const char* chars) {
  return lex_match_charsn(cursor, chars, strlen(chars));
}

size_t lex_match_keywordn(LexCursor cursor, const char* keyword, size_t len) {
  const char *str = lex_cursor_str(cursor);

  if (strncmp(str, keyword, len) == 0) {
    char lookahead = str[len];

    if (lex_idchar(lookahead, true)) // boundary check: prevent cases like 'interrupt' to be split into 'int' 'errupt' (keyword/id)
      return LEX_NO_MATCH;

    return len;
  }

  return LEX_NO_MATCH;
}

size_t lex_match_keyword(LexCursor cursor, const char* keyword) {
  return lex_match_keywordn(cursor, keyword, strlen(keyword));
}

size_t lex_match_wrapped(LexCursor cursor, const char delimiter, bool can_be_escaped, bool multiline) {
  if (lex_cursor_ch(cursor) != delimiter)
    return LEX_NO_MATCH;

  size_t len = 1;
  size_t slash_counter = 0;
  while (1) {
    lex_cursor_move(&cursor, 1);
    len++;
    
    char ch = lex_cursor_ch(cursor);
    
    // Check for unterminated strings:
    if (ch == '\n' && !multiline) return LEX_NO_MATCH; 
    if (ch == '\0') return LEX_NO_MATCH;
    
    if (can_be_escaped) {
      size_t _slash_counter = slash_counter;
      // Count consecutive back slashes. An odd amount of them means that current char is scaped.
      if (ch == '\\') slash_counter++; else slash_counter = 0;

      // Skip scaped delimiter
      if ((_slash_counter % 2 != 0) && ch == delimiter) continue; 
    }

    // Check if end of string was reached
    if (ch == delimiter) break; 
  }

  return len;
}

size_t lex_match_region(LexCursor cursor, const char* prefix, const char* suffix, bool optional_suffix, bool multiline) {
  size_t prefix_len = strlen(prefix);
  size_t suffix_len = strlen(suffix);

  if (lex_match_exact(cursor, prefix)) {
    LexCursor cur = cursor;
    lex_cursor_move(&cur, prefix_len);

    bool found_suffix = false;
    while (lex_cursor_ch(cur) != '\0') {
      if (lex_match_exact(cur, suffix)) {
        lex_cursor_move(&cur, suffix_len);
        found_suffix = true;
        break;
      }

      if (!multiline && lex_cursor_ch(cur) == '\n') {
        return LEX_NO_MATCH; // probably a broken match, like an unterminatted string
      }
      
      lex_cursor_move(&cur, 1);
    }

    if (optional_suffix || found_suffix)
      return lex_cursor_start(cur) - lex_cursor_start(cursor);
      
    return LEX_NO_MATCH; // Non-optional suffix
  }

  return LEX_NO_MATCH;  
}

size_t lex_match_exactn(LexCursor cursor, const char* match, size_t len) {
  const char *str = lex_cursor_str(cursor);

  if (strncmp(str, match, len) == 0)
    return len;

  return LEX_NO_MATCH;
}

size_t lex_match_exact(LexCursor cursor, const char* match) {
  return lex_match_exactn(cursor, match, strlen(match));
}

size_t lex_match_while(LexCursor cursor, lex_predicate_fn pred) {
  const char *str = lex_cursor_str(cursor);

  size_t len = LEX_NO_MATCH;
  while (str[len] != '\0' && pred(str[len])) len++;

  return len;
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
  char* buf = malloc(len + 1);
  strncpy(buf, lex_tkstr(tk), len);
  buf[len] = '\0';
  
  return buf;
}

void lex_cursor_reset(LexCursor *cursor) {
  cursor->index = cursor->length = 0;
}

LexLocation lex_loc(Lex l) {
  size_t row = 1;
  for (size_t i = 0; i < lex_cursor_start(l.cursor); i++)
    if (*lex_source(l, i) == '\n') row++;

  size_t col = 1;
  for (size_t i = lex_cursor_start(l.cursor); i > 0; i--)
    if (*lex_source(l, i - 1) == '\n') break; 
    else col++;

  return (LexLocation) { 
    .file_name = l.src.file_name,
    .line      = lex_view_current_line(l),
    .row       = row, 
    .col       = col,
  };
}

void lex_cursor_line_bounds(LexCursor cursor, size_t *start, size_t *end) {
  if (start) *start = cursor.index == 0? 0 : (cursor.index - 1);
  if (end)   *end = cursor.index; 

  const char *src = cursor.source;
  while (start || end) {
    if (start) {
      size_t look_behind = (*start) -1;
      if (start == 0 || src[look_behind] == '\n')
        start = NULL;
      else 
        (*start)--;
    }

    if (end) {
      if (src[*end] == '\n' || src[*end] == '\0')
        end = NULL;
      else
        (*end)++;
    }
  }
} 

void lex_cursor_move(LexCursor *cursor, lex_ssize_t N) {
  cursor->index += N;
}

LexStringView lex_view_bounds(const char *src, size_t begin, size_t end) {
  return (LexStringView) { 
    .begin = src + begin,
    .end = src + end 
  };
}

LexStringView lex_viewn(const char* cstr, size_t count) {
  return (LexStringView) { 
    .begin = cstr,
    .end = cstr + count
  };
}

LexStringView lex_view(const char* cstr) {
  return lex_viewn(cstr, strlen(cstr));
}

LexStringView lex_view_at_cursor(LexCursor cursor) {
  const char* cursor_str = lex_cursor_str(cursor);

  return (LexStringView) {
    .begin = cursor_str,
    .end = cursor_str + cursor.length
  };
}

char* lex_view_dupstr(LexStringView sv) {
  size_t len = lex_view_count(sv);
  char *dup = malloc(len + 1);
  memcpy(dup, sv.begin, len);
  dup[len] = '\0';

  return dup;
}

size_t lex_view_count(LexStringView sv) {
  return sv.end - sv.begin;
}

bool lex_view_eq(LexStringView sv1, LexStringView sv2) {
  if (lex_view_count(sv1) != lex_view_count(sv2))
    return false;

  const char *p1 = sv1.begin, *p2 = sv2.begin;
  while (p1 != sv1.end) {
    if (*p1 != *p2)
      return false;

    p1++; p2++;
  }

  return true;
}

bool lex_view_eq_cstr(LexStringView sv, const char *cstr) {
  return lex_view_eq(sv, lex_view(cstr));
}

LexStringView lex_view_chop(LexStringView sv, size_t left, size_t right) {
  sv.begin += left;
  if (sv.begin > sv.end) sv.begin = sv.end;
  
  sv.end -= right;
  if (sv.end < sv.begin) sv.end = sv.begin;

  return sv;
}

LexStringView lex_view_trim_left(LexStringView sv) {
  while (isspace(sv.begin[0]) && !lex_view_empty(sv))
    sv = lex_view_chop(sv, 1, 0);

  return sv;
}

LexStringView lex_view_trim_right(LexStringView sv) {
  while (isspace(sv.end[-1]) && !lex_view_empty(sv))
    sv = lex_view_chop(sv, 0, 1);

  return sv;
}

LexStringView lex_view_trim(LexStringView sv) {
  sv = lex_view_trim_left(sv);
  sv = lex_view_trim_right(sv);
  return sv;
}

bool lex_view_empty(LexStringView sv) { return sv.begin >= sv.end; }

LexStringView lex_view_unwrap(LexStringView sv, size_t offset) {
  return lex_view_chop(sv, offset, offset);
}

LexStringView lex_view_current_line(Lex l) {
  size_t begin, end;
  lex_cursor_line_bounds(l.cursor, &begin, &end);
  
  return lex_view_bounds(l.src.content, begin, end);
}

const char* lex_print_style(LexTypeIndex type) {
#ifdef LEX_DISABLE_COLORS  
  return ""; // Useless if colors are disabled
#else
  // It has 36 different highlights
  static const int colors[] = { 34, 37, 35, 36, 32, 33 }; // red only for erros
  static const int styles[] = { 0,  1,  3,  4,  7,  9 };

  const uint8_t color_count = sizeof(colors) / sizeof(int);
  const uint8_t style_count = sizeof(styles) / sizeof(int);

  uint8_t st = (type / color_count) % style_count;
  uint8_t fg = type % color_count;

  static char buf[8]; // "\e[0;34m" -> '\e' '[' '0' ';' '3' '4' 'm' '\0' (8 chars)
  sprintf(buf, "\e[%1u;%2um", styles[st], colors[fg]);

  return buf;
#endif
}

void lex_print_hl(Lex l, bool print_labels) {
  lex_cursor_reset(&l.cursor); // return to begining of file
  l.no_skip = true;

  LexResult result;
  while (lex_current(&l, &result)) {
#ifdef LEX_DISABLE_COLORS
    LexType type = l.types.items[l.tk.id];
    if (type.opt.skip)
      printf("%s", lex_tkstr_tmp(l.tk));
    else
      printf("%s(%s)", type.name,  lex_tkstr_tmp(l.tk));
#else
    printf("%s%s", lex_print_style(l.tk.id),  lex_tkstr_tmp(l.tk));
#endif
    lex_move(&l);
  }

  if (result == LEX_INVALID_TOKEN) {
    printf("\e[30;41m%c <-- Invalid token " LEX_COLOR_RESET, lex_cursor_ch(l.cursor));
  }

  printf(LEX_COLOR_RESET "\n");

  if (print_labels)
    lex_print_types(l);
}

void lex_print_types(Lex l) {
  printf("Token Types:\n\t");
  for (LexTypeIndex i = 0; i < l.types.count; i++)
    printf("%s%s ", lex_print_style(i), l.types.items[i].name);
    
  printf(LEX_COLOR_RESET "\n");
}

#ifdef LEX_PROFILER

Lex __lex_profiler_branch_wrapper(Lex* l_ptr) {
  Lex b = *l_ptr;
  if (l_ptr->profiler_data.root == NULL) {
    b.profiler_data.root = l_ptr;
  }

  b.profiler_data.root->profiler_data.branch_count++;

  return b;  
}

void __lex_profiler_merge_branch_wrapper(Lex* l_ptr, Lex b) {
  b.profiler_data.root->profiler_data.merge_count++;

  Lex_ProfileData l_data = b.profiler_data;
  if (l_ptr == b.profiler_data.root) {
    l_data = l_ptr->profiler_data;
  }

  *l_ptr = b;
  l_ptr->profiler_data = l_data;
}

void lex_print_profiler(Lex l) {
#ifdef LEX_DISABLE_COLORS
  printf("\n[ Lex Profiler ]\n");
#else
  printf("\n\e[7;37m[ Lex Profiler ]" LEX_COLOR_RESET "\n");
#endif // LEX_DISABLE_COLORS


  Lex_ProfilerStats lcurrent = l.profiler_data.lex_current;
  Lex_ProfilerStats lconsume = l.profiler_data.lex_consume;
  Lex_ProfilerStats lskip    = l.profiler_data.lex_skip;
  
  printf("%-25s: %d\n",    "Total created branchs:",  l.profiler_data.branch_count);
  printf("%-25s: %d\n",    "   Merged branchs:",      l.profiler_data.merge_count);
  printf("%-25s: %d\n",    "Calls to 'lex_current'",  lcurrent.call_count); 
  printf("%-25s: %d\n",    "  Dummy calls (cached):", lcurrent.dummy_calls);
  printf("%-25s: %d\n",    "  Skipped tokens:",       lcurrent.skipped_calls);
  printf("%-25s: %d/%d\n", "  Total wast:",           lcurrent.dummy_calls + lcurrent.skipped_calls, lcurrent.call_count);
  printf("%-25s: %d\n",    "Calls to 'lex_consume'",  lconsume.call_count); 
  printf("%-25s: %d\n",    "  Success count:",        lconsume.success_count);
  printf("%-25s: %d\n",    "Calls to 'lex_skip'",     lskip.call_count); 
  printf("%-25s: %d\n",    "  Success count:",        lskip.success_count);
  
  printf("\nToken matching:\n");
  if (l.profiler_data.lex_current.call_count > 0) {
    uint64_t total_time_ns = 0;
    int total_call = 0; 
    printf("  %-20s %-20s %-20s %-20s\n", "Type", "Time (ns)", "Call count", "Avg (ns/call)");
    for (LexTypeIndex i = 0; i < l.types.count; i++) {
      LexType type = l.types.items[i];

      uint64_t time_ns = type.profile_data.n_time;
      int call_count = type.profile_data.call_count;
      uint64_t avg = call_count? (time_ns / call_count) : -1;

      printf("  %s%-20s" LEX_COLOR_RESET " %-20lu %-20d %-20lu\n", lex_print_style(i), type.name, time_ns, call_count, avg);

      total_time_ns += time_ns;
      total_call += call_count;
    }

    uint64_t total_avg = total_time_ns / total_call;
    double total_time_ms = total_time_ns / 1e6;
    double total_avg_ms = total_avg / 1e6;
    printf("\n");
    printf("%-25s: %lu\n", "  Total time (ns)", total_time_ns); 
    printf("%-25s: %d\n", "  Calls", total_call);
    printf("%-25s: %lu\n", "  Avg. call time (ns)", total_avg);
    printf("%-25s: time: %fms  (avg. of %f ms per rule call)\n", "  Human readable", total_time_ms, total_avg_ms);
    printf("\n");
  } else {
    printf("  <No calls>\n\n");
  }
}
#endif // LEX_PROFILER


void lex_repl(LexTypeArray types) {
  printf("Lex.h Debug REPL v1.0\n\tUse 'h' to show help text, and 'q' to quit.\n");

  char *input = NULL;
  size_t input_capacity = 0;

  while (!feof(stdin)) {
    printf(">> "), fflush(stdout);

    lex_ssize_t len = getline(&input, &input_capacity, stdin);
    if (len > 0) {
      if (input[len - 1] == '\n')
        input[len - 1] = '\0';

      Lex l = lex_init(types, LEX_SRC(NULL, input));

      if (strncmp(input, "h", input_capacity) == 0) {
        printf("You are in the Lex.h Debug REPL.\n");
        printf("Options:\n");
        printf("\t'h' Show this help message.\n");
        printf("\t't' Show lexer types.\n");
        printf("\t'q' Quit from repl.\n");
        continue;
      }

      if (strncmp(input, "t", input_capacity) == 0) {
        lex_print_types(l);
        continue;
      }

      if (strncmp(input, "q", input_capacity) == 0)
        break;
  
      lex_print_hl(l, false);
    }
  }

  if (input)
    free(input);
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


bool lex_idchar(char ch, bool allow_numbers) {
  return isalpha(ch) 
      || ch == '$' 
      || ch == '_' 
      || (allow_numbers && isdigit(ch))
      ;
}

bool lex_idchar_kebab(char ch, bool allow_numbers) {
  return lex_idchar(ch, allow_numbers) || ch == '-';
}

size_t lex_builtin_rule_id(LexCursor cursor) {
  const char *start = lex_cursor_str(cursor);

  if (lex_idchar(start[0], false)) {
    int len = 1;
    for (; start[len] != '\0'; len++) {
      char ch = start[len];
      if (!lex_idchar(ch, true))
        break;
    }

    return len;
  }

  return LEX_NO_MATCH;
}

size_t lex_builtin_rule_id_kebab(LexCursor cursor) {
  const char *start = lex_cursor_str(cursor);

  if (lex_idchar_kebab(start[0], false)) {
    int len = 1;
    for (; start[len] != '\0'; len++) {
      char ch = start[len];
      if (!lex_idchar_kebab(ch, true))
        break;
    }

    return len;
  }

  return LEX_NO_MATCH;
}

size_t lex_builtin_rule_dqstring(LexCursor cursor) {
  return lex_match_wrapped(cursor, '"', true, false);
}

size_t lex_builtin_rule_sqstring(LexCursor cursor) {
  return lex_match_wrapped(cursor, '\'', true, false);
}

size_t lex_builtin_rule_string(LexCursor cursor) {
  size_t len = lex_match_wrapped(cursor, '"', true, false);
  if (len != LEX_NO_MATCH)
    return len;

  return lex_match_wrapped(cursor, '\'', true, false);
}

size_t lex_builtin_rule_pylike_comment(LexCursor cursor) {
  return lex_match_region(cursor, "#", "\n", true, false);
}

size_t lex_builtin_rule_asmlike_comment(LexCursor cursor) {
  return lex_match_region(cursor, ";", "\n", true, false);
}

size_t lex_builtin_rule_clike_comment(LexCursor cursor) {
  return lex_match_region(cursor, "//", "\n", true, false);
}

size_t lex_builtin_rule_clike_mlcomment(LexCursor cursor) {
  return lex_match_region(cursor, "/*", "*/", false, true);
}

#endif // LEX_DISABLE_BUILTIN_RULES

[[noreturn]]
void lex_fatal(const char *message) {
  fprintf(stderr, "%s\n", message);
  exit(1);
}

char *lex_read_file(const char *path, LEX_OPTIONAL size_t *out_file_size) {
  FILE *file = fopen(path, "r");

  if (!file)
    return NULL;

  fseek(file, 0, SEEK_END);
  size_t fsize = ftell(file);
  rewind(file);

  char *content = malloc(fsize + 1);
  if (!content) {
    fclose(file);
    return NULL; 
  }

  size_t total = 0;
  while (total < fsize) {
    size_t n = fread(content, 1, fsize - total, file);

    if (n == 0) {
      if (ferror(file)) {
        free(content);
        fclose(file);
        return NULL;
      }

      break;
    }

    total += n;
  }

  fclose(file);
  content[total] = '\0';
  if (out_file_size)
    *out_file_size = total;
  
  return content;
}

#endif // LEX_IMPLEMENTATION

#ifdef LEX_STRIP_PREFIX

/*
 * By defining LEX_STRIP_PREFIX before including lex.h, you'll be able to
 * access any libray symbol without prefixing with 'lex'.
 *
 * NOTE: If there's some naming conflics with your project libraries,
 * you could use "#undef SYMBOL" to remove it.
 *
 * Ex:
 * #define LEX_STRIP_PREFIX
 * #include <lex.h>
 * #undef SUCCESS
 * #define SUCCESS "overriden symbol"
 */

/// NO PREFIX MACROS

#define NO_MATCH       LEX_NO_MATCH
#define TYPEARRAY      LEX_TYPEARRAY
#define TYPE           LEX_TYPE
#define ETYPE          LEX_ETYPE
#define BRANCH         LEX_BRANCH
#define MERGE_BRANCH   LEX_MERGE_BRANCH
#define SRC            LEX_SRC
#define OPTIONAL       LEX_OPTIONAL
#define COLOR_RESET    LEX_COLOR_RESET
#define tkstr          lex_tkstr
#define tklen          lex_tklen
#define tkname         lex_tkname
#define cursor_str     lex_cursor_str
#define cursor_ch      lex_cursor_ch
#define cursor_start   lex_cursor_start
#define cursor_end     lex_cursor_end
#define l_source       lex_source // too generic
#define SVFMT          LEX_SVFMT
#define svarg          lex_svarg
#define LOCFMT         LEX_LOCFMT
#define locarg         lex_locarg

#ifdef LEX_USE_XMACRO
  #define ENUMX            LEX_ENUMX
  #define TYPEX            LEX_TYPEX
  #define XFORWARD_DECL    LEX_XFORWARD_DECL
  #define XMACRO_FRAMEWORK LEX_XMACRO_FRAMEWORK
#endif

/// NO PREFIX STRUCTURES
#define Cursor       LexCursor
#define Location     LexLocation
#define TypeOptions  LexTypeOptions
#define LType        LexType       // too generic
#define TypeArray    LexTypeArray
#define TypeIndex    LexTypeIndex
#define StringView   LexStringView
#define LToken       LexToken      // too generic
#define SourceInfo   LexSourceInfo
#define LResult      LexResult     // too generic
#define predicate_fn lex_predicate_fn
#define LRule        LexerRule     // too generic
// #define ssize_t      lex_ssize_t // Could conflict with ssize_t from sys/types.h

/// NO PREFIX FUNCTIONS
#define l_init             lex_init             // too generic
#define l_current          lex_current          // too generic
#define l_consume          lex_consume          // too generic
#define consume_any        lex_consume_any
#define skipn              lex_skipn
#define skip(l, id, match) lex_skip(l, id, match)
#define move_to            lex_move_to
#define l_move             lex_move             // too generic
#define has_next           lex_has_next
#define match_charsn       lex_match_charsn
#define match_chars        lex_match_chars
#define match_keywordn     lex_match_keywordn
#define match_keyword      lex_match_keyword
#define match_wrapped      lex_match_wrapped
#define match_exactn       lex_match_exactn
#define match_exact        lex_match_exact
#define match_region       lex_match_region
#define match_while        lex_match_while
#define tkstr_tmp          lex_tkstr_tmp
#define tkstr_dup          lex_tkstr_dup
#define cursor_reset       lex_cursor_reset
#define l_loc              lex_loc // not too generic, but useful. So is better not to be exported
#define cursor_line_bounds lex_cursor_line_bounds
#define cursor_move        lex_cursor_move
#define view_bounds        lex_view_bounds
#define viewn              lex_viewn
#define view               lex_view
#define view_at_cursor     lex_view_at_cursor
#define view_dupstr        lex_view_dupstr
#define view_count         lex_view_count
#define view_eq            lex_view_eq
#define view_eq_cstr       lex_view_eq_cstr
#define view_chop          lex_view_chop
#define view_trim_left     lex_view_trim_left
#define view_trim_right    lex_view_trim_right
#define view_trim          lex_view_trim
#define view_empty         lex_view_empty
#define view_unwrap        lex_view_unwrap
#define view_current_line  lex_view_current_line
#define print_style        lex_print_style
#define print_hl           lex_print_hl
#define print_types        lex_print_types
#define print_profiler     lex_print_profiler
#define repl               lex_repl

#ifndef LEX_DISABLE_BUILTIN_RULES
  #define idchar                       lex_idchar
  #define idchar_kebab                 lex_idchar_kebab
  #define builtin_rule_ws              lex_builtin_rule_ws
  #define builtin_rule_id              lex_builtin_rule_id
  #define builtin_rule_id_kebab        lex_builtin_rule_id_kebab
  #define builtin_rule_dqstring        lex_builtin_rule_dqstring
  #define builtin_rule_sqstring        lex_builtin_rule_sqstring
  #define builtin_rule_string          lex_builtin_rule_string
  #define builtin_rule_pylike_comment  lex_builtin_rule_pylike_comment
  #define builtin_rule_asmlike_comment lex_builtin_rule_asmlike_comment
  #define builtin_rule_clike_comment   lex_builtin_rule_clike_comment
  #define builtin_rule_clike_mlcomment lex_builtin_rule_clike_mlcomment
#endif

#define l_fatal     lex_fatal     // too generic
#define l_read_file lex_read_file // too generic

#endif // LEX_STRIP_PREFIX

#endif // LEX_H 
