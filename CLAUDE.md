# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

JAPC ("Just Another Pascal Compiler") is a C++ front-end compiler for a reduced version of Extended Pascal (ISO 10206), built on top of LLVM 12.0. It parses `.pas` source files, runs semantic analysis, and is intended to lower the AST to LLVM IR via the (currently minimal) `Builder` component.

## Build

Requires LLVM 12.0 dev package (`find_package(LLVM 12.0 REQUIRED CONFIG)`), CMake >= 3.5, and a C++ compiler with C11/C++ support.

```sh
mkdir -p build && cd build
cmake ..
make
```

The compiler executable is `japc` (built from `tools/japc.cpp`, target name `${PROJECT_NAME}` = `japc`).

Useful CMake options:
- `-DBUILD_DOC=ON` — build internal Doxygen docs (requires `docs/Doxyfile.in`, Doxygen installed).
- `-DBUILD_USER_DOC=ON` — build Texinfo user manual from `man/japc_es.texi` / `man/japc_en.texi` (requires `makeinfo`).

## Testing

Tests are registered via CTest in `test/CMakeLists.txt`, driven by the `do_test(name target arg result)` helper which runs `japc` against a `.pas` fixture in `test/` and asserts a regex match (`PASS_REGULAR_EXPRESSION`) against the output.

```sh
cd build
ctest                      # run all tests
ctest -R DuplicatedVariables   # run a single test by name
ctest --output-on-failure  # show output when a test fails
```

To add a new test case, drop a `.pas` fixture into `test/` and add a `do_test(...)` line in `test/CMakeLists.txt`.

## CLI usage

`japc` options are parsed in `lib/basic/options_parser.cpp` (`Pascal::parseJapcOptions`). Known flags include `-h` (help), `-wall`, `-werror`, and include-path / output options (`Pascal::ParsedOptions`).

## Architecture

The compiler is a classic multi-stage pipeline, wired together in `Pascal::Compiler` (`include/japc/basic/compiler.h`, `lib/basic/compiler.cpp`):

```
source file -> Scanner -> Parser -> SemanticAnalizer -> Builder (LLVM IR) [WIP]
```

- **Scanner** (`lib/scanner/scanner.cpp`, `include/japc/scanner/scanner.h`): hand-written lexer. Produces `Token`/`TokenType` from raw source text; tracks `Location` (line/char/file) for diagnostics. Keyword-to-token mapping lives in `Scanner::keywordsToTokenMap`.
- **Parser** (`lib/parser/parser.cpp`, `include/japc/parser/parser.h`): hand-written recursive-descent/Pratt-style parser. Consumes tokens from the `Scanner` and builds an AST of `ExpressionAST` nodes. Maintains a shared `Stack<std::shared_ptr<NamedObject>>` (`include/japc/AST/stack.h`) used as the symbol table scope stack during parsing (so type/variable/function lookups happen as the AST is built, not in a separate pass).
- **AST** (`lib/AST/*.cpp`, `include/japc/AST/*.h`): one class per expression/declaration kind (e.g. `binary_expression`, `if_expression`, `for_expression`, `function_definition`, `variable_declaration_expression`, `type_definition`, etc.), all deriving from `ExpressionAST` (`expression.h`). Visitation uses the classic visitor pattern via `Visitor<T>`/`Visitable<T>` templates in `visitor.h` (`ExpressionVisitor = Visitor<ExpressionAST>`). `named_object.h` is the base for anything that can live in the symbol table (types, variables, functions).
- **Sema** (`lib/sema/sema.cpp`, `lib/sema/typecheck.cpp`): `SemanticAnalizer::analize(ast)` walks the parsed AST for type checking and semantic validation, reporting through `JAPCDiagnostics`.
- **Builder** (`include/japc/builder/builder.h`): abstract base for IR generation (`Builder::createChildBuilder()`); LLVM codegen from the AST is not yet fully implemented — this is the active area of work.
- **Diagnostics** (`lib/basic/diagnostics.cpp`, `include/japc/basic/diagnostics.h`): `JAPCDiagnostics` is threaded through the Scanner, Parser, and Sema for error/warning reporting; `-wall`/`-werror` toggle whether warnings are emitted/promoted to errors (see `WarningOptions` in `options_parser.h`).
- **utils**: standalone string utilities (`utils/string_utils.cpp`), built as its own static library and linked privately into `scanner` and `sema`.

### Library dependency graph (see each `lib/*/CMakeLists.txt`)

```
basic --(INTERFACE)--> ast, parser, sema
ast    --(PUBLIC)----> scanner, basic
parser --(PUBLIC)----> ast, basic, LLVM libs
sema   --(PRIVATE)---> utils, ast   --(INTERFACE)--> parser
scanner--(PRIVATE)---> utils
```

`tools/japc.cpp` (the `japc` executable) only links against `basic`, which interface-propagates the rest.

### Adding a new AST node

New expression/declaration kinds follow a fixed pattern: add a header in `include/japc/AST/<name>.h` declaring a class derived from `ExpressionAST` (or `NamedObject` for symbol-table entries), implement it in `lib/AST/<name>.cpp`, and wire parsing for it into the relevant `Parser::parse*` method in `lib/parser/parser.cpp`. CMake picks up new `lib/AST/*.cpp` files automatically via `FILE(GLOB ...)`.

## Code style

`.clang-format` is based on the Microsoft style (generated from CLion C/C++ Code Style settings). Format C++ changes accordingly before committing.
