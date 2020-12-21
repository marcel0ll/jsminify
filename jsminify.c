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

int count_char_at_start(const char *str, const char *target) {
  size_t lenstr = strlen(str);
  int count = 0;
  char * c = str;

  while (memcmp(c, target, 1) == 0 && c < &str[lenstr - 1]) {
    count++;
    c++;
  }

  return count;
}

int count_char_at_end(const char *str, const char *target) {
  size_t lenstr = strlen(str);
  int count = 0;
  char * c = &str[lenstr - 1];

  while (memcmp(c, target, 1) == 0 && c > str) {
    count++;
    c--;
  }

  return count;
}

size_t len_str_int (double value) {
  if (value == 0) return 1;

  size_t len = 0;
  if (value < 0) {
    len++;
    value = fabs(value);
  }

  if (value <= 1) {
    len++;
    return len;
  }

  return (size_t)((ceill(log10(value))) + len);
}

size_t len_str_int_base (double value, int base) {
  if (value == 0) return 1;

  size_t len = 0;
  if (value < 0) {
    len++;
    value = fabs(value);
  }

  if (value <= 1) {
    len++;
    return len;
  }

  return (int)((ceill(log10(value) / log10(base))) + len);
}

char *strremove(char *str, const char *sub, int ignore) {
    char *p, *q, *r;
    size_t len = strlen(sub) - ignore;
    if ((q = r = strstr(str, sub)) != NULL) {
        q = r = q + ignore;
        while ((r = strstr(p = r + len, sub)) != NULL) {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
    return str;
}
void node_number (TSNode node, struct visit_context * context) {
  char * text = ts_node_text(node, context);
  char * subtext = NULL;
  double value;
  size_t text_len = strlen(text);
  if (text_len > 2) {
    if (starts_with("0b", text) || starts_with("0B", text)) {
      // binary
      size_t len = text_len - 2;
      subtext = malloc(len + 1);
      strncpy(subtext, &text[2], len);
      subtext[len] = '\0';
      value = strtoll(subtext, NULL, 2);
    } else if (starts_with("0x", text) || starts_with("0X", text)) {
      // hex
      size_t len = text_len - 2;
      subtext = malloc(len + 1);
      strncpy(subtext, &text[2], len);
      subtext[len] = '\0';
      value = strtoll(subtext, NULL, 16);
    } else if (starts_with("0o", text) || starts_with("0O", text)) {
      // oct
      size_t len = text_len - 2;
      subtext = malloc(len + 1);
      strncpy(subtext, &text[2], len);
      subtext[len] = '\0';
      value = strtoll(subtext, NULL, 8);
    }
  } 
  
  if (subtext == NULL && strlen(text) > 1 && starts_with("0", text) && !starts_with("0.", text)) {
    // oct
    size_t len = text_len - 1;
    subtext = malloc(len + 1);
    strncpy(subtext, &text[1], len);
    subtext[len] = '\0';
    value = strtoll(subtext, NULL, 8);
    if (value == 0L) {
      free(subtext);
      size_t len = strlen(text);
      subtext = malloc(len + 1);
      strncpy(subtext, text, len);
      subtext[len] = '\0';

      value = strtoll(text, NULL, 10);
    }
  } else if (subtext == NULL && strstr(text, "e") || strstr(text, "E")) {
    size_t len = text_len;
    subtext = malloc(len + 1);
    strncpy(subtext, text, len);
    subtext[len] = '\0';

    value = strtold(text, NULL);
  } else if (subtext == NULL && strstr(text, ".")) {
    size_t len = text_len;
    subtext = malloc(len + 1);
    strncpy(subtext, text, len);
    subtext[len] = '\0';

    value = strtold(text, NULL);
  } else if (subtext == NULL) {
    size_t len = text_len;
    subtext = malloc(len + 1);
    strncpy(subtext, text, len);
    subtext[len] = '\0';

    value = strtoll(text, NULL, 10);
  }

  char * new_text =  NULL;
  long long int lli = (long long int) value;
  double res = value - lli;
  if (subtext != NULL) {
    if (res > 0) {
      new_text = malloc(1000);
      sprintf(new_text, "%.16g", value);

      // remove -0 and +0
      char * nt = NULL;
      nt = strremove(new_text, "e+0", 1);
      nt = strremove(new_text, "e+", 1);
      nt = strremove(new_text, "e-0", 2);
    } else {
      new_text = malloc(1000);
      sprintf(new_text, "%lld", lli);
    }
    new_text[strlen(new_text)] = '\0';
  }

  // if number ends with 0s, rewrite it as RADeEXPO
  if(new_text != NULL) {
    int zeroes = count_char_at_end(new_text, "0");
    if (zeroes > 2) {
      size_t enough = ((len_str_int(zeroes) + 1) * sizeof(char));
      char * value_text = malloc(enough);

      size_t rad_len = strlen(new_text) - zeroes;
      char * rad = malloc(rad_len + 1);
      memcpy(rad, text, rad_len);
      rad[rad_len] = '\0';

      sprintf(value_text, "%se%d", rad, zeroes);
      free(new_text);
      new_text = value_text;
    } else if(res <= 0) {
      size_t str_10_len = strlen(new_text);
      // size of value in hex + "0x"
      size_t str_16_len = len_str_int_base(lli, 16) + 2;
      if (str_16_len < str_10_len) {
        // convert to hex
        char * new_value_text = malloc(str_16_len + 1);
        sprintf(new_value_text, "0x%llx", lli);
        if (strlen(new_value_text) < strlen(new_text)) {
          free(new_text);
          new_text = new_value_text;
        }
      }
    }
  }

  if (new_text != NULL && strlen(new_text) < text_len)
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
    "new", "var", "let", "else", "case", "throw", "void",
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
    "super", "extends", "/", NULL };
  context_add_multiple_visitors(context, keyword_types, node_keyword);

  visit_tree(root_node, context);
  printf("\n");

  context_delete(context);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}

