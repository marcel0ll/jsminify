{
  "targets": [{
    "target_name": "jsminify",
    "sources": [
      "jsminify.c",
      "./libs/tree-sitter-visitor/tree-sitter-visitor.c",
      "./libs/tree-sitter-javascript/src/parser.c",
      "./libs/tree-sitter-javascript/src/scanner.c",
      "./libs/tree-sitter/lib/src/lib.c"
    ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
      "./libs/tree-sitter/lib/include",
      "./libs/tree-sitter/lib/src",
    ],
    "dependencies": [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ]
  }]
}
