{
  "targets": [{
    "target_name": "jsminify",
    "sources": [
      "jsminify.c",
      "libs/tree-sitter-visitor/tree-sitter-visitor.c",
      "libs/tree-sitter-javascript/src/parser.c",
      "libs/tree-sitter-javascript/src/scanner.c",
      "libs/hashmap.c/hashmap.c"
    ],
    "libraries": [
      "-lm",
      "libs/tree-sitter/libtree-sitter.a",
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
