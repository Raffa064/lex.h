# About showcases

Each C file in this directory is a self contained project, used to **explain** or **test** some features of the library. You can use them to undestand how the library works.

To build and run showcases, use `make run SHOWCASE=<filename.c>` from the project's root directory.

> [!TIP]
Some showcases contains a lot of useful informations on it's sorce code, so I recommend you to read their **comments**.

> [!IMPORTANT]
As the library still being developed, some of the showcases could be outdated, and may be broken by new versions of the library. We are constantly fixing them, and any reported issue will be addressed as soon as possible.

## Index
- Lexer for a hello world in C
  - [Standard version](https://github.com/Raffa064/lex.h/blob/main/showcases/c-main-prefix.c)
  - [No prefix version](https://github.com/Raffa064/lex.h/blob/main/showcases/c-main.c)
- [Lexer for json](https://github.com/Raffa064/lex.h/blob/main/showcases/json.c)
- [Brain F*ck interpreter](https://github.com/Raffa064/lex.h/blob/main/showcases/bf.c)
- [Missing Types Example](https://github.com/Raffa064/lex.h/blob/main/showcases/missing-types.c)
- Example comparison between `LEX_ETYPES` and `LEX_TYPE`
  - [implicit-index.c](https://github.com/Raffa064/lex.h/blob/main/showcases/implicit-index.c)
  - [explicit-index.c](https://github.com/Raffa064/lex.h/blob/main/showcases/explicit-index.c)
- [Profiling branchs](https://github.com/Raffa064/lex.h/blob/main/showcases/branching.c)
- [Exploring lex_print_hl colorizer](https://github.com/Raffa064/lex.h/blob/main/showcases/)
- [Working CLI Calculator](https://github.com/Raffa064/lex.h/blob/main/showcases/calc.c)
