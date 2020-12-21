#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include <math.h>
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

void node_keyword_space_if_value (TSNode node, char * text) {
  TSNode value = ts_node_child_by_field_name(node, "value", 5);
  printf("%s", ts_node_type(node));

  if (!ts_node_is_null(value)) 
    printf(" ");
}

void node_spaced_keyword (TSNode node, char * text) {
  printf(" %s ", ts_node_type(node));
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

void node_text (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_text(node, context));
}

bool starts_with(const char *pre, const char *str) {
  size_t lenpre = strlen(pre),
         lenstr = strlen(str);
  return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

int ends_with(const char *str, const char *target) {
  size_t lenstr = strlen(str);
  int count = 0;
  char * c = &str[lenstr - 1];

  while (memcmp(c, target, 1) == 0 && c > str) {
    count++;
    c--;
  }

  return count;
}

size_t len_str_int (long long int value) {
  return (int)(ceil(log10(value)));
}

size_t len_str_int_base (long long int value, int base) {
  return (int)(ceil(log10(value) / log10(base)));
}

void node_number (TSNode node, struct visit_context * context) {
  char * text = ts_node_text(node, context);
  char * subtext = NULL;

  long long int value;
  if (starts_with("0b", text) || starts_with("0B", text)) {
    // binary
    size_t len = strlen(text) - 2;
    subtext = malloc(len + 1);
    strncpy(subtext, &text[2], len);
    value = strtol(subtext, NULL, 2);
  } else if (starts_with("0x", text) || starts_with("0X", text)) {
    // hex
    size_t len = strlen(text) - 2;
    subtext = malloc(len + 1);
    strncpy(subtext, &text[2], len);
    value = strtol(subtext, NULL, 16);
  } else if (starts_with("0o", text) || starts_with("0O", text)) {
    // oct
    size_t len = strlen(text) - 2;
    subtext = malloc(len + 1);
    strncpy(subtext, &text[2], len);
    value = strtol(subtext, NULL, 8);
  } else if (starts_with("0", text)) {
    // oct
    size_t len = strlen(text) - 1;
    subtext = malloc(len + 1);
    strncpy(subtext, &text[1], len);
    value = strtol(subtext, NULL, 8);
  } else if (strstr(text, "e") || strstr(text, "E")) {
    // break string into rad and exponent
  } else if (strstr(text, ".")) {
    // not sure
  } else {
    size_t len = strlen(text);
    subtext = malloc(len + 1);
    strncpy(subtext, text, len);

    value = strtol(text, NULL, 10);
  }

  char * new_text =  NULL;
  if (subtext != NULL) {
    size_t enough = ((len_str_int(value) + 1) * sizeof(char));
    new_text = malloc(enough);
    sprintf(new_text, "%lld", value);
  }

  // if number ends with 0s, rewrite it as RADeEXPO
  if(new_text != NULL) {
    int zeroes = ends_with(new_text, "0");
    if (zeroes > 2) {
      size_t enough = ((len_str_int(zeroes) + 1) * sizeof(char));
      char * value_text = malloc(enough);

      size_t rad_len = strlen(text) - zeroes;
      char * rad = malloc(rad_len + 1);
      memcpy(rad, text, rad_len);
      rad[rad_len] = '\0';

      sprintf(value_text, "%se%d", rad, zeroes);
      free(new_text);
      new_text = value_text;
    } else {
      size_t str_10_len = len_str_int(value);
      // size of value in hex + "0x"
      size_t str_16_len = len_str_int_base(value, 16) + 2;
      if (str_16_len < str_10_len) {
        // convert to hex
        char * value_text = malloc(str_16_len + 1);
        sprintf(value_text, "0x%llx", value);
        free(new_text);
        new_text = value_text;
      }
    }
  }

  // if decimal value is shorter
  if (new_text != NULL && strlen(new_text) < strlen(text))
    text = new_text;

  if (new_text != NULL)
    printf("%s", new_text);
  else 
    printf("%s", text);

  if (new_text != NULL) {
    free(new_text);
  }
  if (subtext != NULL) {
    free(subtext);
  }
}

void node_spaced_text (TSNode node, struct visit_context * context) {
  printf(" %s", ts_node_text(node, context));
}

void node_semi (TSNode node, struct visit_context * context) {
  printf(";");
}

int DEBUG = 0;
int main(int argc, char * argv[]) {
  int i;
  for (i = 0; i < argc; i++) {
    char * arg = argv[i];
    if (strcmp("-d", arg) == 0 || strcmp("--debug", arg) == 0) {
      DEBUG = 1;
    }
  }
  char * file_path = argv[argc - 1];
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

  struct visit_context *context = context_new(source_code, DEBUG);

  context_add_visitor(context, visitor_new("identifier", node_identifier));

  context_add_visitor(context, visitor_new("regex_pattern", node_text));
  context_add_visitor(context, visitor_new("regex_flags", node_text));
  context_add_visitor(context, visitor_new("string", node_text));
  context_add_visitor(context, visitor_new("number", node_number));
  context_add_visitor(context, visitor_new("statement_identifier", node_text));
  context_add_visitor(context, visitor_new("property_identifier", node_text));
  context_add_visitor(context, visitor_new("function_declaration", node_function_declaration));
  context_add_visitor(context, visitor_new("function", node_function));
  context_add_visitor(context, visitor_new("statement_block", node_statement_block));

  const char * class_types[] = {"class_declaration", "class", NULL};
  context_add_multiple_visitors(context, class_types, node_class);

  const char * semi_types[] = { "expression_statement_out",
    "variable_declaration_out", "lexical_declaration_out", "return_statement_out", "empty_statement",
    "break_statement_out", "continue_statement_out", NULL};
  context_add_multiple_visitors(context, semi_types, node_semi);

  const char * keyword_space_types[] = { "import", "export", "default", "const",
    "new", "var", "let", "else", "case", "throw",
    "return", "delete", NULL};
  context_add_multiple_visitors(context, keyword_space_types, node_keyword_space);

  const char * keyword_space_if_value_types[] = { "break", "continue", NULL };
  context_add_multiple_visitors(context, keyword_space_if_value_types, node_keyword_space_if_value);

  const char * spaced_keyword_types[] = { "in", "of", "as",
    "instanceof","typeof", NULL };
  context_add_multiple_visitors(context, spaced_keyword_types, node_spaced_keyword);

  const char * keyword_types[] = { "for", "do", "while", "this", "if", "switch",
    "undefined", "null", "debugger", "get", "set", "yield", "eval", "!", "?", "%",
    "*", "**", "++", "+", "--", "-", "~", ".", ":", "=", "==", "+=", "-=",
    "*=", "/=", "&=", "|=", "<=", ">=", "!=", "===", "!==", ",",  "(",  ")",
    "<",  ">",  "|",  "^",  "&",  ">>", ">>>", "<<", "[",  "]",  "{",  "}",
    "||", "&&", "from", "true", "false", "try", "catch", "finally", "with",
    "super", "extends", "void", "/", NULL };
  context_add_multiple_visitors(context, keyword_types, node_keyword);

  visit_tree(root_node, context);

  context_delete(context);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}

