clang                                               \
  -I ./libs/tree-sitter/lib/include                        \
  -I ./libs/tree-sitter/lib/src                            \
  -lm \
  jsminify.c                                        \
  ./libs/tree-sitter-visitor/tree-sitter-visitor.c  \
  ./libs/tree-sitter-javascript/src/parser.c        \
  ./libs/tree-sitter-javascript/src/scanner.c       \
  ./libs/tree-sitter/lib/src/lib.c                  \
  -o jsminify

