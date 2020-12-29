{
  "targets": [{
    "target_name": "jsminify",
    "sources": [
      "jsminify.c",
      "../tree-sitter-visitor/tree-sitter-visitor.c",
      "../tree-sitter-javascript/src/parser.c",
      "../tree-sitter-javascript/src/scanner.c",
      "../hashmap.c/hashmap.c"
    ],
    "libraries": [
      "-lm",
      "../../tree-sitter/libtree-sitter.a",
    ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
      "../tree-sitter/lib/include",
    ],
    "dependencies": [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ],
    "cflags_c": [
      "-std=c99"
    ]
  }]
}
