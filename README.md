# lex.h
`lex.h` a header-only library made in C for lexical analysis, designed to be as fast and flexible as possible. Also, it works completely on the stack, without any deallocation needed.

## Showcases

The project has a [showcases directory](https://github.com/Raffa064/lex.h/blob/main/showcases) including some examples, you can use to understand how to work with the library.

- Lexer for a hello world in C
  - [Standard version](https://github.com/Raffa064/lex.h/blob/main/showcases/c-main-prefix.c)
  - [No prefix version](https://github.com/Raffa064/lex.h/blob/main/showcases/c-main.c)
- [Lexer for json](https://github.com/Raffa064/lex.h/blob/main/showcases/json.c)
- [Brain F*ck interpreter](https://github.com/Raffa064/lex.h/blob/main/showcases/bf.c)
- [Missing Types Example](https://github.com/Raffa064/lex.h/blob/main/showcases/missing-types.c)
- Example comparison between `LEX_ETYPES` and `LEX_TYPE`
  - [implicit-index.c](https://github.com/Raffa064/lex.h/blob/main/showcases/implicit-index.c)
  - [explicit-index.c](https://github.com/Raffa064/lex.h/blob/main/showcases/explicit-index.c)

### Building showcases
To build and run showcases you should clone this repo, and call `make run` at the project's root directory:
```bash
make run SHOWCASE=<filename.c>

# Examples:
make run SHOWCASE=c-main.c
make run SHOWCASE=json.c
make run SHOWCASE=bf.c
```

## How to use
In order to use `lex.h` within your project, the only file needed is the [lex.h](https://raw.githubusercontent.com/Raffa064/lex.h/heads/main/lex.h) itself.

After copying the file into your project, add the following include directive to your source code where you need to use the library:

```c
#define LEX_IMPLEMENTATION
#include "lex.h"
```

As the `LEX_IMPLEMENTATION` option is used to export implementations for the library function, it should be defined **before including the library**, and it should be done only one time, otherwise it will fail at linking time.

> [!TIP]
You can place this snippet on a `lex.c` file, so you just include the header wherever you want without caring about implementation.

- Now, we can start defining our language `token types`.

A token type is defined by a `LexType` structure, which has 3 fields:

- `name`: The name for the token type, useful for logging.
- `rulefn`: A function pointer to the lexer rule that describes how to interpret tokens.
- `options`: A `LexTypeOptions` object, with optional flags.

Here is an example of manual initialization for this struct:
```c
LexType comment = {
	.name = "COMMENT",
	.rulefn = your_rule,
	.options.skip = true
};
```

To declare a token type, you can use LEX_ETYPE to make it shorter:
```c
int COMMENT = 0;
LexType comment = LEX_ETYPE(COMMENT, rulefn, .skip = true);
```
> [!TIP]
In this macro, `options` are defined as a *variadic argument*, so you can define multiple options in "`.opt_name, ...`" format.

Note that the macro expects a integer variable (called `TypeId`), not a string for the "name". It could sound a little bit confusing, but this trick is very useful as you notice later on this tutorial. For now, assume that it is just using the literal name of `COMMENT` as a static string.

The library is all about simplifying the implementation of rules, so there is a lot of helper functions to help you create your own rules. The library also provides some builtin rules you can use to deploy faster, and it is what we are going to use for the examples. But, for instance, a rule function should have this signature:

```c
// The rule should return the length of the match.
// Retuing zero means no match. You can use LEX_NO_MATCH instead of zero to be more readable.
size_t custom_rule(LexCursor cursor);
```

Here is an example for a "unsigned integer" matching rule:

```c
size_t uint_rule(LexCursor cursor) {
  const char *start = curstr(cursor); // get start of the match as string

  size_t len = LEX_NO_MATCH;
  for (; start[len] != '\0'; ++len) { // loop until the end of input
    if (!isdigit(start[len]))         // look for non-number characters
      break;
  }

  return len;
}
```

Now we know how to create a `LexType`, we need to define an array containing all tokens that our language needs. It could be done like so:

```c
enum {WS, COMMENT, STRING, TYPE_COUNT} OurTypes;

LexType our_types[TYPE_COUNT] = {
  LEX_ETYPE(WS,      lex_builtin_rules_ws, .skip = true),      // White Spaces
  LEX_ETYPE(COMMENT, lex_builtin_clike_comment, .skip = true), // comments like this  
  LEX_ETYPE(STRING,  lex_builtin_rules_dqstring),              // Double quoted string
  // ...
} 
```

> [!IMPORTANT]
The order of definition here is the same order in which the lexer will apply the rules, so it must be defined carefully. It's recommended to ALWAYS place WS as the first one, and user-defined identifiers at the end to prevent conflicts.

The **enum** at the beginning of the snippet is the recommended way of defining `TypeIds`. It avoids manually writing the values, and makes it much easier to take the `TYPE_COUNT` as you can see.

Now we have the array done like so, it creates a silly implicit problem. Both `OurTypes` enum and `our_types` needs to be defined in the exact same order, leading to a lot of problems later on if not.

To solve this problem, we can determine that the enum is our source of truth, and use `LEX_TYPE` instead of `LEX_ETYPE`.

```c
enum {WS, COMMENT, STRING, TYPE_COUNT} OurTypes;

LexType our_types[TYPE_COUNT] = {
  LEX_TYPE(WS,      lex_builtin_rules_ws,      .skip = true), // White Spaces
  LEX_TYPE(COMMENT, lex_builtin_clike_comment, .skip = true), // Comments like this one  
  LEX_TYPE(STRING,  lex_builtin_rules_dqstring),              // Double quoted string
  // ...
} 
```

This new macro one has the same functionality, but internally, it will add a **index designator**, which ensures the right index based on `OurTypes` values. For instance, it is same as:
```
...
[WS] = LEX_ETYPE(WS, lex_builtin_rules_ws, .skip = true),
...
```
> That is way you cannot use it outside an LexType static array.

Using this macro also makes compile time warnings when some type is duplicated.

- Creating the lexer with `lex_init`
When coding in C, you can not pass a static array to a function directly without defining some way of knowing it's bounds. So address this issue, `lex.h` has the LexTokenArray structure, which contains a `items` pointer for the original array, and a `count` that stores the amount of items in the array.

You can initialize it manually, or use the `LEX_TYPEARRAY` macro: 
```
LexTypeArray arr = LEX_TYPEARRAY(our_types);
```
> [!IMPORTANT]
The macro expects `our_types` to be an static array, that can be both local or globally initialized as in our previous examples.

We can now use this arr (or the inlined version of it) as the first argument for `lex_init`, which is the function responsible for initializing our lexer as a stack allocated `Lex` object.

```c
Lex lex = lex_init(LEX_TYPEARRAY(our_types), input);
```

And there is! Our lexer is ready to tokenize the input source code, which is the second argument.

In our example, we have defined three types of token: WS and COMMENT which are flagged as `skip`, and STRING. The `skip` flag means that we do not want to read white spaces and comments as tokens.

- Tokenizing a input string
Let's create a small example input string, and print it using `lex_print_hl`.
```c
const char *input = "// Hello\n\"I'm a string\"";
Lex lex = lex_init(LEX_TYPEARRAY(our_types), input);

// This cool utility function will print the `input` colorizing all different token types 
lex_print_hl(lex, true);
```
If everything works as expected, it will print a colorful output, and exit.

So we can use `lex` to iterate over all the tokens inside our input. It is done by calling `lex_current` to emit a token for the current **cursor position** and `lex_move` to move cursor to the next token. It appears like so:

```c
LexResult result;
while (lex_current(&lex, &result)) {
	// consume token stored as "lex->tk"
	printf("Reading token: %s\n", lex_tkname(lex->tk));
	lex_move(&lex);
}
```

This loop will iterate over all the tokens, stopping only at the *End of File* (EOF), or if there is some invalid input that can't be matched to any of `our_types`. You can use `result` to know whether it fails or reached the end of the file by using this code right after the loop:

```c
if (result == LEX_INVALID_TOKEN) {
  fprintf(stderr, "ERROR: Invalid token at %d:%d", lex_curline(lex), lex_curcol(lex));
} else {
  // EOF
}
```

This finishes our tutorial. For more information, the library is well documented at the [source code](https://github.com/Raffa064/lex.h/blob/main/lex.h).

## Design guarantees
- The lexer never allocates or frees memory.
- All lexer state lives on the stack.
- `lex_current` does not advance the cursor.
- Rules are tried in the order they appear in the type array.
- The first matching rule wins.
- Input source is immutable.
- A `LexToken` object remains valid as long as the **input source** is alive.

## Miscellaneous
 
 **PRE-INCLUDE OPTIONS**
 - `LEX_IMPLEMENTATION`: Required to emit the implementation when using `lex.h` as a single-header library.
 - `LEX_STRIP_PREFIX`: It will *#define* all library symbols without `lex_` prefix.
 - `LEX_TYPE_NAME_OFFSET`: Can be used to strip type name prefix (read more about on it's definition).
 - `LEX_DISABLE_BUILTIN_RULES`: Disables all builtin rules (use it if you wanna implement everything by yourself).
 - `LEX_PROFILER`: Enable profiling will expose implemenmtaion for `lex_print_profiler`, which can be used to print information about lex execution.

 
**Built-in rules**
| Function name                   | Description                           | Valid examples                          |
|---------------------------------|---------------------------------------|-----------------------------------------|
|lex_builtin_rule_ws              | White space match                     | ` \n\t\b\0`, `  `                       |
|lex_builtin_rule_id              | User-defined identifiers              | `x`, `foo`, `$_abc123`                  |
|lex_builtin_rule_dqstring        | Double quoted strings                 | `"What is a \"pointer\"?"`              |
|lex_builtin_rule_sqstring        | Single quoted strings                 | `'It\'s handling scapes'`               |
|lex_builtin_rule_string          | Both double/single quoted string      | `"Hello \"world\"?"`, `'That\'s cool!'` |
|lex_builtin_rule_pylike_comment  | Single line sharp comment             | `# Pythonic style`                      |
|lex_builtin_rule_asmlike_comment | Single line semicolon comment         | `; Assembly comment`                    |
|lex_builtin_rule_clike_comment   | Single line double-slash comment      | `// Double-dash comment`                |
|lex_builtin_rule_clike_mlcomment | Multi-line c-like comment             | `/* This is a valid comment */`         |
