#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include "../tree-sitter-visitor/tree-sitter-visitor.h"

void node_keyword (TSNode node, char * text) {
  printf("%s", ts_node_type(node));
}

void node_keyword_semi (TSNode node, char * text) {
  printf("%s;", ts_node_type(node));
}

void node_keyword_space (TSNode node, char * text) {
  printf("%s ", ts_node_type(node));
}

void node_spaced_keyword (TSNode node, char * text) {
  printf(" %s ", ts_node_type(node));
}

int slash_counter = 0;
void node_slash (TSNode node, struct visit_context * context) {
  if (slash_counter < 0) {
    slash_counter++;
  } else {
    printf("/");
  }
}

int class_counter = 0;
void node_class (TSNode node, struct visit_context * context) {
  if (class_counter) {
    printf("class ");
  }
  class_counter = (class_counter + 1) % 2;
}

int function_counter = 0;
void node_function (TSNode node, struct visit_context * context) {
  if (function_counter) {
    printf("function ");
  }
  function_counter = (function_counter + 1) % 2;
}

void node_identifier (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_text(node, context));
}

void node_statement_block(TSNode node, struct visit_context * context) {
  function_counter = 0;
}

void node_function_declaration (TSNode node, struct visit_context * context) {
  printf("function ");
}

void node_regex(TSNode node, struct visit_context * context) {
  slash_counter = -2;
  printf("%s", ts_node_text(node, context));
}

void node_text (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_text(node, context));
}

void node_spaced_text (TSNode node, struct visit_context * context) {
  printf(" %s", ts_node_text(node, context));
}

void node_semi (TSNode node, struct visit_context * context) {
  printf(";");
}

int main(int argc, char * argv[]) {
  char * file_path = argv[1];
  if (file_path == NULL) {
    printf("No file passed...\n");
    return 1;
  }
  TSLanguage *tree_sitter_javascript();
  TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_javascript());

  const char *source_code = get_source(file_path);

  TSTree *tree = ts_parser_parse_string(
    parser,
    NULL,
    source_code,
    strlen(source_code)
  );

  TSNode root_node = ts_tree_root_node(tree);

  struct visit_context *context = context_new(source_code);

  context_add_visitor(context, visitor_new("identifier", node_identifier));

  context_add_visitor(context, visitor_new("/", node_slash));
  context_add_visitor(context, visitor_new("regex", node_regex));
  context_add_visitor(context, visitor_new("string", node_text));
  context_add_visitor(context, visitor_new("number", node_text));
  context_add_visitor(context, visitor_new("statement_identifier", node_text));
  context_add_visitor(context, visitor_new("property_identifier", node_text));
  context_add_visitor(context, visitor_new("function_declaration", node_function_declaration));
  context_add_visitor(context, visitor_new("function", node_function));
  context_add_visitor(context, visitor_new("statement_block", node_statement_block));

  const char * class_types[] = {"class_declaration", "class", NULL};
  context_add_multiple_visitors(context, class_types, node_class);

  const char * semi_types[] = { "expression_statement_out",
    "variable_declaration_out", "return_statement_out", "empty_statement",
    "break_statement_out", "continue_statement_out", NULL};
  context_add_multiple_visitors(context, semi_types, node_semi);

  const char * keyword_space_types[] = { "import", "export", "default", "const",
    "new", "var", "let", "else", "case", "break", "continue", "throw",
    "return", "delete", NULL};
  context_add_multiple_visitors(context, keyword_space_types, node_keyword_space);

  const char * spaced_keyword_types[] = { "in", "of", "as",
    "instanceof","typeof", NULL };
  context_add_multiple_visitors(context, spaced_keyword_types, node_spaced_keyword);

  const char * keyword_types[] = { "for", "do", "while", "this", "if", "switch",
    "undefined", "null", "debugger", "get", "set", "yield", "eval", "!", "?", "%",
    "*", "**", "++", "+", "--", "-", "~", ".", ":", "=", "==", "+=", "-=",
    "*=", "/=", "&=", "|=", "<=", ">=", "!=", "===", "!==", ",",  "(",  ")",
    "<",  ">",  "|",  "^",  "&",  ">>", ">>>", "<<", "[",  "]",  "{",  "}",
    "||", "&&", "from", "true", "false", "try", "catch", "finally", "with",
    "super", "extends", "void", NULL };
  context_add_multiple_visitors(context, keyword_types, node_keyword);

  visit_tree(root_node, context);

  context_delete(context);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}

