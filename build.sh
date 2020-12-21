clang                                   \
  -I ../tree-sitter/lib/include            \
  -lm \
  jsminify.c                    \
  ../tree-sitter/libtree-sitter.a          \
  ../tree-sitter-javascript/src/parser.c         \
  ../tree-sitter-javascript/src/scanner.c         \
  ../hashmap.c/hashmap.c         \
  ../tree-sitter-visitor/tree-sitter-visitor.c         \
  -o jsminify -g

